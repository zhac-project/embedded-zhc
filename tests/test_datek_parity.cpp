// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for two Datek devices:
//
//   HSE2927E "Motion Sensor" (Eva motion) — z2m's `extend:
//   [m.illuminance()]` adds a msIlluminanceMeasurement (0x0400)
//   measuredValue -> "illuminance" decoder + expose that the generated
//   port dropped (it wired only battery/temperature/IAS-motion/
//   led_on_motion). Fixed by wiring the generic kFzIlluminance + adding
//   the expose + 0x0400 bind.
//
//   HSE2905E "Meter Reader" (Eva AMS HAN
// power-meter sensor). z2m stacks two `m.electricityMeter` bundles plus
// `m.temperature()`:
//   metering   (producedEnergy: true)        -> energy + produced_energy + power
//   electrical (threePhase: true, power: false)
//              -> voltage/current phase A + phase B/C + ac_frequency + power_factor
// The generic kFzMetering (seMetering energy 0x0000 / power 0x0400) and
// kFzElectricalMeasurement (haElectricalMeasurement power 0x050B /
// voltage 0x0505 / current 0x0508) decode only the phase-A core, so
// produced_energy, the phase B/C voltage/current channels, ac_frequency
// and power_factor were dead exposes. The fix wires Datek
// kFzMeteringExtras (seMetering) + kFzElectricalMeasurementExtras
// (haElectricalMeasurement) ALONGSIDE the generics; all numeric channels
// are raw pass-through (runtime scales downstream).
//
// Verified here:
//   * produced_energy (seMetering 0x0001 CurrentSummReceived, u48).
//   * ac_frequency (0x0300), power_factor (0x0510), and the phase B/C
//     voltage (0x0905/0x0A05) and current (0x0908/0x0A08) channels.
//   * Regression: the generic converters still own energy / power /
//     voltage / current.
//
// z2m-source: zigbee-herdsman-converters/src/devices/datek.ts #HSE2905E
//             + lib/modernExtend.ts genericMeter (electricityMeter).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::datek {
extern const PreparedDefinition kDef_HSE2905E;
extern const PreparedDefinition kDef_HSE2927E;
}  // namespace zhc::devices::datek

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

constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t HA_ELEC     = 0x0B04;
constexpr std::uint16_t MS_ILLUM    = 0x0400;

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

// ── HSE2905E: dual electricityMeter bundle + temperature ─────────────
void check_hse2905e() {
    const auto& def = devices::datek::kDef_HSE2905E;

    // seMetering extras: produced_energy (0x0001 CurrentSummReceived, u48).
    expect_uint(def, SE_METERING, "seMetering", 0x0001, 0x25, u48(424242),
                "produced_energy", 424242);

    // haElectricalMeasurement extras:
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0300, 0x21, u16(5000),
                "ac_frequency", 5000);   // 50.00 Hz raw
    // power_factor: s8 -50 (0xCE) surfaced as Int.
    {
        const std::uint8_t pf[] = {0xCE};
        auto r = dispatch_zcl(def, HA_ELEC, "haElectricalMeasurement", 1,
                              attr_report(0x0510, 0x28 /*INT8*/, pf));
        assert(def_exposes(def, "power_factor"));
        const Value* v = r.merged.find("power_factor");
        assert(v && v->type == ValueType::Int && v->i == -50);
    }
    // Phase B/C voltage + current (UINT16).
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0905, 0x21, u16(2302),
                "voltage_phase_b", 2302);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0A05, 0x21, u16(2303),
                "voltage_phase_c", 2303);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0908, 0x21, u16(1600),
                "current_phase_b", 1600);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0A08, 0x21, u16(1700),
                "current_phase_c", 1700);

    // ── Regression: the generic converters still own the core channels ──
    expect_uint(def, SE_METERING, "seMetering", 0x0000, 0x25, u48(987654),
                "energy", 987654);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0505, 0x21, u16(2301),
                "voltage", 2301);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0508, 0x21, u16(1500),
                "current", 1500);
    // ActivePower 0x050B (s16) -> power via generic kFzElectricalMeasurement.
    expect_int(def, HA_ELEC, "haElectricalMeasurement", 0x050B, 0x29, u16(0xFC18),
               "power", -1000);
    // temperature expose still present.
    assert(def_exposes(def, "temperature"));
}

// ── HSE2927E: motion sensor illuminance channel ─────────────────────
void check_hse2927e() {
    const auto& def = devices::datek::kDef_HSE2927E;

    // msIlluminanceMeasurement 0x0400 measuredValue (attr 0x0000, u16)
    // -> "illuminance" via generic kFzIlluminance.
    expect_uint(def, MS_ILLUM, "msIlluminanceMeasurement", 0x0000, 0x21,
                u16(12345), "illuminance", 12345);

    // Regression: existing channels still decode/expose.
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "led_on_motion"));
    assert(def_exposes(def, "occupancy"));
}

}  // namespace

int main() {
    check_hse2905e();
    check_hse2927e();
    return 0;
}
