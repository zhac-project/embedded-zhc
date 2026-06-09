// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the BITUO TECHNIK DIN-rail energy-meter family
// (SDM01/SDM02 single/split-phase, SPM01/SPM02 three-phase; U00/U01/U02
// SKUs). The auto-generated ports catalogued the full z2m expose set
// (produced_energy, ac_frequency, power_factor, power_apparent,
// power_reactive, total_power*, the per-phase b/c voltage/current/power
// and the per-phase tier-summation energy counters) but the generic
// `kFzMetering` (seMetering 0x0000 energy / 0x0400 power only) and
// `kFzElectricalMeasurement` (haElectricalMeasurement 0x050B power /
// 0x0505 voltage / 0x0508 current only) decode none of them — so every
// extra channel was a dead expose.
//
// z2m drives all 11 with `m.electricityMeter` (producedEnergy +
// acFrequency + powerFactor, threePhase on SDM01W/SPM02) plus the
// vendor `bituo_fz.total_power` / `bituo_fz.phase_energy` converters,
// which read STANDARD attrs (no manufacturer-specific cluster). The fix
// wires `kFzBituoMeteringExtras` (seMetering) + `kFzBituoElectrical
// MeasurementExtras` (haElectricalMeasurement) ALONGSIDE the generics on
// every def. All numeric channels are raw pass-through, matching the
// generic converters (runtime scales downstream).
//
// Verified here:
//   * SDM01B-U01 single-phase: produced_energy / ac_frequency /
//     power_factor / power_apparent decode; generic energy/power/
//     voltage/current still decode (regression).
//   * SPM02-U01 three-phase + phase_energy: per-phase b/c voltage/
//     current/power, total_power*, power_reactive, and per-phase tier
//     energy/produced_energy all decode.
//   * SPM01-U02 (no onOff SKU): carries no `state` expose; produced_
//     energy + ac_frequency decode; generic energy still decodes.
//
// z2m-source: zigbee-herdsman-converters/src/devices/bituo_technik.ts
//             (bituo_fz.total_power / bituo_fz.phase_energy) +
//             lib/modernExtend.ts genericMeter (electricityMeter).

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

namespace zhc::devices::bituo_technik {
extern const PreparedDefinition kDef_SDM01B_U01;
extern const PreparedDefinition kDef_SPM02_U01;
extern const PreparedDefinition kDef_SPM01_U02;
}  // namespace zhc::devices::bituo_technik

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

// seMetering 0x0702, haElectricalMeasurement 0x0B04.
constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t HA_ELEC     = 0x0B04;

// ZCL fixed-size little-endian payload helpers.
std::vector<std::uint8_t> u16(std::uint16_t x) { return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)}; }
std::vector<std::uint8_t> u32(std::uint32_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8),
            static_cast<std::uint8_t>(x >> 16), static_cast<std::uint8_t>(x >> 24)};
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

// ── SDM01B-U01: single-phase, the base electricityMeter channel set ──
void check_sdm01b() {
    const auto& def = devices::bituo_technik::kDef_SDM01B_U01;

    // seMetering extras: produced_energy (attr 0x0001 CurrentSummReceived, u48).
    expect_uint(def, SE_METERING, "seMetering", 0x0001, 0x25, u48(123456), "produced_energy", 123456);

    // haElectricalMeasurement extras:
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0300, 0x21, u16(5000), "ac_frequency", 5000);   // 50.00 Hz raw
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x0510, 0x29, u16(0xFFCE), "power_factor", -50);   // s16 -50
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x050F, 0x21, u16(2300), "power_apparent", 2300);  // VA raw

    // ── Regression: the generic converters still own the core channels ──
    expect_uint(def, SE_METERING, "seMetering", 0x0000, 0x25, u48(987654), "energy", 987654);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0505, 0x21, u16(2301), "voltage", 2301);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0508, 0x21, u16(1500), "current", 1500);
    // InstantaneousDemand 0x0400 (s24) → power via generic kFzMetering.
    const std::uint8_t demand[] = {0xE8, 0x03, 0x00};   // 1000
    auto rp = dispatch_zcl(def, SE_METERING, "seMetering", 1, attr_report(0x0400, 0x2A, demand));
    const Value* pv = rp.merged.find("power");
    assert(pv && pv->type == ValueType::Int && pv->i == 1000);
}

// ── SPM02-U01: three-phase + bituo_fz.total_power + phase_energy ─────
void check_spm02() {
    const auto& def = devices::bituo_technik::kDef_SPM02_U01;

    // Per-phase B/C from haElectricalMeasurement.
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x090B, 0x29, u16(800),  "power_phase_b", 800);    // activePowerPhB
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x0A0B, 0x29, u16(0xFD44), "power_phase_c", -700); // activePowerPhC s16
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0905, 0x21, u16(2302), "voltage_phase_b", 2302); // rmsVoltagePhB
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0A05, 0x21, u16(2303), "voltage_phase_c", 2303); // rmsVoltagePhC
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0908, 0x21, u16(1600), "current_phase_b", 1600); // rmsCurrentPhB
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0A08, 0x21, u16(1700), "current_phase_c", 1700); // rmsCurrentPhC

    // total_power* (bituo_fz.total_power).
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x0304, 0x2B, u32(0xFFFFF000), "total_power", -4096);       // s32 TotalActivePower
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x0305, 0x2B, u32(2500), "total_power_reactive", 2500);     // s32 TotalReactivePower
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0306, 0x23, u32(7000), "total_power_apparent", 7000);     // u32 TotalApparentPower

    // power_reactive (reactivePower 0x050E, s16).
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x050E, 0x29, u16(900), "power_reactive", 900);

    // ac_frequency / power_factor still decode on the 3-phase variant.
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0300, 0x21, u16(5001), "ac_frequency", 5001);
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x0510, 0x29, u16(995), "power_factor", 995);

    // Per-phase tier-summation energy (bituo_fz.phase_energy, u48):
    expect_uint(def, SE_METERING, "seMetering", 0x0102, 0x25, u48(1001), "energy_phase_a", 1001);          // Tier1Delivered
    expect_uint(def, SE_METERING, "seMetering", 0x0103, 0x25, u48(11),   "produced_energy_phase_a", 11);   // Tier1Received
    expect_uint(def, SE_METERING, "seMetering", 0x0104, 0x25, u48(1002), "energy_phase_b", 1002);          // Tier2Delivered
    expect_uint(def, SE_METERING, "seMetering", 0x0105, 0x25, u48(12),   "produced_energy_phase_b", 12);   // Tier2Received
    expect_uint(def, SE_METERING, "seMetering", 0x0106, 0x25, u48(1003), "energy_phase_c", 1003);          // Tier3Delivered
    expect_uint(def, SE_METERING, "seMetering", 0x0107, 0x25, u48(13),   "produced_energy_phase_c", 13);   // Tier3Received

    // produced_energy (whole-meter) + regression on generic energy.
    expect_uint(def, SE_METERING, "seMetering", 0x0001, 0x25, u48(55555), "produced_energy", 55555);
    expect_uint(def, SE_METERING, "seMetering", 0x0000, 0x25, u48(66666), "energy", 66666);
}

// ── SPM01-U02: no-onOff SKU — no `state` expose, channels still decode ─
void check_spm01_u02() {
    const auto& def = devices::bituo_technik::kDef_SPM01_U02;

    assert(!def_exposes(def, "state"));        // U02 has no m.onOff()
    assert(def_exposes(def, "energy"));

    expect_uint(def, SE_METERING, "seMetering", 0x0001, 0x25, u48(4242), "produced_energy", 4242);
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0300, 0x21, u16(4999), "ac_frequency", 4999);
    // Regression: generic energy decode.
    expect_uint(def, SE_METERING, "seMetering", 0x0000, 0x25, u48(7777), "energy", 7777);
}

}  // namespace

int main() {
    check_sdm01b();
    check_spm02();
    check_spm01_u02();
    return 0;
}
