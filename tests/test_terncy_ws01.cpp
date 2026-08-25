// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the TERNCY WS01 wall-switch family (z2m v26.98.0):
//   TERNCY-WS01-D1..D4 (neutral) and -S1..S3 (no-neutral).
//
// These share the AduroSmart manufacturer cluster (0xFCCC) with the older
// TERNCY devices covered by test_terncy_parity.cpp, but carry a DIFFERENT
// frame shape, which is what these tests pin:
//
//   * a wider click table — 1..7 maps to single/double/triple/quadruple/
//     5_click/6_click/7_click, where the older terncy_raw decoder stopped
//     at 4 (quadruple);
//   * a second command id, 0x29, carrying hold (body[0]==0x02) and release
//     (body[0]==0x08) plus a 16-bit little-endian `action_duration`;
//   * the endpoint rides in the action VALUE (`single_l2`), not the key —
//     `action` is on dispatch.cpp's kAlwaysGlobalKeys list, so the runtime
//     never suffixes it. The label is resolved from the def's endpoint_map.
//
// Offset mapping (the trap the older decoder fell into): z2m indexes the
// WHOLE frame, ZHC's raw_body starts after the 5-byte manufacturer-specific
// header [fc, mfg_lo, mfg_hi, tsn, cmd]. So z2m data[4] == msg.command_id,
// data[5..8] == raw_body[0..3].
//
// z2m-source: zigbee-herdsman-converters/src/devices/terncy.ts
//             fzLocal.ws01_action.

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::terncy {
extern const PreparedDefinition kDef_TERNCY_WS01_D1;
extern const PreparedDefinition kDef_TERNCY_WS01_D2;
extern const PreparedDefinition kDef_TERNCY_WS01_D4;
extern const PreparedDefinition kDef_TERNCY_WS01_S3;
}  // namespace zhc::devices::terncy

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = src_ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// [fc, mfg_lo, mfg_hi, tsn, cmd, body...]; fc 0x0D = cluster-specific +
// manufacturer-specific + server->client, the byte z2m sees as `13`.
std::vector<std::uint8_t> frame(std::uint8_t cmd, std::vector<std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x0D, 0x28, 0x12, 0x42, cmd};
    v.insert(v.end(), body.begin(), body.end());
    return v;
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}

std::uint64_t uint_of(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v) return ~0ULL;
    if (v->type == ValueType::Uint) return v->u;
    if (v->type == ValueType::Int)  return static_cast<std::uint64_t>(v->i);
    return ~0ULL;
}

constexpr std::uint16_t kAduro = 0xFCCC;

void test_click_table() {
    std::printf("click table (1..7), endpoint in the value\n");
    const auto& d4 = ::zhc::devices::terncy::kDef_TERNCY_WS01_D4;

    struct Case { std::uint8_t code; const char* label; };
    constexpr Case kCases[] = {
        {1, "single"}, {2, "double"}, {3, "triple"}, {4, "quadruple"},
        {5, "5_click"}, {6, "6_click"}, {7, "7_click"},
    };
    for (const auto& c : kCases) {
        // body[0] is don't-care (z2m never reads it on the click path),
        // body[1] is the click code -> z2m's data[6].
        auto f = frame(0x00, {0x08, c.code});
        auto r = dispatch(d4, kAduro, 1, f);
        char want[32];
        std::snprintf(want, sizeof(want), "%s_l1", c.label);
        check(action_is(r, want), want);
    }

    // Endpoint 3 on a 4-gang -> l3, proving the label comes from endpoint_map
    // rather than being hardcoded.
    auto r3 = dispatch(d4, kAduro, 3, frame(0x00, {0x08, 2}));
    check(action_is(r3, "double_l3"), "double_l3 from endpoint 3");

    // Click code 0 and 8 are outside z2m's table -> nothing emitted.
    check(!has_key(dispatch(d4, kAduro, 1, frame(0x00, {0x08, 0})), "action"),
          "click code 0 rejected");
    check(!has_key(dispatch(d4, kAduro, 1, frame(0x00, {0x08, 8})), "action"),
          "click code 8 rejected");
}

void test_hold_release_duration() {
    std::printf("hold/release + action_duration (cmd 0x29)\n");
    const auto& d2 = ::zhc::devices::terncy::kDef_TERNCY_WS01_D2;

    // body = [kind, dontcare, dur_lo, dur_hi]; z2m reads data[5], data[7],
    // data[8] -> raw_body[0], [2], [3].
    auto hold = dispatch(d2, kAduro, 2, frame(0x29, {0x02, 0x00, 0x2C, 0x01}));
    check(action_is(hold, "hold_l2"), "hold_l2");
    check(uint_of(hold, "action_duration") == 0x012C, "duration 300 (LE)");

    auto rel = dispatch(d2, kAduro, 1, frame(0x29, {0x08, 0x00, 0xFF, 0xFF}));
    check(action_is(rel, "release_l1"), "release_l1");
    check(uint_of(rel, "action_duration") == 0xFFFF, "duration 65535");

    // A kind byte that is neither 0x02 nor 0x08 is ignored.
    check(!has_key(dispatch(d2, kAduro, 1, frame(0x29, {0x05, 0, 1, 0})), "action"),
          "unknown hold/release kind rejected");

    // Short body (z2m requires data.length >= 9) must not read past the end.
    check(!has_key(dispatch(d2, kAduro, 1, frame(0x29, {0x02, 0x00})), "action"),
          "truncated hold frame rejected");
}

void test_endpoint_bounds() {
    std::printf("endpoint bounds\n");
    // A 1-gang switch only maps endpoint 1; a frame from endpoint 2 has no
    // label and must be dropped rather than mislabelled or read out of range.
    const auto& d1 = ::zhc::devices::terncy::kDef_TERNCY_WS01_D1;
    check(action_is(dispatch(d1, kAduro, 1, frame(0x00, {0x08, 1})), "single_l1"),
          "D1 endpoint 1 decodes");
    check(!has_key(dispatch(d1, kAduro, 2, frame(0x00, {0x08, 1})), "action"),
          "D1 endpoint 2 dropped");

    // S3 maps three gangs; endpoint 4 is off the end.
    const auto& s3 = ::zhc::devices::terncy::kDef_TERNCY_WS01_S3;
    check(action_is(dispatch(s3, kAduro, 3, frame(0x00, {0x08, 1})), "single_l3"),
          "S3 endpoint 3 decodes");
    check(!has_key(dispatch(s3, kAduro, 4, frame(0x00, {0x08, 1})), "action"),
          "S3 endpoint 4 dropped");
}

void test_relay_and_exposes() {
    std::printf("relays and declared exposes\n");
    const auto& d2 = ::zhc::devices::terncy::kDef_TERNCY_WS01_D2;

    // genOnOff attribute report (attr 0x0000, bool) on endpoint 2 -> state_l2.
    std::vector<std::uint8_t> on{0x18, 0x42, 0x0A, 0x00, 0x00, 0x10, 0x01};
    auto r = dispatch(d2, 0x0006, 2, on);
    check(has_key(r, "state_l2"), "state_l2 from endpoint 2");
    check(!has_key(r, "state_l1"), "endpoint 2 does not emit state_l1");

    auto exposes_has = [](const PreparedDefinition& def, const char* key) {
        for (std::size_t i = 0; i < def.exposes_count; ++i)
            if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
        return false;
    };
    check(exposes_has(d2, "state_l1") && exposes_has(d2, "state_l2"),
          "D2 declares both gangs");
    check(!exposes_has(d2, "state_l3"), "D2 declares no third gang");
    check(exposes_has(d2, "action") && exposes_has(d2, "action_duration"),
          "D2 declares the action surface");

    const auto& d4 = ::zhc::devices::terncy::kDef_TERNCY_WS01_D4;
    check(exposes_has(d4, "state_l4"), "D4 declares four gangs");
}

}  // namespace

int main() {
    std::printf("== TERNCY WS01 family parity ==\n");
    test_click_table();
    test_hold_release_duration();
    test_endpoint_bounds();
    test_relay_and_exposes();
    if (g_failures) {
        std::printf("FAILED: %d check(s)\n", g_failures);
        return 1;
    }
    std::printf("all TERNCY WS01 checks passed\n");
    return 0;
}
