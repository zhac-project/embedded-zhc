// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Saswell (z2m<->embedded-zhc parity pass). Saswell has a
// single z2m entry (SEA801-Zigbee/SEA802-Zigbee, a Tuya-MCU thermostatic
// radiator valve) that decodes every datapoint via
// `legacy.fz.saswell_thermostat` on cluster manuSpecificTuya (0xEF00).
//
// Bug found + fixed (gap class d — MISROUTED Tuya-DP):
//   Two ez defs shared the model "SEA801-Zigbee/SEA802-Zigbee". The good
//   one (kDef_SEA801) wired the full 0xEF00 DP map. The sibling generated
//   def (Sas_SEA801_Zigbee_SEA802_Zigbee.cpp) was the ONLY def carrying the
//   legacy `_TYST11_*` fingerprints (GbxAXL2 / uhszj9s / 88teujp / w7cahqs /
//   aj4jz0i), and it wired the GENERIC genThermostat (0x0201) decoder. A
//   Tuya-MCU TRV never speaks genThermostat, so those legacy variants
//   decoded NOTHING (dead def). The fix folds the `_TYST11_*` fingerprints
//   into kDef_SEA801 (routing them through the correct DP map) and removes
//   the broken duplicate + its registry reference.
//
// These tests resolve the legacy fingerprints and assert they (a) land on
// the Tuya-DP def and (b) decode real 0xEF00 DP frames — proving the
// genThermostat dead-end is gone. The TS0601/_TZE200 path + per-DP scaling
// (setpoint and local_temperature /10, calibration identity) are pinned as
// regression guards.
//
// z2m-source: zigbee-herdsman-converters/src/devices/saswell.ts
//             + lib/legacy.ts `fz.saswell_thermostat`.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/saswell_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> saswell_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::saswell::kSaswellRegistry,
        devices::saswell::kSaswellRegistryCount);
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

const PreparedDefinition& resolve(const char* model, const char* manu) {
    const auto* d = find_definition(model, manu, saswell_registry());
    assert(d && "saswell fingerprint must resolve to a def");
    assert(d->model &&
           std::strcmp(d->model, "SEA801-Zigbee/SEA802-Zigbee") == 0);
    return *d;
}

// Decode a 4-byte big-endian Numeric DP and return its value as a double
// (Int or Float). Returns NAN-ish sentinel via found flag.
double decode_numeric(const PreparedDefinition& def, std::uint8_t dp,
                      std::int32_t wire_be, const char* out_key, bool& found) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((wire_be >> 24) & 0xFF),
        static_cast<std::uint8_t>((wire_be >> 16) & 0xFF),
        static_cast<std::uint8_t>((wire_be >>  8) & 0xFF),
        static_cast<std::uint8_t>( wire_be        & 0xFF),
    };
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(b, 4) },
    };
    const auto r = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    const Value* v = r.merged.find(out_key);
    found = (v != nullptr);
    if (!v) return 0.0;
    return (v->type == ValueType::Int) ? double(v->i) : double(v->f);
}

// Decode a 1-byte Bool DP and return the emitted boolean (found=false if
// the key never surfaced).
bool decode_bool(const PreparedDefinition& def, std::uint8_t dp,
                 std::uint8_t wire, const char* out_key, bool& found) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x01, std::span<const std::uint8_t>(b, 1) },
    };
    const auto r = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    const Value* v = r.merged.find(out_key);
    found = (v != nullptr && v->type == ValueType::Bool);
    return (v && v->type == ValueType::Bool) ? v->b : false;
}

}  // namespace

// --- The core fix: legacy _TYST11_* variants resolve to the Tuya-DP def ----
// Before the fix these matched only the genThermostat (0x0201) sibling and
// decoded nothing. Each legacy modelID + its manufacturer name must now land
// on kDef_SEA801 AND decode a live 0xEF00 DP frame.
static void test_legacy_tyst11_variants_decode_dp_stream() {
    struct Variant { const char* model; const char* manu; };
    const Variant variants[] = {
        { "GbxAXL2", "_TYST11_KGbxAXL2" },
        { "uhszj9s", "_TYST11_zuhszj9s" },
        { "88teujp", "_TYST11_c88teujp" },
        { "w7cahqs", "_TYST11_yw7cahqs" },
        { "aj4jz0i", "_TYST11_caj4jz0i" },
    };
    for (const auto& v : variants) {
        const auto& d = resolve(v.model, v.manu);

        // The def must NOT be wired to the generic genThermostat converter.
        // It must decode the Tuya 0xEF00 DP stream. DP103 setpoint = 215/10.
        bool found = false;
        const double sp = decode_numeric(d, 103, 215,
                                         "current_heating_setpoint", found);
        assert(found && "legacy variant must decode the 0xEF00 DP stream");
        assert(sp > 21.0 && sp < 22.0);
    }
}

// --- DP103 current_heating_setpoint scales /10 (z2m value/10) ---------------
static void test_setpoint_scale_div10() {
    const auto& d = resolve("TS0601", "_TZE200_c88teujp");
    bool found = false;
    // 0.5 C step: 235 -> 23.5 C.
    const double sp = decode_numeric(d, 103, 235,
                                     "current_heating_setpoint", found);
    assert(found);
    assert(sp > 23.4 && sp < 23.6);
}

// --- DP102 local_temperature scales /10 (z2m value/10) ----------------------
static void test_local_temperature_scale_div10() {
    const auto& d = resolve("TS0601", "_TZE200_yw7cahqs");
    bool found = false;
    const double lt = decode_numeric(d, 102, 211, "local_temperature", found);
    assert(found);
    assert(lt > 21.0 && lt < 21.2);  // 211/10 = 21.1
}

// --- DP27 local_temperature_calibration: signed identity, no /10 -----------
// ez reinterprets the 4-byte value as a SIGNED int: 0xFFFFFFFD -> -3 (not
// -0.3, not 4294967293). This INTENTIONALLY DIVERGES from z2m's buggy
// `value>6 ? 0xffffffff-value : value`: z2m reads the DP UNSIGNED, so a -3
// setting (wire 0xFFFFFFFD = 4294967293) is >6 and z2m returns 0xffffffff -
// 4294967293 = +2 (a known one's-complement off-by-one). ez emits the correct
// -3. The +4 case matches z2m (4 is <=6 -> passthrough); only negatives differ.
static void test_calibration_identity_signed() {
    const auto& d = resolve("TS0601", "_TZE200_zuhszj9s");
    bool found = false;
    const double cneg = decode_numeric(d, 27, -3,
                                       "local_temperature_calibration", found);
    assert(found);
    assert(cneg > -3.5 && cneg < -2.5);

    const double cpos = decode_numeric(d, 27, 4,
                                       "local_temperature_calibration", found);
    assert(found);
    assert(cpos > 3.5 && cpos < 4.5);
}

// --- Bool DPs surface their semantic keys -----------------------------------
// child_lock (40), away_mode (106), battery_low (105), system_mode (101).
static void test_bool_dps_surface_keys() {
    const auto& d = resolve("TS0601", "_TZE284_0dvm9mva");
    bool found = false;

    decode_bool(d, 40, 1, "child_lock", found);
    assert(found && "DP40 must surface child_lock");

    decode_bool(d, 106, 1, "away_mode", found);
    assert(found && "DP106 must surface away_mode");

    decode_bool(d, 105, 1, "battery_low", found);
    assert(found && "DP105 must surface battery_low");

    decode_bool(d, 101, 1, "system_mode", found);
    assert(found && "DP101 must surface system_mode");

    decode_bool(d, 3, 1, "heating", found);
    assert(found && "DP3 must surface heating");
}

// --- Registry de-dup guard --------------------------------------------------
// The broken genThermostat duplicate is gone: only one def remains, and every
// fingerprint (TS0601 and legacy) resolves to it.
static void test_single_def_no_dead_duplicate() {
    assert(devices::saswell::kSaswellRegistryCount == 1 &&
           "the dead genThermostat duplicate must be removed");
}

int main() {
    test_legacy_tyst11_variants_decode_dp_stream();
    test_setpoint_scale_div10();
    test_local_temperature_scale_div10();
    test_calibration_identity_signed();
    test_bool_dps_surface_keys();
    test_single_def_no_dead_duplicate();
    return 0;
}
