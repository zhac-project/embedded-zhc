// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for FrankEver FK_V02 — a TS0601 Tuya-MCU "smart water valve".
//
// The generator misrouted this Tuya-DP device to bare genOnOff (cluster 0x0006)
// and dropped two of its three channels: it only knew the `state` expose and
// wired kFzOnOff, which never matches a TuyaDp-family frame. z2m decodes the
// whole device over the 0xEF00 DP stream via `legacy.fz.frankever_valve`:
//
//   DP 1   -> state     Bool     ON/OFF
//   DP 101 -> threshold Numeric  raw value
//   DP 9   -> timer     Numeric  value / 60  (seconds -> minutes)
//
// These tests resolve the def and decode real 0xEF00 (manuSpecificTuya) DP
// frames, asserting all three keys surface with the right values and scale, and
// that the device binds 0xEF00 (not genOnOff).
//
// z2m-source: frankever.ts #FK_V02 + lib/legacy.ts fz.frankever_valve.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/frankever_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> frankever_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::frankever::kFrankeverRegistry,
        devices::frankever::kFrankeverRegistryCount);
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
    const auto* d = find_definition("TS0601", manu, frankever_registry());
    assert(d && "frankever manufacturer name must resolve to a def");
    return *d;
}

// Build a 4-byte big-endian Numeric DP record (dp_type 0x02).
struct NumDp {
    std::uint8_t bytes[4];
};
NumDp be32(std::uint32_t v) {
    return NumDp{ { static_cast<std::uint8_t>((v >> 24) & 0xFF),
                    static_cast<std::uint8_t>((v >> 16) & 0xFF),
                    static_cast<std::uint8_t>((v >> 8) & 0xFF),
                    static_cast<std::uint8_t>(v & 0xFF) } };
}

}  // namespace

// --- All three manufacturer names resolve to the single def. ---------------
static void test_all_mfr_names_resolve() {
    const char* names[] = { "_TZE200_wt9agwf3", "_TZE200_5uodvhgc", "_TZE200_1n2zev06" };
    const PreparedDefinition* first = &resolve(names[0]);
    for (const char* n : names) {
        const auto& d = resolve(n);
        assert(d.model && std::strcmp(d.model, "FK_V02") == 0);
        assert(&d == first && "all names share one def");
    }
    assert(std::strcmp(first->vendor, "FrankEver") == 0);
}

// --- The device binds the Tuya DP cluster (0xEF00), not genOnOff (0x0006). --
static void test_binds_tuya_cluster_not_genonoff() {
    const auto& d = resolve("_TZE200_wt9agwf3");
    assert(d.bindings && d.bindings_count == 1);
    assert(d.bindings[0].cluster_id == 0xEF00 &&
           "must bind manuSpecificTuya, not genOnOff");
}

// --- DP1 (Bool) -> state, routed through the Tuya DP map. -------------------
// The misrouted dup wired kFzOnOff (cluster 0x0006) which would NOT match a
// TuyaDp-family frame, so any_matched would be false.
static void test_state_dp_routes_through_tuya_map() {
    const auto& d = resolve("_TZE200_5uodvhgc");
    const std::uint8_t on[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 1, 0x01, std::span<const std::uint8_t>(on, 1) },
    };
    const auto result = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched && "DP1 must route through the Tuya DP map");
    const Value* v = result.merged.find("state");
    assert(v && v->type == ValueType::Bool && v->b == true);
}

// --- DP101 (Numeric, divisor 1) -> threshold, raw pass-through. -------------
static void test_threshold_dp_raw() {
    const auto& d = resolve("_TZE200_1n2zev06");
    const NumDp w = be32(50);
    const TuyaDpRecord recs[] = {
        { 101, 0x02, std::span<const std::uint8_t>(w.bytes, 4) },
    };
    const auto result = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find("threshold");
    assert(v && "DP101 must surface threshold");
    // divisor 1 -> integer pass-through (Uint/Int), value unchanged.
    if (v->type == ValueType::Uint)       assert(v->u == 50);
    else if (v->type == ValueType::Int)   assert(v->i == 50);
    else                                  assert(false && "threshold must be integer");
}

// --- DP9 (Numeric, divisor 60) -> timer, seconds -> minutes. ----------------
// z2m: {timer: value / 60}. 300 s -> 5 min.
static void test_timer_dp_scaled_seconds_to_minutes() {
    const auto& d = resolve("_TZE200_wt9agwf3");
    const NumDp w = be32(300);
    const TuyaDpRecord recs[] = {
        { 9, 0x02, std::span<const std::uint8_t>(w.bytes, 4) },
    };
    const auto result = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find("timer");
    assert(v && v->type == ValueType::Float && "timer scaled -> Float");
    const float diff = v->f - 5.0f;
    assert(diff > -0.001f && diff < 0.001f && "300 s / 60 == 5 min");
}

// --- Exposes carry all three StateSet keys (state/threshold/timer). ---------
static void test_exposes_complete() {
    const auto& d = resolve("_TZE200_wt9agwf3");
    bool has_state = false, has_threshold = false, has_timer = false;
    for (std::size_t i = 0; i < d.exposes_count; ++i) {
        const char* n = d.exposes[i].name;
        if (std::strcmp(n, "state") == 0)     has_state = true;
        if (std::strcmp(n, "threshold") == 0) has_threshold = true;
        if (std::strcmp(n, "timer") == 0)     has_timer = true;
        // every channel is writable in z2m (STATE_SET).
        assert(d.exposes[i].access == Access::StateSet);
    }
    assert(has_state && has_threshold && has_timer &&
           "state + threshold + timer must all be exposed");
}

int main() {
    test_all_mfr_names_resolve();
    test_binds_tuya_cluster_not_genonoff();
    test_state_dp_routes_through_tuya_map();
    test_threshold_dp_raw();
    test_timer_dp_scaled_seconds_to_minutes();
    test_exposes_complete();
    return 0;
}
