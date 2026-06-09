// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Orvibo scene remotes CR11S8UZ + T40S6Z.
// z2m-source: orvibo.ts #CR11S8UZ (fz.orvibo_raw_1) + #T40S6Z
//             (fz.orvibo_raw_2); converters in fromZigbee.ts.
//
// Bug fixed: both devices were generated as state-only stubs wiring the
// generic kFzOnOff converter behind a Binary `state` expose. They are
// stateless scene remotes with no genOnOff relay — z2m decodes a
// vendor-private raw frame on cluster 0x0017 and publishes an `action`
// string ("button_<n>_<click|hold|release>"). So embedded-zhc emitted
// nothing on a button press: the entire action surface was lost.
//
// Both defs were graduated to Tier-2 parents wiring the shared
// kFzOrviboRaw1Action / kFzOrviboRaw2Action converters (orvibo/_shared.cpp).
// raw_1 maps the wire button byte {3,11,7,15} -> buttons 1..4; raw_2 is
// the identity {1..7}. Action byte {0,2,3} -> {click,hold,release}.
//
// These tests pin, on the documented raw wire shape (FC 0x19 =
// cluster-specific, server->client), that each button/action byte pair
// produces the z2m-matching "button_<n>_<verb>" action, that an
// unrecognised button byte and a direction-mismatched frame produce no
// match, and that the bare "state"/"on"/"off" literals never appear.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::orvibo {
extern const PreparedDefinition kDef_CR11S8UZ;  // 4-button sticker switch (raw_1)
extern const PreparedDefinition kDef_T40S6Z;    // 6-button mix switch    (raw_2)
}  // namespace zhc::devices::orvibo

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// 6-byte Orvibo raw button frame as documented by z2m (orvibo_raw_1):
//   data = [FC, tsn, cmd, BUTTON, 0x00, ACTION]
// FC 0x19 = cluster-specific (bit0), server->client (bit3), DDR (bit4).
// After the 3-byte ZCL header (FC/tsn/cmd, non-manufacturer) is stripped,
// raw_body = [BUTTON, 0x00, ACTION].
struct RawFrame { std::array<std::uint8_t, 6> bytes; };

RawFrame raw_frame(std::uint8_t button_byte, std::uint8_t action_byte,
                   std::uint8_t fc = 0x19, std::uint8_t tsn = 0x42,
                   std::uint8_t cmd = 0x08) {
    RawFrame f{};
    f.bytes = {fc, tsn, cmd, button_byte, 0x00, action_byte};
    return f;
}

DispatchResult run(const PreparedDefinition& def, const RawFrame& rf) {
    auto raw = make_frame(0x0017 /* manuSpecificOrvibo */, 1,
                          std::span<const std::uint8_t>(rf.bytes.data(),
                                                        rf.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

// ── CR11S8UZ — fz.orvibo_raw_1, 4 buttons ───────────────────────────
//   button byte {3->1, 11->2, 7->3, 15->4}; action {0:click,2:hold,3:release}
void test_cr11s8uz_actions() {
    const auto& def = devices::orvibo::kDef_CR11S8UZ;

    check(action_is(run(def, raw_frame(3,  0)), "button_1_click"),   "CR11 b1 click");
    check(action_is(run(def, raw_frame(3,  2)), "button_1_hold"),    "CR11 b1 hold");
    check(action_is(run(def, raw_frame(3,  3)), "button_1_release"), "CR11 b1 release");
    check(action_is(run(def, raw_frame(11, 0)), "button_2_click"),   "CR11 b2 click");
    check(action_is(run(def, raw_frame(7,  2)), "button_3_hold"),    "CR11 b3 hold");
    check(action_is(run(def, raw_frame(15, 3)), "button_4_release"), "CR11 b4 release");

    // Unrecognised button byte -> no match, no action emitted.
    const auto bad = run(def, raw_frame(99, 0));
    check(!bad.any_matched,        "CR11 unknown button no match");
    check(!has_key(bad, "action"), "CR11 unknown button no action key");

    // Unrecognised action byte (1) -> no match.
    check(!has_key(run(def, raw_frame(3, 1)), "action"), "CR11 unknown action no key");

    // The dead stub literals must never appear.
    const auto r = run(def, raw_frame(3, 0));
    check(!has_key(r, "state"),                 "CR11 no state key");
    check(!action_is(r, "on") && !action_is(r, "off"), "CR11 no on/off literal");

    // Direction-mismatched frame (FC 0x11 = cluster-specific, client->server)
    // must NOT match — the matcher enforces direction exactly.
    check(!run(def, raw_frame(3, 0, /*fc=*/0x11)).any_matched,
          "CR11 wrong-direction no match");
}

// ── T40S6Z — fz.orvibo_raw_2, identity button lookup ────────────────
void test_t40s6z_actions() {
    const auto& def = devices::orvibo::kDef_T40S6Z;

    check(action_is(run(def, raw_frame(1, 0)), "button_1_click"), "T40 b1 click");
    check(action_is(run(def, raw_frame(2, 0)), "button_2_click"), "T40 b2 click");
    check(action_is(run(def, raw_frame(6, 0)), "button_6_click"), "T40 b6 click");
    // raw_2 supports hold/release too (converter parity, not just the expose list).
    check(action_is(run(def, raw_frame(3, 2)), "button_3_hold"),    "T40 b3 hold");
    check(action_is(run(def, raw_frame(4, 3)), "button_4_release"), "T40 b4 release");

    // Button 0 / out-of-range -> no match.
    check(!run(def, raw_frame(0, 0)).any_matched, "T40 button0 no match");
    check(!run(def, raw_frame(8, 0)).any_matched, "T40 button8 no match");

    const auto r = run(def, raw_frame(1, 0));
    check(!has_key(r, "state"), "T40 no state key");
}

// ── Exposes: each def advertises `action` (String), not `state` ─────
void test_exposes() {
    for (const auto* def : {&devices::orvibo::kDef_CR11S8UZ,
                            &devices::orvibo::kDef_T40S6Z}) {
        bool has_action = false, has_state = false;
        for (std::size_t i = 0; i < def->exposes_count; ++i) {
            if (std::strcmp(def->exposes[i].name, "action") == 0) {
                has_action = true;
                check(def->exposes[i].type == ExposeType::String,
                      "action expose is String");
            }
            if (std::strcmp(def->exposes[i].name, "state") == 0) has_state = true;
        }
        check(has_action, "def exposes action");
        check(!has_state, "def no longer exposes state");
    }
}

}  // namespace

int main() {
    std::printf("test_orvibo_parity\n");
    test_cr11s8uz_actions();
    test_t40s6z_actions();
    test_exposes();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
