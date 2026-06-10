// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Woox (Tuya-OEM) family. Found by a z2m<->embedded-zhc
// parity pass over the 6 existing woox defs (woox.ts + lib/legacy.ts).
//
// Bug fixed:
//   * R7051 (TS0219 smart siren): z2m decodes the alarm via
//     fz.ias_alarm_only_alarm_1 — ZoneStatus (ssIasZone attr 0x0002) bit 0 →
//     `alarm` ONLY. The generated def lowered the broader kFzIasZone, which
//     also emits tamper + battery_low, and declared phantom `tamper` /
//     `battery_low` exposes z2m never surfaces for this device. Re-pointed at
//     the new kFzIasZoneAlarmOnly and dropped the two phantom exposes.
//
// The remaining fixtures pin the Tuya-DP decoders (R7049 smoke alarm, R7060
// irrigation, R7067 TRV) against the z2m legacy converter semantics
// (invert-bool, enum maps, per-DP scale, comfort/eco DP assignment) so the
// already-correct ports stay correct.
//
// z2m-source: zigbee-herdsman-converters/src/devices/woox.ts + lib/legacy.ts.

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::woox {
extern const PreparedDefinition kDef_R7051;  // siren  → IAS alarm-only
extern const PreparedDefinition kDef_R7049;  // smoke  → Tuya-DP
extern const PreparedDefinition kDef_R7060;  // irrigation → Tuya-DP
extern const PreparedDefinition kDef_R7067;  // TRV    → Tuya-DP
}  // namespace zhc::devices::woox

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

double as_num(const Value* v) {
    assert(v);
    return (v->type == ValueType::Int) ? double(v->i)
         : (v->type == ValueType::Uint) ? double(v->u)
         : v->f;
}

// ── ssIasZone ZoneStatus attribute-report harness (cluster 0x0500) ────
InboundApsFrame make_zcl_frame(std::uint16_t cluster,
                               std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};   // fc, tsn, cmd=ReportAttributes
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_zcl_frame(cluster_id, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Report ssIasZone ZoneStatus (attr 0x0002, MAP16 0x19) with the given bits.
DispatchResult dispatch_zonestatus(const PreparedDefinition& def,
                                   std::uint16_t zone_status) {
    const std::uint8_t zs[] = {
        static_cast<std::uint8_t>(zone_status & 0xFF),
        static_cast<std::uint8_t>(zone_status >> 8),
    };
    return dispatch_zcl(def, 0x0500, "ssIasZone", attr_report(0x0002, 0x19, zs));
}

// ── Tuya-DP frame harness (0xEF00, command 0x02 dataReport) ───────────
DecodedMessage make_dp_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_dp_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_dp_msg();
    auto raw = make_dp_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

DispatchResult decode_value(const PreparedDefinition& def, std::uint8_t dp,
                            std::uint32_t value) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((value >> 24) & 0xFF),
        static_cast<std::uint8_t>((value >> 16) & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF),
        static_cast<std::uint8_t>(value & 0xFF),
    };
    const TuyaDpRecord recs[] = { { dp, 0x02, std::span<const std::uint8_t>(b, 4) } };
    return dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
}

DispatchResult decode_bool(const PreparedDefinition& def, std::uint8_t dp,
                           std::uint8_t on) {
    const std::uint8_t b[] = { on };
    const TuyaDpRecord recs[] = { { dp, 0x01, std::span<const std::uint8_t>(b, 1) } };
    return dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
}

const char* decode_enum(const PreparedDefinition& def, std::uint8_t dp,
                        std::uint8_t wire, const char* out_key) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = { { dp, 0x04, std::span<const std::uint8_t>(b, 1) } };
    const auto r = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(r.any_matched);
    const Value* v = r.merged.find(out_key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

// ── R7051 — siren: IAS alarm-only (no phantom tamper / battery_low) ───
void test_r7051_alarm_only() {
    const auto& d = devices::woox::kDef_R7051;
    assert(def_exposes(d, "alarm"));
    // Phantom exposes z2m never surfaces (fz.ias_alarm_only_alarm_1).
    assert(!def_exposes(d, "tamper"));
    assert(!def_exposes(d, "battery_low"));

    // alarm_1 (bit 0) asserted → alarm true, no tamper / battery_low keys.
    auto on = dispatch_zonestatus(d, 0x0001);
    assert(on.any_matched);
    const Value* av = on.merged.find("alarm");
    assert(av && av->type == ValueType::Bool && av->b);
    assert(on.merged.find("tamper") == nullptr);
    assert(on.merged.find("battery_low") == nullptr);

    // Clear → alarm false.
    auto off = dispatch_zonestatus(d, 0x0000);
    assert(off.any_matched);
    const Value* off_av = off.merged.find("alarm");
    assert(off_av && off_av->type == ValueType::Bool && !off_av->b);

    // Even with tamper(bit2)+battery_low(bit3) set on the wire, only `alarm`
    // surfaces — matching z2m's alarm-only converter.
    auto tb = dispatch_zonestatus(d, 0x000C);
    assert(tb.any_matched);
    assert(tb.merged.find("tamper") == nullptr);
    assert(tb.merged.find("battery_low") == nullptr);
}

// ── R7049 — smoke alarm: Tuya-DP (R7049_status) ───────────────────────
void test_r7049_smoke() {
    const auto& d = devices::woox::kDef_R7049;
    assert(def_exposes(d, "smoke"));

    // DP1 smoke: z2m does Boolean(!value) — invert-bool. wire 0 → smoke TRUE.
    auto s0 = decode_bool(d, 1, 0);
    const Value* sv0 = s0.merged.find("smoke");
    assert(sv0 && sv0->type == ValueType::Bool && sv0->b);
    auto s1 = decode_bool(d, 1, 1);
    const Value* sv1 = s1.merged.find("smoke");
    assert(sv1 && sv1->type == ValueType::Bool && !sv1->b);

    // DP14 battery_level enum {0:low,1:middle,2:high}.
    assert(std::strcmp(decode_enum(d, 14, 0, "battery_level"), "low") == 0);
    assert(std::strcmp(decode_enum(d, 14, 2, "battery_level"), "high") == 0);

    // DP9 test_alarm_result enum.
    assert(std::strcmp(decode_enum(d, 9, 1, "test_alarm_result"),
                       "check_success") == 0);

    // DP20 alarm: z2m maps {0:true, 1:false} (inverted-enum lookup).
    assert(std::strcmp(decode_enum(d, 20, 0, "alarm"), "true") == 0);
    assert(std::strcmp(decode_enum(d, 20, 1, "alarm"), "false") == 0);
}

// ── R7060 — irrigation: Tuya-DP (woox_R7060) ──────────────────────────
// NOTE: the ez def types DP102 as a Bool with invert-bool (0 → state ON,
// nonzero → OFF). z2m's woox_R7060 instead reads the raw integer and maps
// `value === 2 ? "OFF" : "ON"`. For a 0/1 wire that agrees with the
// invert-bool model; the value===2 "OFF" / value===1 "ON" Value-DP variant
// is ambiguous from z2m alone and hardware-gated — see gaps[]. This fixture
// pins the def's declared Bool contract.
void test_r7060_switch_battery() {
    const auto& d = devices::woox::kDef_R7060;
    assert(def_exposes(d, "state"));
    assert(def_exposes(d, "battery"));

    // DP102 state (Bool + invert): wire 0 → state ON, wire 1 → state OFF.
    auto on = decode_bool(d, 102, 0);
    const Value* sv_on = on.merged.find("state");
    assert(sv_on && sv_on->type == ValueType::Bool && sv_on->b);
    auto off = decode_bool(d, 102, 1);
    const Value* sv = off.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && !sv->b);

    // DP101 battery (Value).
    auto b = decode_value(d, 101, 77);
    const Value* bv = b.merged.find("battery");
    assert(bv && std::fabs(as_num(bv) - 77.0) < 0.01);
}

// ── R7067 — TRV: Tuya-DP (woox_thermostat) ────────────────────────────
void test_r7067_thermostat() {
    const auto& d = devices::woox::kDef_R7067;
    // Flat exposes, no Climate composite (project rule).
    assert(def_exposes(d, "local_temperature"));
    assert(def_exposes(d, "current_heating_setpoint"));
    assert(def_exposes(d, "comfort_temperature"));
    assert(def_exposes(d, "eco_temperature"));

    // DP24 local_temperature /10: wire 215 → 21.5 C.
    auto lt = decode_value(d, 24, 215);
    const Value* ltv = lt.merged.find("local_temperature");
    assert(ltv && ltv->type == ValueType::Float);
    assert(std::fabs(as_num(ltv) - 21.5) < 0.01);

    // DP16 manual setpoint /2: wire 44 → 22.0 C.
    auto sp = decode_value(d, 16, 44);
    const Value* spv = sp.merged.find("current_heating_setpoint");
    assert(spv && spv->type == ValueType::Float);
    assert(std::fabs(as_num(spv) - 22.0) < 0.01);

    // Comfort = DP101, Eco = DP102 (z2m wooxComfortTemperature:101,
    // wooxEnergySavingTemperature:102), both /2.
    auto cf = decode_value(d, 101, 46);   // 46/2 = 23.0
    const Value* cfv = cf.merged.find("comfort_temperature");
    assert(cfv && std::fabs(as_num(cfv) - 23.0) < 0.01);
    auto ec = decode_value(d, 102, 34);   // 34/2 = 17.0
    const Value* ecv = ec.merged.find("eco_temperature");
    assert(ecv && std::fabs(as_num(ecv) - 17.0) < 0.01);

    // DP30 child_lock enum {0:UNLOCK,1:LOCK}.
    assert(std::strcmp(decode_enum(d, 30, 1, "child_lock"), "LOCK") == 0);
    assert(std::strcmp(decode_enum(d, 30, 0, "child_lock"), "UNLOCK") == 0);
}

}  // namespace

int main() {
    test_r7051_alarm_only();
    test_r7049_smoke();
    test_r7060_switch_battery();
    test_r7067_thermostat();
    return 0;
}
