// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Nexelec Open'R port. The vendor parity sweep fixed
// an auto-generated port that dropped channels:
//
//   * Open'R "Air Quality Sensor Nexelec" — z2m extend = [m.temperature(),
//     m.humidity(), m.co2(), m.battery(), m.identify()] →
//     msTemperatureMeasurement (0x0402) /100, msRelativeHumidity (0x0405)
//     /100, msCO2 (0x040D) ppm, genPowerCfg (0x0001) battery. The port
//     wired ONLY kFzBattery, so temperature/humidity/co2 were dead. Now
//     wires kFzTemperature + kFzHumidity + kFzCO2 alongside kFzBattery.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nexelec.ts

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

namespace zhc::devices::nexelec {
extern const PreparedDefinition kDef_Open_R;
}  // namespace zhc::devices::nexelec

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

// LE byte helper.
std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

// Cluster ids.
constexpr std::uint16_t TEMP_MEAS = 0x0402;  // msTemperatureMeasurement
constexpr std::uint16_t HUMIDITY  = 0x0405;  // msRelativeHumidity
constexpr std::uint16_t CO2_MEAS  = 0x040D;  // msCO2
constexpr std::uint16_t POWER_CFG = 0x0001;  // genPowerCfg

// ── Open'R: CO2 + temperature + humidity multi-sensor (+ battery) ────
void check_open_r() {
    const auto& def = devices::nexelec::kDef_Open_R;

    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "co2"));
    assert(def_exposes(def, "battery"));

    // measuredValue 0x0000 (s16) → temperature 21.5 °C (z2m /100 → Float).
    {
        auto r = dispatch_zcl(def, TEMP_MEAS, "msTemperatureMeasurement", 1,
                              attr_report(0x0000, 0x29, u16(2150)));
        assert(r.any_matched);
        const Value* v = r.merged.find("temperature");
        assert(v && v->type == ValueType::Float && v->f > 21.49f && v->f < 21.51f);
    }
    // measuredValue 0x0000 (u16) → humidity 45.0 % (z2m /100 → Float).
    {
        auto r = dispatch_zcl(def, HUMIDITY, "msRelativeHumidity", 1,
                              attr_report(0x0000, 0x21, u16(4500)));
        assert(r.any_matched);
        const Value* v = r.merged.find("humidity");
        assert(v && v->type == ValueType::Float && v->f > 44.99f && v->f < 45.01f);
    }
    // measuredValue 0x0000 (u16, already ppm) → co2 800.
    {
        auto r = dispatch_zcl(def, CO2_MEAS, "msCO2", 1,
                              attr_report(0x0000, 0x21, u16(800)));
        assert(r.any_matched);
        const Value* v = r.merged.find("co2");
        assert(v && v->type == ValueType::Uint && v->u == 800);
    }
    // Regression: battery via genPowerCfg still decodes.
    {
        const std::uint8_t batt[] = {0xC8};  // batteryPercentageRemaining 0x0021
        auto r = dispatch_zcl(def, POWER_CFG, "genPowerCfg", 1,
                              attr_report(0x0021, 0x20, batt));
        assert(r.any_matched);
    }
}

}  // namespace

int main() {
    check_open_r();
    return 0;
}
