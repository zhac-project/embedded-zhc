// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the TECH Controllers (Polish) VNTH-T2 smart radiator
// valves — pure Tuya-datapoint TRVs reporting on cluster 0xEF00
// (manuSpecificTuya) cmd 0x02.
//
//   VNTH-T2_v1  TS0601 / _TZE204_r7brscr6
//   VNTH-T2_v2  TS0601 / _TZE204_p1qrtljn  (adds DP 39 scale_protection)
//
// Bug fixed: z2m maps DP 101 to BOTH `state` (valueConverter.onOff) AND
// `system_mode` (valueConverterBasic.lookup({heat: true, off: false})). The
// auto-generated DP extract kept only the `state` row, so the heat/off
// system_mode channel was never decoded and was absent from exposes. A
// second DP-101 row now emits system_mode via the new kTuyaDpFlagBoolEnum
// (a boolean DP fanned to a string label). The multi-row same-DP fan-out in
// fz_tuya_datapoints fires both rows from one DP-101 frame.
//
// These tests pin (a) both manufacturer names resolve through find_definition
// to their full DP def (never the __NEUTERED__ shadow stubs), (b) DP 101
// decodes to BOTH state and system_mode, (c) the core thermostat datapoints
// (setpoint /10, signed local-temp calibration) decode on the real wire.
//
// z2m-source: tech.ts #VNTH-T2_v1 / #VNTH-T2_v2.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/tech_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> tech_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::tech::kTechRegistry, devices::tech::kTechRegistryCount);
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

struct ManuModel {
    const char* manu;
    const char* model;
};
constexpr ManuModel kTechManus[] = {
    { "_TZE204_r7brscr6", "VNTH-T2_v1" },
    { "_TZE204_p1qrtljn", "VNTH-T2_v2" },
};

}  // namespace

// Both manufacturer names resolve to their full DP def (correct model, real
// datapoint-bearing surface), never the __NEUTERED__ shadow stub.
static void test_both_manus_resolve_to_full_dp_def() {
    for (const auto& mm : kTechManus) {
        const auto* d = find_definition("TS0601", mm.manu, tech_registry());
        assert(d && "TECH manufacturer name must resolve to a def");
        assert(d->model && std::strcmp(d->model, mm.model) == 0);
        // Full DP defs wire a Tuya DP from_zigbee + to_zigbee converter; the
        // neutered stubs carry neither and match no fingerprint.
        assert(d->from_zigbee && d->from_zigbee_count >= 1);
        assert(d->to_zigbee && d->to_zigbee_count >= 1);
        // The core climate surface a battery-only stub never carried.
        assert(def_exposes(*d, "current_heating_setpoint"));
        assert(def_exposes(*d, "local_temperature"));
    }
}

// The fix: both defs declare the previously-dropped system_mode expose.
static void test_both_defs_declare_system_mode_expose() {
    for (const auto& mm : kTechManus) {
        const auto* d = find_definition("TS0601", mm.manu, tech_registry());
        assert(d);
        assert(def_exposes(*d, "system_mode") &&
               "system_mode channel must be exposed");
        // state stays exposed alongside it (both fan from DP 101).
        assert(def_exposes(*d, "state"));
    }
}

// Core fix decode: DP 101 = true fans out to BOTH state=true AND
// system_mode="heat". Before the fix only state surfaced.
static void test_dp101_fans_state_and_system_mode_heat() {
    for (const auto& mm : kTechManus) {
        const auto* d = find_definition("TS0601", mm.manu, tech_registry());
        assert(d);
        const std::uint8_t kBoolOn[] = { 0x01 };
        const TuyaDpRecord recs[] = {
            { 101, 0x01, std::span<const std::uint8_t>(kBoolOn, 1) },
        };
        const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
        assert(result.any_matched);

        const Value* st = result.merged.find("state");
        assert(st && st->type == ValueType::Bool && st->b == true);

        const Value* mode = result.merged.find("system_mode");
        assert(mode && mode->type == ValueType::StringRef);
        assert(mode->str && std::strcmp(mode->str, "heat") == 0);
    }
}

// DP 101 = false → state=false AND system_mode="off".
static void test_dp101_fans_state_and_system_mode_off() {
    for (const auto& mm : kTechManus) {
        const auto* d = find_definition("TS0601", mm.manu, tech_registry());
        assert(d);
        const std::uint8_t kBoolOff[] = { 0x00 };
        const TuyaDpRecord recs[] = {
            { 101, 0x01, std::span<const std::uint8_t>(kBoolOff, 1) },
        };
        const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
        assert(result.any_matched);

        const Value* st = result.merged.find("state");
        assert(st && st->type == ValueType::Bool && st->b == false);

        const Value* mode = result.merged.find("system_mode");
        assert(mode && mode->type == ValueType::StringRef);
        assert(mode->str && std::strcmp(mode->str, "off") == 0);
    }
}

// Core thermostat datapoints decode on the real wire: setpoint (DP 4, /10)
// and signed local-temperature calibration (DP 47, /10 with int32 sign).
// z2m localTempCalibration1 sign-corrects an unsigned read; ez decodes the
// 4-byte big-endian value as a signed int32, so a negative calibration
// arrives directly (no >55 wraparound branch needed).
static void test_setpoint_and_signed_calibration_decode() {
    const auto* d = find_definition("TS0601", "_TZE204_r7brscr6", tech_registry());
    assert(d);

    // 215 = 0x000000D7 → 21.5 °C setpoint.
    const std::uint8_t kSp215[] = { 0x00, 0x00, 0x00, 0xD7 };
    // -10 as signed big-endian int32 = 0xFFFFFFF6 → /10 → -1.0 °C calibration.
    const std::uint8_t kCalNeg10[] = { 0xFF, 0xFF, 0xFF, 0xF6 };
    const TuyaDpRecord recs[] = {
        { 4,  0x02, std::span<const std::uint8_t>(kSp215, 4) },
        { 47, 0x02, std::span<const std::uint8_t>(kCalNeg10, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Float);
    assert(sp->f > 21.49f && sp->f < 21.51f);

    const Value* cal = result.merged.find("local_temperature_calibration");
    assert(cal && cal->type == ValueType::Float);
    assert(cal->f > -1.01f && cal->f < -0.99f);
}

int main() {
    test_both_manus_resolve_to_full_dp_def();
    test_both_defs_declare_system_mode_expose();
    test_dp101_fans_state_and_system_mode_heat();
    test_dp101_fans_state_and_system_mode_off();
    test_setpoint_and_signed_calibration_decode();
    return 0;
}
