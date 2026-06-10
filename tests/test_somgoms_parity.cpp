// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Somgoms family (Tuya-MCU TS0601 / legacy-DP switches,
// dimmers and curtain motors — ZSTY-SM-1*ZG). Found by a z2m<->embedded-zhc
// parity pass over the 4 existing somgoms defs.
//
// REAL BUG (all 4 defs): every device was auto-ported onto generic standard-
// cluster converters + bindings:
//   ZSTY-SM-11ZG-US-W  -> kFzOnOff/kTzOnOff + genOnOff (0x0006)
//   ZSTY-SM-1CTZG-US-W -> kFzCoverPosition + windowCovering (0x0102)
//   SM-1CTW-EU         -> kFzCoverPosition + windowCovering (0x0102)
//   ZSTY-SM-1DMZG-US-W -> kFzOnOff/kFzBrightness + genOnOff/genLevelCtrl
// But z2m wires these as Tuya-MCU devices speaking the 0xEF00
// (manuSpecificTuya) DP stream (legacy.fz.tuya_switch / tuya_cover /
// tuya_dimmer). The standard clusters are never used, so every port was DEAD:
// no state/position/brightness ever decoded, and writes went nowhere.
//
// FIX: re-wired all 4 onto the Tuya-DP read+write infra:
//   switch  : DP1 (state) bool         -> state
//   covers  : DP2/DP3 (position)       -> position
//   dimmer  : DP1 (state) + DP3 (level)-> state + brightness
//
// These tests decode real 0xEF00 DP frames through the dispatcher and assert
// the corrected keys are emitted under the right type.
//
// z2m-source: zigbee-herdsman-converters/src/devices/somgoms.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/somgoms_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> somgoms_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::somgoms::kSomgomsRegistry,
        devices::somgoms::kSomgomsRegistryCount);
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

const PreparedDefinition& resolve(const char* model_fp, const char* manu,
                                  const char* model) {
    const auto* d = find_definition(model_fp, manu, somgoms_registry());
    assert(d && "somgoms fingerprint must resolve to a def");
    assert(d->model && std::strcmp(d->model, model) == 0);
    return *d;
}

// Decode a single Bool DP (dp_type 0x01, 1 wire byte).
DispatchResult decode_bool(const PreparedDefinition& def, std::uint8_t dp,
                           std::uint8_t wire) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x01, std::span<const std::uint8_t>(b, 1) },
    };
    return dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
}

// Decode a single Numeric DP (dp_type 0x02, 4 wire bytes, big-endian).
DispatchResult decode_value(const PreparedDefinition& def, std::uint8_t dp,
                            std::int32_t v) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((v >> 24) & 0xFF),
        static_cast<std::uint8_t>((v >> 16) & 0xFF),
        static_cast<std::uint8_t>((v >> 8) & 0xFF),
        static_cast<std::uint8_t>(v & 0xFF),
    };
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(b, 4) },
    };
    return dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
}

double num(const Value* v) {
    if (!v) return -1e9;
    switch (v->type) {
        case ValueType::Int:   return double(v->i);
        case ValueType::Uint:  return double(v->u);
        case ValueType::Float: return double(v->f);
        default:               return -1e9;
    }
}

}  // namespace

// --- 1-gang switch: DP1 (state) bool decodes via the 0xEF00 DP stream -------
// Old port bound genOnOff and would never see this frame.
static void test_switch_dp1_state() {
    const auto& d = resolve("tdtqgwv", nullptr, "ZSTY-SM-11ZG-US-W");

    auto on = decode_bool(d, 1, 1);
    assert(on.any_matched);
    const Value* s_on = on.merged.find("state");
    assert(s_on && s_on->type == ValueType::Bool && s_on->b == true);

    auto off = decode_bool(d, 1, 0);
    assert(off.any_matched);
    const Value* s_off = off.merged.find("state");
    assert(s_off && s_off->type == ValueType::Bool && s_off->b == false);
}

// --- Curtain ZSTY-SM-1CTZG-US-W: DP2 + DP3 -> position ----------------------
static void test_cover_ctzg_position() {
    const auto& d = resolve("bordckq", nullptr, "ZSTY-SM-1CTZG-US-W");

    // DP2 coverPosition
    auto r2 = decode_value(d, 2, 60);
    assert(r2.any_matched);
    assert(num(r2.merged.find("position")) > 59.5 &&
           num(r2.merged.find("position")) < 60.5);

    // DP3 coverArrived (position echo) must ALSO surface position.
    auto r3 = decode_value(d, 3, 25);
    assert(r3.any_matched);
    assert(num(r3.merged.find("position")) > 24.5 &&
           num(r3.merged.find("position")) < 25.5);
}

// --- Curtain SM-1CTW-EU (TS0601 / _TZE200_sbordckq): DP2 -> position --------
static void test_cover_ctw_eu_position() {
    const auto& d = resolve("TS0601", "_TZE200_sbordckq", "SM-1CTW-EU");

    auto r2 = decode_value(d, 2, 100);
    assert(r2.any_matched);
    assert(num(r2.merged.find("position")) > 99.5 &&
           num(r2.merged.find("position")) < 100.5);
}

// --- Dimmer ZSTY-SM-1DMZG-US-W: DP1 -> state, DP3 -> brightness -------------
static void test_dimmer_state_and_brightness() {
    const auto& d = resolve("hpb9yts", nullptr, "ZSTY-SM-1DMZG-US-W");

    auto st = decode_bool(d, 1, 1);
    assert(st.any_matched);
    const Value* s = st.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);

    // DP3 dimmerLevel passes through raw (divisor 1).
    auto br = decode_value(d, 3, 500);
    assert(br.any_matched);
    assert(num(br.merged.find("brightness")) > 499.5 &&
           num(br.merged.find("brightness")) < 500.5);
}

int main() {
    test_switch_dp1_state();
    test_cover_ctzg_position();
    test_cover_ctw_eu_position();
    test_dimmer_state_and_brightness();
    return 0;
}
