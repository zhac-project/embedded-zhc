// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the LifeControl (MCLH series) ports. The vendor
// parity sweep fixed three auto-generated ports that dropped channels:
//
//   * MCLH-02 "vivi ZLight" smart bulb — z2m m.light({colorTemp, color:
//     true}); the port wired only genOnOff + genLevelCtrl, dropping the
//     lightingColorCtrl (0x0300) half. Now wires kFzColorTemperature +
//     kFzColor so color_temp / hue / saturation decode.
//   * MCLH-03 "RICI01" smart socket — z2m m.electricityMeter() default
//     cluster "both" → seMetering (0x0702) energy + haElectricalMeasure-
//     ment (0x0B04) power/voltage/current. The port wired only
//     kFzMetering, dropping voltage/current. Now wires
//     kFzElectricalMeasurement alongside.
//   * MCLH-08 "VOC_Sensor" air-quality sensor — z2m's custom airQuality()
//     packs temperature/humidity/eco2/voc into ONE msTemperatureMeasure-
//     ment (0x0402) report (measuredValue / minMeasuredValue /
//     maxMeasuredValue / tolerance). The port wired only kFzBattery, so
//     all four readings were dead. Now wires the vendor
//     kFzLifecontrolAirQuality converter (incl. the negative-temperature
//     wrap).
//
// z2m-source: zigbee-herdsman-converters/src/devices/lifecontrol.ts

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lifecontrol {
extern const PreparedDefinition kDef_MCLH_02;
extern const PreparedDefinition kDef_MCLH_03;
extern const PreparedDefinition kDef_MCLH_08;
}  // namespace zhc::devices::lifecontrol

using namespace zhc;

namespace {

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

// LE byte helpers.
std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}
std::vector<std::uint8_t> u48(std::uint64_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8),
            static_cast<std::uint8_t>(x >> 16), static_cast<std::uint8_t>(x >> 24),
            static_cast<std::uint8_t>(x >> 32), static_cast<std::uint8_t>(x >> 40)};
}

const Value* decode_one(const PreparedDefinition& def, std::uint16_t cluster,
                        const char* cname, std::uint16_t attr, std::uint8_t type,
                        const std::vector<std::uint8_t>& val, const char* key) {
    auto r = dispatch_zcl(def, cluster, cname, 1, attr_report(attr, type, val));
    assert(r.any_matched);
    return r.merged.find(key);
}

void expect_uint(const PreparedDefinition& def, std::uint16_t cluster,
                 const char* cname, std::uint16_t attr, std::uint8_t type,
                 const std::vector<std::uint8_t>& val, const char* key,
                 std::uint64_t want) {
    assert(def_exposes(def, key));
    const Value* v = decode_one(def, cluster, cname, attr, type, val, key);
    assert(v && v->type == ValueType::Uint && v->u == want);
}

void expect_int(const PreparedDefinition& def, std::uint16_t cluster,
                const char* cname, std::uint16_t attr, std::uint8_t type,
                const std::vector<std::uint8_t>& val, const char* key,
                std::int64_t want) {
    assert(def_exposes(def, key));
    const Value* v = decode_one(def, cluster, cname, attr, type, val, key);
    assert(v && v->type == ValueType::Int && v->i == want);
}

// seMetering 0x0702, haElectricalMeasurement 0x0B04, lightingColorCtrl
// 0x0300, msTemperatureMeasurement 0x0402.
constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t HA_ELEC     = 0x0B04;
constexpr std::uint16_t COLOR_CTRL  = 0x0300;
constexpr std::uint16_t TEMP_MEAS   = 0x0402;

// ── MCLH-02: colour CT bulb — colour-temp + xy/hue-sat decode ────────
void check_mclh02() {
    const auto& def = devices::lifecontrol::kDef_MCLH_02;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));

    // colorTemperature attr 0x0007 (u16) → color_temp.
    expect_uint(def, COLOR_CTRL, "lightingColorCtrl", 0x0007, 0x21, u16(250), "color_temp", 250);
    // currentHue attr 0x0000 (u8) → hue.
    const std::uint8_t hue[] = {0x40};
    {
        auto r = dispatch_zcl(def, COLOR_CTRL, "lightingColorCtrl", 1, attr_report(0x0000, 0x20, hue));
        const Value* v = r.merged.find("hue");
        assert(v && v->type == ValueType::Uint && v->u == 0x40);
    }
    // currentSaturation attr 0x0001 (u8) → saturation.
    const std::uint8_t sat[] = {0x80};
    {
        auto r = dispatch_zcl(def, COLOR_CTRL, "lightingColorCtrl", 1, attr_report(0x0001, 0x20, sat));
        const Value* v = r.merged.find("saturation");
        assert(v && v->type == ValueType::Uint && v->u == 0x80);
    }
}

// ── MCLH-03: smart socket — 0x0B04 voltage/current + 0x0702 energy ───
void check_mclh03() {
    const auto& def = devices::lifecontrol::kDef_MCLH_03;

    // Newly-wired haElectricalMeasurement half.
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0505, 0x21, u16(2301), "voltage", 2301); // rmsVoltage
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0508, 0x21, u16(1500), "current", 1500); // rmsCurrent
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x050B, 0x29, u16(0x00FA), "power", 250);   // activePower s16

    // Regression: seMetering energy still decodes.
    expect_uint(def, SE_METERING, "seMetering", 0x0000, 0x25, u48(987654), "energy", 987654);
}

// ── MCLH-08: air-quality sensor — packed 0x0402 report ───────────────
void check_mclh08() {
    const auto& def = devices::lifecontrol::kDef_MCLH_08;

    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "eco2"));
    assert(def_exposes(def, "voc"));
    assert(def_exposes(def, "battery"));

    // measuredValue 0x0000 (s16) → temperature 21.5 °C (z2m /100 → Float).
    {
        auto r = dispatch_zcl(def, TEMP_MEAS, "msTemperatureMeasurement", 1, attr_report(0x0000, 0x29, u16(2150)));
        assert(r.any_matched);
        const Value* v = r.merged.find("temperature");
        assert(v && v->type == ValueType::Float && v->f > 21.49f && v->f < 21.51f);
    }
    // minMeasuredValue 0x0001 (u16) → humidity 45.0 % (z2m /100 → Float).
    {
        auto r = dispatch_zcl(def, TEMP_MEAS, "msTemperatureMeasurement", 1, attr_report(0x0001, 0x21, u16(4500)));
        const Value* v = r.merged.find("humidity");
        assert(v && v->type == ValueType::Float && v->f > 44.99f && v->f < 45.01f);
    }
    // maxMeasuredValue 0x0002 (u16) → eco2 (ppm raw).
    expect_uint(def, TEMP_MEAS, "msTemperatureMeasurement", 0x0002, 0x21, u16(800), "eco2", 800);
    // tolerance 0x0003 (u16) → voc (ppb raw).
    expect_uint(def, TEMP_MEAS, "msTemperatureMeasurement", 0x0003, 0x21, u16(125), "voc", 125);

    // Negative-temperature wrap: measuredValue < -1000 → -(v+32767)*5/3,
    // then /100. s16 -2000 = 0xF830 → centi -(-2000+32767)*5/3 = -51278
    // → -512.78 °C.
    {
        auto r = dispatch_zcl(def, TEMP_MEAS, "msTemperatureMeasurement", 1, attr_report(0x0000, 0x29, u16(0xF830)));
        const Value* v = r.merged.find("temperature");
        assert(v && v->type == ValueType::Float && v->f > -512.79f && v->f < -512.77f);
    }

    // Regression: battery via genPowerCfg still decodes.
    const std::uint8_t batt[] = {0xC8};  // batteryPercentageRemaining 0x0021, 200 → dontDivide → 200/.. raw 200
    {
        auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1, attr_report(0x0021, 0x20, batt));
        assert(r.any_matched);
    }
}

}  // namespace

int main() {
    check_mclh02();
    check_mclh03();
    check_mclh08();
    return 0;
}
