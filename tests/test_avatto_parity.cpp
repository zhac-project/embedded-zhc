// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Avatto family (Tuya-OEM TS0601 datapoint devices plus
// one TS0011 on/off module). Found by a z2m<->embedded-zhc parity pass over the
// 11 existing avatto defs. The thermostats/TRVs, energy meters, the temp/humid
// sensor, the boiler switch and the on/off module were all at parity; three
// real decode bugs were fixed, all in the auto-generated DP tables:
//
//   1. ME168_AVATTO (_TZE200_4aijvczq): DP47 local_temperature_calibration
//      carried divisor=100, but z2m wires `localTempCalibration2`, whose
//      decode is identity (from: (v) => v) — the wire value is already in
//      whole degrees. The /100 turned a +3 C offset into 0.03 C.
//      (TRV26/TRV60 use `localTempCalibration1` = v/10 -> divisor=10, correct.)
//   2. ZDMS16-1 (_TZE204_5cuocqty): DP4 switch_type used the generic
//      `switchType` enum order {momentary:0, toggle:1, state:2}. z2m wires
//      `switchType2` = {toggle:0, state:1, momentary:2}, so every reading was
//      mislabelled.
//   3. ZDMS16-2 (_TZE204_o9gyszw2): same bug on BOTH channels — DP4 (l1) and
//      DP10 (l2) switch_type enums used the generic order; z2m uses
//      `switchType2`.
//
// The sibling ZDMS16-US-W2 (_TZE204_sdykkwsu) already used the correct
// `switchType2` order (z2m inline {toggle:0, state:1, momentary:2}) and is
// pinned here as a regression guard so a future regen does not "harmonise" the
// three dimmers back onto the wrong generic order.
//
// These tests decode real 0xEF00 (manuSpecificTuya) DP frames through the
// dispatcher and assert the corrected calibration scale and switch_type labels.
//
// z2m-source: zigbee-herdsman-converters/src/devices/avatto.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/avatto_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> avatto_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::avatto::kAvattoRegistry,
        devices::avatto::kAvattoRegistryCount);
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

const PreparedDefinition& resolve(const char* manu, const char* model) {
    const auto* d = find_definition("TS0601", manu, avatto_registry());
    assert(d && "avatto manufacturer name must resolve to a def");
    assert(d->model && std::strcmp(d->model, model) == 0);
    return *d;
}

// Decode a single Enum DP (dp_type 0x04, 1 wire byte) and return the StringRef
// label emitted under out_key (nullptr if absent / not a string).
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

// --- Bug 1: ME168_AVATTO DP47 calibration is identity (divisor=1) -----------
// A +3 C offset on the wire (Value 0x00000003) must decode to 3, NOT 0.03.
// With the old divisor=100 the value surfaced as Float 0.03.
static void test_me168_calibration_is_whole_degrees() {
    const auto& d = resolve("_TZE200_4aijvczq", "ME168_AVATTO");

    const std::uint8_t kPos3[] = { 0x00, 0x00, 0x00, 0x03 };
    const TuyaDpRecord recs[] = {
        { 47, 0x02, std::span<const std::uint8_t>(kPos3, 4) },
    };
    const auto result = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* cal = result.merged.find("local_temperature_calibration");
    assert(cal && "DP47 must surface local_temperature_calibration");
    const double cv = (cal->type == ValueType::Int) ? double(cal->i) : cal->f;
    // Identity decode: 3 wire -> 3 degrees. Old /100 bug would give 0.03.
    assert(cv > 2.5 && cv < 3.5);
}

// Negative calibration round-trips through the signed Int path: -2 C on the
// wire (0xFFFFFFFE) -> -2, not -0.02.
static void test_me168_calibration_negative() {
    const auto& d = resolve("_TZE200_4aijvczq", "ME168_AVATTO");

    const std::uint8_t kNeg2[] = { 0xFF, 0xFF, 0xFF, 0xFE };
    const TuyaDpRecord recs[] = {
        { 47, 0x02, std::span<const std::uint8_t>(kNeg2, 4) },
    };
    const auto result = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* cal = result.merged.find("local_temperature_calibration");
    assert(cal);
    const double cv = (cal->type == ValueType::Int) ? double(cal->i) : cal->f;
    assert(cv > -2.5 && cv < -1.5);
}

// --- Bug 2: ZDMS16-1 switch_type follows z2m switchType2 --------------------
// switchType2 = {toggle:0, state:1, momentary:2}.
static void test_zdms16_1_switch_type_decode() {
    const auto& d = resolve("_TZE204_5cuocqty", "ZDMS16-1");

    const char* s0 = decode_enum(d, 4, 0, "switch_type");
    assert(s0 && std::strcmp(s0, "toggle") == 0);
    const char* s1 = decode_enum(d, 4, 1, "switch_type");
    assert(s1 && std::strcmp(s1, "state") == 0);
    const char* s2 = decode_enum(d, 4, 2, "switch_type");
    assert(s2 && std::strcmp(s2, "momentary") == 0);
}

// --- Bug 3: ZDMS16-2 switch_type on BOTH channels follows switchType2 -------
static void test_zdms16_2_switch_type_decode_both_channels() {
    const auto& d = resolve("_TZE204_o9gyszw2", "ZDMS16-2");

    // l1 / DP4
    const char* l1_0 = decode_enum(d, 4, 0, "switch_type_l1");
    assert(l1_0 && std::strcmp(l1_0, "toggle") == 0);
    const char* l1_2 = decode_enum(d, 4, 2, "switch_type_l1");
    assert(l1_2 && std::strcmp(l1_2, "momentary") == 0);

    // l2 / DP10
    const char* l2_0 = decode_enum(d, 10, 0, "switch_type_l2");
    assert(l2_0 && std::strcmp(l2_0, "toggle") == 0);
    const char* l2_1 = decode_enum(d, 10, 1, "switch_type_l2");
    assert(l2_1 && std::strcmp(l2_1, "state") == 0);
}

// --- Regression guard: ZDMS16-US-W2 was already correct ---------------------
// Its z2m inline lookup {toggle:0, state:1, momentary:2} == switchType2, so it
// must stay {0:toggle} and NOT be regressed onto the generic order.
static void test_zdms16_us_w2_switch_type_unchanged() {
    const auto& d = resolve("_TZE204_sdykkwsu", "ZDMS16-US-W2");

    const char* l1_0 = decode_enum(d, 4, 0, "switch_type_l1");
    assert(l1_0 && std::strcmp(l1_0, "toggle") == 0);
    const char* l2_2 = decode_enum(d, 10, 2, "switch_type_l2");
    assert(l2_2 && std::strcmp(l2_2, "momentary") == 0);
}

int main() {
    test_me168_calibration_is_whole_degrees();
    test_me168_calibration_negative();
    test_zdms16_1_switch_type_decode();
    test_zdms16_2_switch_type_decode_both_channels();
    test_zdms16_us_w2_switch_type_unchanged();
    return 0;
}
