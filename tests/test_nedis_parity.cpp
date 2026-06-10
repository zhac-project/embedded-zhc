// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Nedis family. There is exactly one z2m definition:
// ZBHTR20WT, a Tuya-OEM TS0601 datapoint TRV (thermostat radiator valve)
// fingerprinted on three manufacturer names (_TZE200_ne4pikwm,
// _TZE284_ne4pikwm, _TZE284_hcs66axl). embedded-zhc splits it into one def
// per manufacturer name; all three carry the same 0xEF00 DP map.
//
// A z2m<->embedded-zhc parity pass found ONE real decode bug, present in all
// three defs (auto-generated DP table):
//
//   DP27 local_temperature_calibration carried divisor=100, but z2m wires
//   `valueConverter.localTempCalibration2`, whose decode is identity
//   (from: (v) => v) — the wire value is already in whole degrees. The /100
//   turned a +3 C offset into 0.03 C. Corrected to divisor=1 (raw int
//   pass-through). The expose range is -6..6 step 1, confirming whole-degree
//   units. (Sibling TRV families wiring `localTempCalibration1` = v/10 keep
//   divisor=10 — not this device.)
//
// Pinned alongside: regression guards that local_temperature (DP102) and
// current_heating_setpoint (DP103) stay /10 (z2m divideBy10), and that the
// running_state enum {heat:1, idle:0} decodes — so a future regen does not
// "harmonise" the calibration scale back onto /100 or perturb the others.
//
// These tests decode real 0xEF00 (manuSpecificTuya) DP frames through the
// dispatcher and assert the corrected calibration scale.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nedis.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/nedis_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> nedis_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::nedis::kNedisRegistry,
        devices::nedis::kNedisRegistryCount);
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

// Resolve the def that wins find_definition for a given manufacturer name.
// All three nedis manufacturer names must reach a real DP-mapped def — never
// the battery+onOff stub (which loses Pass-1 ordering).
const PreparedDefinition& resolve(const char* manu) {
    const auto* d = find_definition("TS0601", manu, nedis_registry());
    assert(d && "nedis manufacturer name must resolve to a def");
    assert(d->to_zigbee_count > 0 &&
           "winning def must carry the Tuya DP tz map (not the onOff stub)");
    return *d;
}

double decode_numeric(const PreparedDefinition& def, std::uint8_t dp,
                      const std::uint8_t (&wire)[4], const char* out_key) {
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(wire, 4) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    assert(v && "expected key absent from decode");
    return (v->type == ValueType::Int) ? double(v->i) : v->f;
}

const char* decode_enum(const PreparedDefinition& def, std::uint8_t dp,
                        std::uint8_t wire, const char* out_key) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x04, std::span<const std::uint8_t>(b, 1) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

}  // namespace

// --- Bug fix: DP27 calibration is identity (divisor=1) ----------------------
// A +3 C offset on the wire (0x00000003) must decode to 3, NOT 0.03.
// Asserted on every one of the three nedis manufacturer names, since the bug
// was present in all three DP tables.
static void test_calibration_is_whole_degrees_all_variants() {
    const char* const manus[] = {
        "_TZE200_ne4pikwm", "_TZE284_ne4pikwm", "_TZE284_hcs66axl",
    };
    for (const char* manu : manus) {
        const auto& d = resolve(manu);

        const std::uint8_t kPos3[4] = { 0x00, 0x00, 0x00, 0x03 };
        const double pos =
            decode_numeric(d, 27, kPos3, "local_temperature_calibration");
        // Identity decode: 3 wire -> 3 degrees. Old /100 bug gave 0.03.
        assert(pos > 2.5 && pos < 3.5);

        // Negative offset round-trips through the signed Int path:
        // -2 C on the wire (0xFFFFFFFE) -> -2, not -0.02.
        const std::uint8_t kNeg2[4] = { 0xFF, 0xFF, 0xFF, 0xFE };
        const double neg =
            decode_numeric(d, 27, kNeg2, "local_temperature_calibration");
        assert(neg > -2.5 && neg < -1.5);
    }
}

// --- Regression guard: local_temperature (DP102) stays /10 ------------------
// z2m valueConverter.divideBy10: wire 215 -> 21.5 C.
static void test_local_temperature_divide_by_10() {
    const auto& d = resolve("_TZE200_ne4pikwm");
    const std::uint8_t k215[4] = { 0x00, 0x00, 0x00, 0xD7 };  // 215
    const double t = decode_numeric(d, 102, k215, "local_temperature");
    assert(t > 21.4 && t < 21.6);
}

// --- Regression guard: current_heating_setpoint (DP103) stays /10 -----------
// z2m valueConverter.divideBy10: wire 205 -> 20.5 C (NOT /2). 0.5 step is a
// UI constraint, the wire converter is divideBy10.
static void test_setpoint_divide_by_10() {
    const auto& d = resolve("_TZE284_hcs66axl");
    const std::uint8_t k205[4] = { 0x00, 0x00, 0x00, 0xCD };  // 205
    const double sp = decode_numeric(d, 103, k205, "current_heating_setpoint");
    assert(sp > 20.4 && sp < 20.6);
}

// --- Regression guard: running_state enum {heat:1, idle:0} ------------------
static void test_running_state_enum_decode() {
    const auto& d = resolve("_TZE284_ne4pikwm");
    const char* heat = decode_enum(d, 3, 1, "running_state");
    assert(heat && std::strcmp(heat, "heat") == 0);
    const char* idle = decode_enum(d, 3, 0, "running_state");
    assert(idle && std::strcmp(idle, "idle") == 0);
}

int main() {
    test_calibration_is_whole_degrees_all_variants();
    test_local_temperature_divide_by_10();
    test_setpoint_divide_by_10();
    test_running_state_enum_decode();
    return 0;
}
