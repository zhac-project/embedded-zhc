// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Alecto SMART-HEAT10 (radiator-valve TRV thermostat) and
// SMART-SMOKE10 (smoke detector) — both Tuya-MCU (0xEF00 manuSpecificTuya)
// DP-stream devices (TS0601 / _TZE200_* + _TYST11_*).
//
// Bug fixed:
//   * SMART-HEAT10 was auto-ported as the generic kFzThermostat / kTzThermostat
//     (standard hvacThermostat 0x0201 cluster). The device speaks NONE of that —
//     z2m decodes it via legacy.fz.tuya_thermostat, a 0xEF00 siterwell-style DP
//     stream. The misroute left a dead 0x0201 binding and dropped child_lock /
//     window_detection / battery.
//   * SMART-SMOKE10 was auto-ported as a generic IAS-zone (ssIasZone 0x0500)
//     emitting bare alarm/tamper/battery_low. z2m decodes it via
//     legacy.fz.tuya_alecto_smoke, a 0xEF00 DP stream. The IAS-vs-DP misroute
//     left every real channel (smoke_state, smoke_value, checking_result,
//     battery_state, battery, lifecycle, self_checking, silence) dead.
// Both were graduated to Tier-2 overrides wiring fz_tuya_datapoints +
// tz_tuya_datapoints with the full DP map.
//
// SUSPECT (wrong scale): HEAT10 setpoint/local_temperature ARE /10 (confirmed
// from z2m value/10). SMOKE10 smoke_value/battery are raw pass-through (z2m
// applies NO divisor) — confirmed FALSE there.
//
// z2m-source: alecto.ts + lib/legacy.ts fz.tuya_thermostat / fz.tuya_alecto_smoke.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/alecto_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> alecto_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::alecto::kAlectoRegistry, devices::alecto::kAlectoRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

DecodedMessage make_msg() {
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

InboundApsFrame make_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

const PreparedDefinition* heat10() {
    const auto* d = find_definition("TS0601", "_TZE200_8daqwrsj", alecto_registry());
    assert(d && "_TZE200_8daqwrsj must resolve to the Alecto SMART-HEAT10 def");
    return d;
}

const PreparedDefinition* smoke10() {
    const auto* d = find_definition("TS0601", "_TZE200_qtbrwrfv", alecto_registry());
    assert(d && "_TZE200_qtbrwrfv must resolve to the Alecto SMART-SMOKE10 def");
    return d;
}

}  // namespace

// Both manufacturer fingerprints (and the legacy _TYST11_ variant) resolve to
// full Tuya-DP defs carrying a from_zigbee + to_zigbee DP converter — not a
// generic thermostat / IAS-zone stub.
static void test_manu_resolves_to_full_dp_def() {
    const auto* h = heat10();
    assert(h->model && std::strcmp(h->model, "SMART-HEAT10") == 0);
    assert(h->vendor && std::strcmp(h->vendor, "Alecto") == 0);
    assert(h->from_zigbee && h->from_zigbee_count >= 2);  // sync-time + DP map
    assert(h->to_zigbee && h->to_zigbee_count >= 1);
    // legacy fingerprint variant resolves to the same def.
    assert(find_definition("daqwrsj", "_TYST11_8daqwrsj", alecto_registry()) == h);

    const auto* s = smoke10();
    assert(s->model && std::strcmp(s->model, "SMART-SMOKE10") == 0);
    assert(s->from_zigbee && s->from_zigbee_count >= 2);
    assert(s->to_zigbee && s->to_zigbee_count >= 1);
    assert(find_definition("tbrwrfv", "_TYST11_qtbrwrfv", alecto_registry()) == s);
}

// HEAT10 binds 0xEF00 (the Tuya MCU cluster), NOT the phantom hvacThermostat
// 0x0201 the auto stub wired. SMOKE10 binds 0xEF00, NOT ssIasZone 0x0500.
static void test_bindings_are_tuya_not_phantom() {
    const auto* h = heat10();
    bool has_ef00_h = false, has_0201 = false;
    for (std::size_t i = 0; i < h->bindings_count; ++i) {
        if (h->bindings[i].cluster_id == 0xEF00) has_ef00_h = true;
        if (h->bindings[i].cluster_id == 0x0201) has_0201 = true;
    }
    assert(has_ef00_h && !has_0201);

    const auto* s = smoke10();
    bool has_ef00_s = false, has_0500 = false;
    for (std::size_t i = 0; i < s->bindings_count; ++i) {
        if (s->bindings[i].cluster_id == 0xEF00) has_ef00_s = true;
        if (s->bindings[i].cluster_id == 0x0500) has_0500 = true;
    }
    assert(has_ef00_s && !has_0500);
}

// HEAT10 exposes the restored flat thermostat channels (no Climate composite),
// and the phantom standard-thermostat-only surface is gone.
static void test_heat10_exposes_declared() {
    const auto* h = heat10();
    assert(def_exposes(*h, "local_temperature"));
    assert(def_exposes(*h, "current_heating_setpoint"));
    assert(def_exposes(*h, "system_mode"));
    assert(def_exposes(*h, "child_lock"));        // dropped by the auto stub
    assert(def_exposes(*h, "window_detection"));  // dropped by the auto stub
    assert(def_exposes(*h, "battery"));           // dropped by the auto stub
}

// HEAT10 setpoint (DP2) + local_temperature (DP3) decode /10 -> Float
// (z2m value/10). Confirms the scale SUSPECT.
static void test_heat10_temp_divisor_10() {
    const auto* h = heat10();
    // current_heating_setpoint DP2 = 215 -> 21.5°C (0x000000D7).
    const std::uint8_t kSp[] = { 0x00, 0x00, 0x00, 0xD7 };
    // local_temperature DP3 = 230 -> 23.0°C (0x000000E6).
    const std::uint8_t kLt[] = { 0x00, 0x00, 0x00, 0xE6 };
    const TuyaDpRecord recs[] = {
        { 2, 0x02, std::span<const std::uint8_t>(kSp, 4) },
        { 3, 0x02, std::span<const std::uint8_t>(kLt, 4) },
    };
    const auto r = dispatch_dp(*h, std::span<const TuyaDpRecord>(recs, 2));
    assert(r.any_matched);
    const Value* sp = r.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Float && sp->f > 21.49 && sp->f < 21.51);
    const Value* lt = r.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Float && lt->f > 22.99 && lt->f < 23.01);
}

// HEAT10 system_mode (DP4) enum: 0->off, 1->auto, 2->heat
// (z2m preset -> presetToSystemMode = thermostatSystemModes4).
static void test_heat10_system_mode_enum() {
    const auto* h = heat10();
    const std::uint8_t kHeat[] = { 0x02 };
    const TuyaDpRecord recs[] = { { 4, 0x04, std::span<const std::uint8_t>(kHeat, 1) } };
    const auto r = dispatch_dp(*h, std::span<const TuyaDpRecord>(recs, 1));
    assert(r.any_matched);
    const Value* sm = r.merged.find("system_mode");
    assert(sm && sm->type == ValueType::StringRef && std::strcmp(sm->str, "heat") == 0);

    const std::uint8_t kOff[] = { 0x00 };
    const TuyaDpRecord recsOff[] = { { 4, 0x04, std::span<const std::uint8_t>(kOff, 1) } };
    const auto rOff = dispatch_dp(*h, std::span<const TuyaDpRecord>(recsOff, 1));
    const Value* smOff = rOff.merged.find("system_mode");
    assert(smOff && std::strcmp(smOff->str, "off") == 0);
}

// HEAT10 child_lock (DP7 bool -> "LOCK"/"UNLOCK") + window_detection
// (DP18 bool -> "ON"/"OFF"), and battery (DP21) raw pass-through Int.
static void test_heat10_bool_enums_and_battery() {
    const auto* h = heat10();
    const std::uint8_t kLock[] = { 0x01 };   // child_lock ON
    const std::uint8_t kWin[]  = { 0x01 };   // window_detection ON
    const std::uint8_t kBat[]  = { 0x00, 0x00, 0x00, 0x4B };  // battery 75 %
    const TuyaDpRecord recs[] = {
        { 7,  0x01, std::span<const std::uint8_t>(kLock, 1) },
        { 18, 0x01, std::span<const std::uint8_t>(kWin, 1) },
        { 21, 0x02, std::span<const std::uint8_t>(kBat, 4) },
    };
    const auto r = dispatch_dp(*h, std::span<const TuyaDpRecord>(recs, 3));
    assert(r.any_matched);
    const Value* cl = r.merged.find("child_lock");
    assert(cl && cl->type == ValueType::StringRef && std::strcmp(cl->str, "LOCK") == 0);
    const Value* wd = r.merged.find("window_detection");
    assert(wd && wd->type == ValueType::StringRef && std::strcmp(wd->str, "ON") == 0);
    const Value* bat = r.merged.find("battery");
    assert(bat && bat->type == ValueType::Int && bat->i == 75);
}

// SMOKE10 exposes the restored DP channels; the phantom IAS surface is gone.
static void test_smoke10_exposes_declared() {
    const auto* s = smoke10();
    assert(def_exposes(*s, "smoke_state"));
    assert(def_exposes(*s, "smoke_value"));
    assert(def_exposes(*s, "checking_result"));
    assert(def_exposes(*s, "battery_state"));
    assert(def_exposes(*s, "battery"));
    assert(def_exposes(*s, "lifecycle"));
    assert(def_exposes(*s, "self_checking"));
    assert(def_exposes(*s, "silence"));
    // The auto IAS stub's keys must be gone.
    assert(!def_exposes(*s, "alarm"));
    assert(!def_exposes(*s, "tamper"));
    assert(!def_exposes(*s, "battery_low"));
}

// SMOKE10 enum DPs: smoke_state (DP1) {0:alarm,1:normal}, checking_result
// (DP9) {0:checking,1:check_success,2:check_failure,3:others}, battery_state
// (DP14) {0:low,1:middle,2:high}.
static void test_smoke10_enum_dps() {
    const auto* s = smoke10();
    const std::uint8_t kAlarm[] = { 0x00 };
    const std::uint8_t kFail[]  = { 0x02 };
    const std::uint8_t kHigh[]  = { 0x02 };
    const TuyaDpRecord recs[] = {
        { 1,  0x04, std::span<const std::uint8_t>(kAlarm, 1) },
        { 9,  0x04, std::span<const std::uint8_t>(kFail, 1) },
        { 14, 0x04, std::span<const std::uint8_t>(kHigh, 1) },
    };
    const auto r = dispatch_dp(*s, std::span<const TuyaDpRecord>(recs, 3));
    assert(r.any_matched);
    const Value* st = r.merged.find("smoke_state");
    assert(st && st->type == ValueType::StringRef && std::strcmp(st->str, "alarm") == 0);
    const Value* cr = r.merged.find("checking_result");
    assert(cr && cr->type == ValueType::StringRef && std::strcmp(cr->str, "check_failure") == 0);
    const Value* bs = r.merged.find("battery_state");
    assert(bs && bs->type == ValueType::StringRef && std::strcmp(bs->str, "high") == 0);
}

// SMOKE10 numeric DPs raw pass-through (no divisor): smoke_value (DP2),
// battery (DP15). Confirms the scale SUSPECT is FALSE.
static void test_smoke10_numeric_raw() {
    const auto* s = smoke10();
    const std::uint8_t kSmoke[] = { 0x00, 0x00, 0x00, 0x2A };  // smoke_value 42
    const std::uint8_t kBat[]   = { 0x00, 0x00, 0x00, 0x5A };  // battery 90 %
    const TuyaDpRecord recs[] = {
        { 2,  0x02, std::span<const std::uint8_t>(kSmoke, 4) },
        { 15, 0x02, std::span<const std::uint8_t>(kBat, 4) },
    };
    const auto r = dispatch_dp(*s, std::span<const TuyaDpRecord>(recs, 2));
    assert(r.any_matched);
    const Value* sv = r.merged.find("smoke_value");
    assert(sv && sv->type == ValueType::Int && sv->i == 42);
    const Value* bat = r.merged.find("battery");
    assert(bat && bat->type == ValueType::Int && bat->i == 90);
}

// SMOKE10 bool DPs: lifecycle (DP12), silence (DP16), self_checking (DP8)
// surface as plain Bool (no enum fan-out).
static void test_smoke10_bool_dps() {
    const auto* s = smoke10();
    const std::uint8_t kOn[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 12, 0x01, std::span<const std::uint8_t>(kOn, 1) },
        { 16, 0x01, std::span<const std::uint8_t>(kOn, 1) },
        { 8,  0x01, std::span<const std::uint8_t>(kOn, 1) },
    };
    const auto r = dispatch_dp(*s, std::span<const TuyaDpRecord>(recs, 3));
    assert(r.any_matched);
    const Value* lc = r.merged.find("lifecycle");
    assert(lc && lc->type == ValueType::Bool && lc->b == true);
    const Value* sl = r.merged.find("silence");
    assert(sl && sl->type == ValueType::Bool && sl->b == true);
    const Value* sc = r.merged.find("self_checking");
    assert(sc && sc->type == ValueType::Bool && sc->b == true);
}

int main() {
    test_manu_resolves_to_full_dp_def();
    test_bindings_are_tuya_not_phantom();
    test_heat10_exposes_declared();
    test_heat10_temp_divisor_10();
    test_heat10_system_mode_enum();
    test_heat10_bool_enums_and_battery();
    test_smoke10_exposes_declared();
    test_smoke10_enum_dps();
    test_smoke10_numeric_raw();
    test_smoke10_bool_dps();
    return 0;
}
