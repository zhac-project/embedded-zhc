// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Fixture: Tuya TS0044 4-button wireless scene switch
// (Moes ESW-0ZAA-EU, HOBEIAN ZG-101ZS, LoraTap SS6400ZB, …).
//
// Captured wire frame from a real press (cmd 0xFD `commandTuyaAction`):
//   I (..) zhc_adapter: 0x...8352cf Tuya/TS0044 (no match)
//   cluster=0x0006(genOnOff) family=Zcl type=4 cmd=0xfd zcl[4]=01 0c fd 00
//
// Before the fix, kFzTuyaOnOffAction matched only cmd 0x00/0x01/0x02
// (off/on/toggle), so every press dropped silently. After: TS0044
// uses kFzTuyaButtonAction which decodes cmd 0xFD and emits
// `action = "<src_endpoint>_<single|double|hold>"` matching the z2m
// action enum format.

// Force assert() to remain enabled even when the rest of the tree is
// built with -DNDEBUG; without this Release builds elide every check
// and the test reports a false pass.
#undef NDEBUG
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/types.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0044;
}  // namespace zhc::devices::tuya

// Test-only hook from tuya/_shared.cpp — kept for source compatibility
// since H-2 moved dedup state onto `DeviceRuntimeState`. The reset call
// is now satisfied by zeroing the test's local RuntimeStore each test.
namespace zhc::tuya {
void reset_action_dedup_cache_for_test();
}

namespace {
// Shared per-test store so the dispatcher can find a DeviceRuntimeState
// for non-zero device_index (where the dedup cache lives).
zhc::RuntimeStore<128> g_test_store{};

// Zero every slot so each test case starts with a fresh dedup state.
// Replaces the old `reset_action_dedup_cache_for_test()` call (which
// is now a no-op because the cache lives per-device on the store).
void reset_test_dedup() {
    g_test_store = {};
}
}  // namespace

namespace {

InboundApsFrame make_frame(std::uint16_t cluster,
                            std::uint8_t   src_ep,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// commandTuyaAction wire format (4 bytes):
//   fc(1)=0x01 (cluster-specific, C→S, no manuf, default-response on)
//   tsn(1)     transaction seq
//   cmd(1)=0xFD
//   payload(1) 0=single, 1=double, 2=hold
struct ActionFrame {
    std::array<std::uint8_t, 4> bytes;
};

ActionFrame action_press(std::uint8_t click, std::uint8_t tsn = 0x0C) {
    return ActionFrame{ { 0x01, tsn, 0xFD, click } };
}

// `dev_idx == 0` keeps the dedup logic disabled (default for the
// pre-existing tests that synthesise repeated frames). Pass a nonzero
// index when exercising dedup behaviour.
DispatchResult dispatch_press(std::uint8_t src_ep, std::uint8_t click,
                                std::uint8_t  tsn     = 0x0C,
                                std::uint16_t dev_idx = 0) {
    auto f = action_press(click, tsn);
    auto raw = make_frame(0x0006, src_ep,
                          std::span<const std::uint8_t>(f.bytes.data(),
                                                          f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    ctx.device_index = dev_idx;
    // Wire the test store so DeviceRuntimeState (where H-2 moved dedup
    // state) is reachable. dev_idx == 0 callers still bypass dedup by
    // design, store wiring or not.
    ctx.store     = &g_test_store;
    ctx.store_get = &decltype(g_test_store)::get;
    return dispatch_from_zigbee(msg, {}, devices::tuya::kDefTS0044, raw, ctx);
}

void expect_action(const DispatchResult& r, const char* expected) {
    assert(r.any_matched);
    const Value* a = r.merged.find("action");
    assert(a && a->type == ValueType::StringRef);
    assert(std::strcmp(a->str, expected) == 0);
}

}  // namespace

// All twelve action variants (4 buttons × 3 click types). One assertion
// per cell — a future generator regression that emits e.g. "single"
// instead of "1_single" lights up here immediately.
static void test_all_button_click_combinations() {
    expect_action(dispatch_press(1, 0), "1_single");
    expect_action(dispatch_press(1, 1), "1_double");
    expect_action(dispatch_press(1, 2), "1_hold");

    expect_action(dispatch_press(2, 0), "2_single");
    expect_action(dispatch_press(2, 1), "2_double");
    expect_action(dispatch_press(2, 2), "2_hold");

    expect_action(dispatch_press(3, 0), "3_single");
    expect_action(dispatch_press(3, 1), "3_double");
    expect_action(dispatch_press(3, 2), "3_hold");

    expect_action(dispatch_press(4, 0), "4_single");
    expect_action(dispatch_press(4, 1), "4_double");
    expect_action(dispatch_press(4, 2), "4_hold");
}

// Exact byte-for-byte replay of the user's captured frame.
// `zcl[4] = 01 0c fd 00` with src_endpoint=1 ⇒ "1_single".
static void test_real_capture_replay() {
    const std::uint8_t kCaptured[] = { 0x01, 0x0c, 0xfd, 0x00 };
    auto raw = make_frame(0x0006, 1,
                          std::span<const std::uint8_t>(kCaptured, 4));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, devices::tuya::kDefTS0044,
                                   raw, ctx);
    assert(r.any_matched);
    const Value* a = r.merged.find("action");
    assert(a && a->type == ValueType::StringRef);
    assert(std::strcmp(a->str, "1_single") == 0);
}

// Unmapped payload byte (>= 3) must return no match. Catches a future
// edit that lazily indexes the label table out of bounds.
static void test_unknown_click_value_no_match() {
    auto r = dispatch_press(1, /*click=*/0x05);
    assert(!r.any_matched);
}

// src_endpoint outside [1..8] must return no match — table is 8 rows.
// Real TS0044 only uses 1..4 but the converter is reused by other
// scene switches with more buttons.
static void test_endpoint_out_of_range_no_match() {
    auto r = dispatch_press(/*ep=*/9, /*click=*/0);
    assert(!r.any_matched);
    r = dispatch_press(/*ep=*/0, /*click=*/0);
    assert(!r.any_matched);
}

// After P2 unified the action converters, kFzTuyaButtonAction also
// decodes cmd 0xFC `tuyaAction2` (rotary). A physical TS0044 has no
// wheel and will never send 0xFC, but the converter accepts it
// harmlessly. Lock the per-endpoint rotate label so a regression to
// "0xFD-only" gets caught early.
static void test_rotary_cmd_decodes_per_endpoint() {
    const std::uint8_t kFc[] = { 0x01, 0x0c, 0xFC, 0x00 };  // rotate_right
    auto raw = make_frame(0x0006, 3, std::span<const std::uint8_t>(kFc, 4));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, devices::tuya::kDefTS0044,
                                   raw, ctx);
    assert(r.any_matched);
    const Value* a = r.merged.find("action");
    assert(a && a->type == ValueType::StringRef);
    assert(std::strcmp(a->str, "3_rotate_right") == 0);
}

// A cmd id outside the unified converter's table (0x00–0x02, 0xFC, 0xFD)
// must still produce no-match — guards against an accidental wildcard
// that would swallow ZCL Default Response (cmd 0x0B), Report Attributes
// (0x0A), etc. that happen to ride cluster genOnOff.
static void test_unknown_cmd_does_not_match() {
    const std::uint8_t kFrame[] = { 0x01, 0x0c, 0x05, 0x00 };
    auto raw = make_frame(0x0006, 1, std::span<const std::uint8_t>(kFrame, 4));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, devices::tuya::kDefTS0044,
                                   raw, ctx);
    assert(!r.any_matched);
}

// ── P3: transaction-sequence dedup ──────────────────────────────────
//
// Tuya buttons retransmit each press 2-3× with the same transaction
// sequence byte. The unified converter remembers (device_index, tsn)
// and silently swallows a repeat within the 800 ms window — `any_matched`
// is still set (converter ran, so dispatcher won't log "unhandled"),
// but no `action` key reaches `merged` so the rules engine doesn't
// fire twice on a single press.

// Same (device, tsn) → first fires, second is suppressed.
static void test_retx_with_same_tsn_dedups() {
    reset_test_dedup();
    auto r1 = dispatch_press(/*ep=*/1, /*click=*/0,
                              /*tsn=*/0x40, /*dev_idx=*/42);
    expect_action(r1, "1_single");

    auto r2 = dispatch_press(/*ep=*/1, /*click=*/0,
                              /*tsn=*/0x40, /*dev_idx=*/42);
    // Retx: converter ran (matched) but emitted nothing.
    assert(r2.merged.find("action") == nullptr);
}

// Different tsn under the same device → both fire (legitimate next press).
static void test_distinct_tsn_both_fire() {
    reset_test_dedup();
    auto r1 = dispatch_press(/*ep=*/2, /*click=*/1,
                              /*tsn=*/0x10, /*dev_idx=*/77);
    expect_action(r1, "2_double");

    auto r2 = dispatch_press(/*ep=*/2, /*click=*/1,
                              /*tsn=*/0x11, /*dev_idx=*/77);
    expect_action(r2, "2_double");
}

// Same tsn but different device_index → both fire (two devices happen
// to share a tsn; dedup must NOT cross devices).
static void test_distinct_device_both_fire() {
    reset_test_dedup();
    auto r1 = dispatch_press(/*ep=*/1, /*click=*/0,
                              /*tsn=*/0x55, /*dev_idx=*/10);
    expect_action(r1, "1_single");

    auto r2 = dispatch_press(/*ep=*/1, /*click=*/0,
                              /*tsn=*/0x55, /*dev_idx=*/11);
    expect_action(r2, "1_single");
}

// device_index == 0 (default / synthetic frames) bypasses dedup, so
// the existing "all combinations" test that reuses tsn 0x0C across
// 12 calls keeps working. Lock that contract.
static void test_zero_device_index_skips_dedup() {
    reset_test_dedup();
    auto r1 = dispatch_press(/*ep=*/4, /*click=*/2,
                              /*tsn=*/0xAA, /*dev_idx=*/0);
    expect_action(r1, "4_hold");

    auto r2 = dispatch_press(/*ep=*/4, /*click=*/2,
                              /*tsn=*/0xAA, /*dev_idx=*/0);
    expect_action(r2, "4_hold");
}

int main() {
    test_all_button_click_combinations();
    test_real_capture_replay();
    test_unknown_click_value_no_match();
    test_endpoint_out_of_range_no_match();
    test_rotary_cmd_decodes_per_endpoint();
    test_unknown_cmd_does_not_match();
    test_retx_with_same_tsn_dedups();
    test_distinct_tsn_both_fire();
    test_distinct_device_both_fire();
    test_zero_device_index_skips_dedup();
    return 0;
}
