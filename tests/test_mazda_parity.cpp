// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for MAZDA (single device: TR-M2Z thermostatic radiator valve,
// a Tuya-OEM TS0601 0xEF00 datapoint TRV with two fingerprint variants
// _TZE204_k6rdmisz / _TZE284_k6rdmisz). A z2m<->embedded-zhc parity pass over
// the auto-port found two real decode bugs in the generated DP table, both on
// the same datapoint:
//
//   1. The DP map declared count=23 while the entry table holds 24 entries, so
//      DP47 local_temperature_calibration (the last row) was truncated off the
//      end of the map and never decoded at all.
//   2. Once included, DP47 carried divisor=1, but z2m wires
//      `localTempCalibration3` whose decode is v/10 (the wire is a signed s32
//      in tenths of a degree; decode_tuya_dp already sign-reinterprets the
//      big-endian 4-byte value). divisor=10 restores the scale — a +30 wire
//      value is +3.0 C, not +30 C.
//
// These tests decode real 0xEF00 (manuSpecificTuya) DP frames through the
// dispatcher and assert the calibration scale on BOTH fingerprint variants,
// plus a setpoint regression guard that the rest of the (now complete) map
// still decodes.
//
// z2m-source: zigbee-herdsman-converters/src/devices/mazda.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/mazda_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> mazda_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::mazda::kMazdaRegistry,
        devices::mazda::kMazdaRegistryCount);
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

const PreparedDefinition& resolve(const char* manu) {
    const auto* d = find_definition("TS0601", manu, mazda_registry());
    assert(d && "mazda manufacturer name must resolve to a def");
    assert(d->model && std::strcmp(d->model, "TR-M2Z") == 0);
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
    assert(v && "expected numeric DP to surface its key");
    return (v->type == ValueType::Int) ? double(v->i) : double(v->f);
}

// Bug 1+2: DP47 must now decode (was truncated by count=23) and at /10 scale
// (was divisor=1). +30 wire -> +3.0 C.
void test_calibration_positive(const char* manu) {
    const auto& d = resolve(manu);
    const std::uint8_t kPos30[] = { 0x00, 0x00, 0x00, 0x1E };  // 30
    const double cv = decode_numeric(d, 47, kPos30, "local_temperature_calibration");
    assert(cv > 2.9 && cv < 3.1);  // 30/10 = 3.0, not 30
}

// Negative calibration round-trips through the signed Int path: -25 wire
// (0xFFFFFFE7) -> -2.5 C.
void test_calibration_negative(const char* manu) {
    const auto& d = resolve(manu);
    const std::uint8_t kNeg25[] = { 0xFF, 0xFF, 0xFF, 0xE7 };  // -25
    const double cv = decode_numeric(d, 47, kNeg25, "local_temperature_calibration");
    assert(cv > -2.6 && cv < -2.4);  // -25/10 = -2.5
}

// Regression guard: the rest of the (now 24-entry) map still decodes — DP4
// current_heating_setpoint at /10. 215 wire -> 21.5 C.
void test_setpoint_still_decodes(const char* manu) {
    const auto& d = resolve(manu);
    const std::uint8_t kSp215[] = { 0x00, 0x00, 0x00, 0xD7 };  // 215
    const double sv = decode_numeric(d, 4, kSp215, "current_heating_setpoint");
    assert(sv > 21.4 && sv < 21.6);
}

}  // namespace

int main() {
    for (const char* manu : { "_TZE204_k6rdmisz", "_TZE284_k6rdmisz" }) {
        test_calibration_positive(manu);
        test_calibration_negative(manu);
        test_setpoint_still_decodes(manu);
    }
    return 0;
}
