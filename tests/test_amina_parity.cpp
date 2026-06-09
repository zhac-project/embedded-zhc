// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Amina amina S EV Charger (single def, endpoint 10).
//
// The amina S drives its AC metering through z2m's
// `m.electricityMeter({cluster: "electrical", acFrequency: true,
// threePhase: true})` plus a standalone `m.numeric total_active_power`
// (attribute `totalActivePower` 0x0304, key total_active_power, kW).
// That expose set spans STANDARD haElectricalMeasurement (0x0B04):
//   power / voltage / current (base electrical branch),
//   ac_frequency (acFrequency:true),
//   power_phase_b/c, voltage_phase_b/c, current_phase_b/c (threePhase:true),
//   total_active_power (the standalone numeric).
//
// The hand-rewrite leaned solely on the generic kFzElectricalMeasurement
// (activePower 0x050B / rmsVoltage 0x0505 / rmsCurrent 0x0508 only) and
// only exposed `power`/`energy`, so voltage/current were decoded-but-
// unexposed and ac_frequency / total_active_power / every per-phase b/c
// channel were dropped entirely. The device's PRIMARY power reading is
// total_active_power (0x0304) — amina.ts `fzLocal.poll_energy` keys its
// energy poll off `msg.data.totalActivePower`. The fix adds
// kFzAminaElectricalMeasurementExtras (definitions/amina/_shared.cpp),
// wired alongside the generic, plus the missing exposes.
//
// The EV-specific state / alarms / energy / offline-config channels ride
// amina's manuSpecific cluster 0xFEE7 and were already decoded by the
// per-device converters in Ami_amina_S.cpp; verified here for regression
// (they must keep firing now that the extras converter coexists).
//
// z2m-source: zigbee-herdsman-converters/src/devices/amina.ts +
//             lib/modernExtend.ts genericMeter (electricityMeter
//             cluster:"electrical" / acFrequency / threePhase).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::amina {
extern const PreparedDefinition kDef_amina_S;
}  // namespace zhc::devices::amina

using namespace zhc;

namespace {

// ── ZCL attribute report builder: fc, tsn, cmd=0x0A, then records ─────
// Manu-specific reports (0xFEE7) carry the manufacturer-specific frame
// bit (0x04) + the 2-byte manufacturer code after the fc byte; the
// generic 0x0B04 reports use the plain server-to-client header.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value,
                                      std::uint16_t mfg = 0) {
    std::vector<std::uint8_t> v;
    if (mfg != 0) {
        v.push_back(0x1C);  // fc: manu-specific | server-to-client | cluster? no — global cmd, manu-spec bit set
        v.push_back(static_cast<std::uint8_t>(mfg & 0xFF));
        v.push_back(static_cast<std::uint8_t>(mfg >> 8));
    } else {
        v.push_back(0x18);  // fc: global cmd, server-to-client, disable default rsp
    }
    v.push_back(0x42);  // tsn
    v.push_back(0x0A);  // cmd: reportAttributes
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

// ZCL fixed-size little-endian payload helpers.
std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}
std::vector<std::uint8_t> u32(std::uint32_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8),
            static_cast<std::uint8_t>(x >> 16), static_cast<std::uint8_t>(x >> 24)};
}

constexpr std::uint16_t HA_ELEC      = 0x0B04;
constexpr std::uint16_t AMINA_CLUSTER = 0xFEE7;
constexpr std::uint16_t AMINA_MFG     = 0x143B;

const Value* decode_one(const PreparedDefinition& def, std::uint16_t cluster,
                        const char* cname, std::uint16_t attr, std::uint8_t type,
                        const std::vector<std::uint8_t>& val, const char* key,
                        std::uint16_t mfg = 0) {
    auto r = dispatch_zcl(def, cluster, cname, 10, attr_report(attr, type, val, mfg));
    assert(r.any_matched);
    return r.merged.find(key);
}

void expect_uint(const PreparedDefinition& def, std::uint16_t cluster,
                 const char* cname, std::uint16_t attr, std::uint8_t type,
                 const std::vector<std::uint8_t>& val, const char* key,
                 std::uint64_t want, std::uint16_t mfg = 0) {
    assert(def_exposes(def, key));
    const Value* v = decode_one(def, cluster, cname, attr, type, val, key, mfg);
    assert(v && v->type == ValueType::Uint && v->u == want);
}

void expect_int(const PreparedDefinition& def, std::uint16_t cluster,
                const char* cname, std::uint16_t attr, std::uint8_t type,
                const std::vector<std::uint8_t>& val, const char* key,
                std::int64_t want, std::uint16_t mfg = 0) {
    assert(def_exposes(def, key));
    const Value* v = decode_one(def, cluster, cname, attr, type, val, key, mfg);
    assert(v && v->type == ValueType::Int && v->i == want);
}

// ── haElectricalMeasurement extras (the fix) ────────────────────────
void check_elec_extras() {
    const auto& def = devices::amina::kDef_amina_S;

    // ac_frequency 0x0300 (u16) — acFrequency:true.
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0300, 0x21, u16(5000), "ac_frequency", 5000);

    // total_active_power 0x0304 (s32) — the device's primary power channel.
    expect_int(def, HA_ELEC, "haElectricalMeasurement", 0x0304, 0x2B, u32(0xFFFFFC18), "total_active_power", -1000); // -1000

    // Per-phase B/C (threePhase:true). Power s16, voltage/current u16.
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x090B, 0x29, u16(800),    "power_phase_b", 800);    // activePowerPhB
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x0A0B, 0x29, u16(0xFD44), "power_phase_c", -700);   // activePowerPhC s16
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0905, 0x21, u16(2302),   "voltage_phase_b", 2302); // rmsVoltagePhB
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0A05, 0x21, u16(2303),   "voltage_phase_c", 2303); // rmsVoltagePhC
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0908, 0x21, u16(1600),   "current_phase_b", 1600); // rmsCurrentPhB
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0A08, 0x21, u16(1700),   "current_phase_c", 1700); // rmsCurrentPhC
}

// ── Regression: the generic ELM still owns power/voltage/current ────
void check_generic_regression() {
    const auto& def = devices::amina::kDef_amina_S;

    // activePower 0x050B (s16) -> power.
    expect_int (def, HA_ELEC, "haElectricalMeasurement", 0x050B, 0x29, u16(0x0BB8), "power", 3000);
    // rmsVoltage 0x0505 (u16) -> voltage (now exposed).
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0505, 0x21, u16(2300), "voltage", 2300);
    // rmsCurrent 0x0508 (u16) -> current (now exposed).
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0508, 0x21, u16(1500), "current", 1500);
}

// ── Regression: the amina manuSpecific 0xFEE7 decoders still fire ───
void check_custom_cluster_regression() {
    const auto& def = devices::amina::kDef_amina_S;

    // evStatus 0x03 (BITMAP16): bit0 ev_connected, bit2 charging, bit4 derated.
    // value 0b0000000000010101 = bits 0,2,4 set -> connected+charging+derated.
    {
        auto r = dispatch_zcl(def, AMINA_CLUSTER, "aminaControlCluster", 10,
                              attr_report(0x0003, 0x19, u16(0x0015), AMINA_MFG));  // BITMAP16
        assert(r.any_matched);
        const Value* c = r.merged.find("charging");
        const Value* e = r.merged.find("ev_connected");
        const Value* d = r.merged.find("derated");
        assert(c && c->type == ValueType::Bool && c->b == true);
        assert(e && e->type == ValueType::Bool && e->b == true);
        assert(d && d->type == ValueType::Bool && d->b == true);
    }

    // alarms 0x02 (BITMAP16): non-zero -> alarm_active true + raw bitmap.
    {
        auto r = dispatch_zcl(def, AMINA_CLUSTER, "aminaControlCluster", 10,
                              attr_report(0x0002, 0x19, u16(0x0080), AMINA_MFG));  // overcurrent bit 7
        assert(r.any_matched);
        const Value* a = r.merged.find("alarm_active");
        const Value* raw = r.merged.find("alarms");
        assert(a && a->type == ValueType::Bool && a->b == true);
        assert(raw && raw->type == ValueType::Uint && raw->u == 0x0080);
    }

    // totalActiveEnergy 0x10 (u32) -> energy ; lastSessionEnergy 0x11 -> last_session_energy.
    expect_uint(def, AMINA_CLUSTER, "aminaControlCluster", 0x0010, 0x23, u32(123456), "energy", 123456, AMINA_MFG);
    expect_uint(def, AMINA_CLUSTER, "aminaControlCluster", 0x0011, 0x23, u32(789),   "last_session_energy", 789, AMINA_MFG);

    // genLevelCtrl currentLevel 0x0000 (u8) -> charge_limit.
    {
        const std::uint8_t lvl[] = {16};
        auto r = dispatch_zcl(def, 0x0008, "genLevelCtrl", 10, attr_report(0x0000, 0x20, lvl));
        assert(r.any_matched);
        const Value* v = r.merged.find("charge_limit");
        assert(v && v->type == ValueType::Uint && v->u == 16);
    }
}

// ── power_factor must NOT be exposed (z2m default powerFactor:false) ─
void check_no_phantom_channels() {
    const auto& def = devices::amina::kDef_amina_S;
    assert(!def_exposes(def, "power_factor"));
    assert(!def_exposes(def, "power_factor_phase_b"));
    // produced_energy is not in the amina model either.
    assert(!def_exposes(def, "produced_energy"));
}

}  // namespace

int main() {
    check_elec_extras();
    check_generic_regression();
    check_custom_cluster_regression();
    check_no_phantom_channels();
    return 0;
}
