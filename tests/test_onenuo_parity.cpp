// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Onenuo 288WZ smoke detector
// (TS0601 / _TZE204_kgaxpvxr + _TZE284_n4ttsck2) — a pure Tuya-datapoint
// smoke detector reporting on cluster 0xEF00 (manuSpecificTuya).
//
// Bug fixed: the auto-extracted DP defs kept only 3 of z2m's 5 datapoints
// (DP15 battery, DP16 silence, DP102 sensitivity) and DROPPED the two
// primary channels — DP1 (smoke + smoke_state, the actual alarm) and
// DP101 (self_test_result). They also carried phantom `state`/`action`
// binary exposes that the device never reports. A sibling descriptive
// stub (kDef_D288WZ) wired the detector to ssIasZone (0x0500) instead of
// the Tuya DP map, which is wrong for a TS0601 0xEF00 device (it is a
// harmless dead dup — it loses find_definition Pass 1 to these per-manu
// DP defs, so it is left in place).
//
// DP1 is an enum state {0:alarm,1:normal,2:detecting,3:unknown} that z2m
// fans into BOTH a string `smoke_state` and a boolean `smoke` (= state
// === "alarm"). The boolean fan-out uses the new kTuyaDpFlagEnumBool flag.
// DP101 is a boolean fanned to "failure"/"success" via kTuyaDpFlagBoolEnum
// (z2m lookup {failure:false, success:true}).
//
// These tests pin (a) both manufacturer names resolve through
// find_definition to the full DP def, (b) the smoke / smoke_state /
// self_test_result exposes are declared (and the phantom state/action are
// gone), and (c) every restored datapoint decodes on the real wire with
// z2m-matching semantics.
//
// z2m-source: devices/onenuo.ts #288WZ.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/onenuo_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> onenuo_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::onenuo::kOnenuoRegistry,
        devices::onenuo::kOnenuoRegistryCount);
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

const PreparedDefinition* onenuo_def(const char* manu) {
    const auto* d = find_definition("TS0601", manu, onenuo_registry());
    assert(d && "manufacturer must resolve to the Onenuo smoke detector def");
    return d;
}

}  // namespace

// Both manufacturer names resolve to a full DP def (Tuya from + to_zigbee),
// not the IAS / battery-only stub.
static void test_both_manus_resolve_to_full_dp_def() {
    for (const char* manu : { "_TZE204_kgaxpvxr", "_TZE284_n4ttsck2" }) {
        const auto* d = onenuo_def(manu);
        assert(d->vendor && std::strcmp(d->vendor, "Onenuo") == 0);
        assert(d->from_zigbee && d->from_zigbee_count >= 1);
        assert(d->to_zigbee && d->to_zigbee_count >= 1);
        // The detector must NOT bind ssIasZone (0x0500) — it is a 0xEF00
        // Tuya-DP device. Its only binding is the manuSpecificTuya cluster.
        for (std::size_t i = 0; i < d->bindings_count; ++i) {
            assert(d->bindings[i].cluster_id != 0x0500 &&
                   "smoke detector must not bind IAS Zone");
        }
    }
}

// The real exposes are declared, and the phantom state/action binaries
// (which the device never reports) are gone.
static void test_exposes_match_z2m() {
    for (const char* manu : { "_TZE204_kgaxpvxr", "_TZE284_n4ttsck2" }) {
        const auto* d = onenuo_def(manu);
        assert(def_exposes(*d, "smoke"));
        assert(def_exposes(*d, "smoke_state"));
        assert(def_exposes(*d, "self_test_result"));
        assert(def_exposes(*d, "battery"));
        assert(def_exposes(*d, "silence"));
        assert(def_exposes(*d, "sensitivity"));
        // Phantom generator exposes removed.
        assert(!def_exposes(*d, "state"));
        assert(!def_exposes(*d, "action"));
    }
}

// DP1 enum value 0 (alarm) fans to BOTH smoke=true and smoke_state="alarm".
static void test_dp1_alarm_fans_to_smoke_and_state() {
    for (const char* manu : { "_TZE204_kgaxpvxr", "_TZE284_n4ttsck2" }) {
        const auto* d = onenuo_def(manu);
        // Enum DP (dp_type 0x04), 1 byte = 0 -> alarm.
        const std::uint8_t kAlarm[] = { 0x00 };
        const TuyaDpRecord recs[] = {
            { 1, 0x04, std::span<const std::uint8_t>(kAlarm, 1) },
        };
        const auto r = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
        assert(r.any_matched);

        const Value* smoke = r.merged.find("smoke");
        assert(smoke && smoke->type == ValueType::Bool && smoke->b == true);

        const Value* state = r.merged.find("smoke_state");
        assert(state && state->type == ValueType::StringRef);
        assert(state->str && std::strcmp(state->str, "alarm") == 0);
    }
}

// DP1 enum value 1 (normal) -> smoke=false, smoke_state="normal".
static void test_dp1_normal_clears_smoke() {
    const auto* d = onenuo_def("_TZE204_kgaxpvxr");
    const std::uint8_t kNormal[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 1, 0x04, std::span<const std::uint8_t>(kNormal, 1) },
    };
    const auto r = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(r.any_matched);

    const Value* smoke = r.merged.find("smoke");
    assert(smoke && smoke->type == ValueType::Bool && smoke->b == false);

    const Value* state = r.merged.find("smoke_state");
    assert(state && state->type == ValueType::StringRef);
    assert(state->str && std::strcmp(state->str, "normal") == 0);

    // A non-alarm state (e.g. detecting) also clears smoke.
    const std::uint8_t kDetecting[] = { 0x02 };
    const TuyaDpRecord recs2[] = {
        { 1, 0x04, std::span<const std::uint8_t>(kDetecting, 1) },
    };
    const auto r2 = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs2, 1));
    const Value* smoke2 = r2.merged.find("smoke");
    assert(smoke2 && smoke2->type == ValueType::Bool && smoke2->b == false);
    const Value* state2 = r2.merged.find("smoke_state");
    assert(state2 && state2->str && std::strcmp(state2->str, "detecting") == 0);
}

// DP101 self_test_result boolean fans to "failure"/"success"
// (z2m lookup {failure:false, success:true}).
static void test_dp101_self_test_result() {
    const auto* d = onenuo_def("_TZE284_n4ttsck2");

    const std::uint8_t kOn[] = { 0x01 };
    const TuyaDpRecord recsOk[] = {
        { 101, 0x01, std::span<const std::uint8_t>(kOn, 1) },
    };
    const auto rOk = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOk, 1));
    assert(rOk.any_matched);
    const Value* ok = rOk.merged.find("self_test_result");
    assert(ok && ok->type == ValueType::StringRef);
    assert(ok->str && std::strcmp(ok->str, "success") == 0);

    const std::uint8_t kOff[] = { 0x00 };
    const TuyaDpRecord recsFail[] = {
        { 101, 0x01, std::span<const std::uint8_t>(kOff, 1) },
    };
    const auto rFail = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsFail, 1));
    const Value* fail = rFail.merged.find("self_test_result");
    assert(fail && fail->type == ValueType::StringRef);
    assert(fail->str && std::strcmp(fail->str, "failure") == 0);
}

// Pre-existing channels still decode: DP15 battery (raw), DP16 silence (raw),
// DP102 sensitivity enum.
static void test_battery_silence_sensitivity() {
    const auto* d = onenuo_def("_TZE204_kgaxpvxr");
    // 87 = 0x00000057 battery, 1 silence, enum 2 = high.
    const std::uint8_t kBat87[] = { 0x00, 0x00, 0x00, 0x57 };
    const std::uint8_t kSil1[]  = { 0x00, 0x00, 0x00, 0x01 };
    const std::uint8_t kHigh[]  = { 0x02 };
    const TuyaDpRecord recs[] = {
        { 15,  0x02, std::span<const std::uint8_t>(kBat87, 4) },
        { 16,  0x02, std::span<const std::uint8_t>(kSil1, 4) },
        { 102, 0x04, std::span<const std::uint8_t>(kHigh, 1) },
    };
    const auto r = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 3));
    assert(r.any_matched);

    const Value* bat = r.merged.find("battery");
    assert(bat && bat->type == ValueType::Int && bat->i == 87);

    const Value* sil = r.merged.find("silence");
    assert(sil && sil->type == ValueType::Int && sil->i == 1);

    const Value* sens = r.merged.find("sensitivity");
    assert(sens && sens->type == ValueType::StringRef);
    assert(sens->str && std::strcmp(sens->str, "high") == 0);
}

int main() {
    test_both_manus_resolve_to_full_dp_def();
    test_exposes_match_z2m();
    test_dp1_alarm_fans_to_smoke_and_state();
    test_dp1_normal_clears_smoke();
    test_dp101_self_test_result();
    test_battery_silence_sensitivity();
    return 0;
}
