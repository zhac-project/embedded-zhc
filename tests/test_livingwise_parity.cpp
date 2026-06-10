// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the LivingWise LVS series — IAS sensors + scene controller.
// z2m-source: livingwise.ts.
//
// Bugs fixed (all generated as wrong-key / wrong-class stubs):
//   * LVS-SM10ZW (contact)  — wired generic kFzIasZone (bare `alarm`) but
//     exposes the semantic `contact` key. z2m fz.ias_contact_alarm_1 emits
//     `contact` (zoneStatus bit 0). Dead IAS key. Swapped to the typed
//     kFzIasContactAlarm + expose `alarm`->`contact`.
//   * LVS-SN10ZW_SN11 (occupancy) — same kFzIasZone dead-key bug. z2m
//     fz.ias_occupancy_alarm_1_with_timeout emits `occupancy` (bit 0).
//     Swapped to kFzIasMotionAlarm + expose `alarm`->`occupancy`. battery
//     (fz.battery) untouched.
//   * LVS-SC7 (scene controller) — generated as a controllable on/off
//     switch (kFzOnOff/kTzOnOff behind a `state` StateSet expose). It is a
//     stateless 7-button remote: z2m fz.orvibo_raw_2 decodes a vendor raw
//     frame on cluster 0x0017 into `action` = "button_<n>_<verb>". Reused
//     the shared kFzOrviboRaw2Action converter; dropped the dead state.
//
// These tests pin the semantic IAS keys (bare `alarm` gone), tamper /
// battery_low bits, the per-button action strings, and that the SC7 no
// longer carries the phantom `state` expose / on/off literals.

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

namespace zhc::devices::livingwise {
extern const PreparedDefinition kDef_LVS_SM10ZW;        // contact (ias_contact_alarm_1)
extern const PreparedDefinition kDef_LVS_SN10ZW_SN11;   // occupancy (ias_occupancy_alarm_1)
extern const PreparedDefinition kDef_LVS_SC7;           // 7-button scene remote (orvibo_raw_2)
}  // namespace zhc::devices::livingwise

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// ── IAS ─────────────────────────────────────────────────────────────
// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Assert: alarm_1 (bit 0) -> semantic key, bare `alarm` absent,
// tamper/battery_low track bits 2/3. `invert` follows z2m's contact polarity
// (zoneType:"contact" publishes contact = !bit0: closed door = magnet present
// = bit0 clear = contact:true). occupancy stays raw bit0 (invert=false).
void check_alarm1(const PreparedDefinition& def, const char* sem,
                  bool invert = false) {
    check(def_exposes(def, sem), "semantic key is a declared expose");
    check(!def_exposes(def, "alarm"), "bare alarm key not exposed");

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    check(on.any_matched, "alarm_1 frame matched");
    // bit0 set → raw key true; inverted (contact) key false.
    check((invert ? b_false : b_true)(on.merged.find(sem)), "semantic key polarity on alarm_1");
    check(on.merged.find("alarm") == nullptr, "bare alarm key gone");
    check(b_false(on.merged.find("tamper")), "tamper clear on alarm_1");
    check(b_false(on.merged.find("battery_low")), "battery_low clear on alarm_1");

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    check(off.any_matched, "clear frame matched");
    // bit0 clear → raw key false; inverted (contact) key true.
    check((invert ? b_true : b_false)(off.merged.find(sem)), "semantic key polarity on clear");

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    check(tb.any_matched, "tamper+batt frame matched");
    // bit0 clear → raw key false; inverted (contact) key true.
    check((invert ? b_true : b_false)(tb.merged.find(sem)), "semantic key polarity when only tamper/batt");
    check(b_true(tb.merged.find("tamper")), "tamper set on bit2");
    check(b_true(tb.merged.find("battery_low")), "battery_low set on bit3");
}

void test_contact()   { check_alarm1(devices::livingwise::kDef_LVS_SM10ZW,      "contact", /*invert=*/true); }
void test_occupancy() { check_alarm1(devices::livingwise::kDef_LVS_SN10ZW_SN11, "occupancy"); }

// ── SC7 — fz.orvibo_raw_2 scene controller ──────────────────────────
// 6-byte raw frame: [FC, tsn, cmd, BUTTON, 0x00, ACTION].
// FC 0x19 = cluster-specific (bit0) + server->client (bit3) + DDR (bit4).
struct RawFrame { std::array<std::uint8_t, 6> bytes; };

RawFrame raw_frame(std::uint8_t button_byte, std::uint8_t action_byte,
                   std::uint8_t fc = 0x19, std::uint8_t tsn = 0x42,
                   std::uint8_t cmd = 0x08) {
    RawFrame f{};
    f.bytes = {fc, tsn, cmd, button_byte, 0x00, action_byte};
    return f;
}

DispatchResult dispatch_raw(const PreparedDefinition& def, const RawFrame& rf) {
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

void test_sc7_actions() {
    const auto& def = devices::livingwise::kDef_LVS_SC7;

    // Identity button lookup {1..7}; action byte {0:click,2:hold,3:release}.
    check(action_is(dispatch_raw(def, raw_frame(1, 0)), "button_1_click"),   "SC7 b1 click");
    check(action_is(dispatch_raw(def, raw_frame(4, 2)), "button_4_hold"),    "SC7 b4 hold");
    check(action_is(dispatch_raw(def, raw_frame(7, 3)), "button_7_release"), "SC7 b7 release");

    // Out-of-range button -> no match.
    check(!dispatch_raw(def, raw_frame(0, 0)).any_matched, "SC7 button0 no match");
    check(!dispatch_raw(def, raw_frame(8, 0)).any_matched, "SC7 button8 no match");

    // Unrecognised action byte -> no match.
    check(!has_key(dispatch_raw(def, raw_frame(1, 1)), "action"), "SC7 unknown action no key");

    // Direction-mismatched frame (FC 0x11 = client->server) must NOT match.
    check(!dispatch_raw(def, raw_frame(1, 0, /*fc=*/0x11)).any_matched,
          "SC7 wrong-direction no match");

    // Phantom on/off must be gone.
    const auto r = dispatch_raw(def, raw_frame(1, 0));
    check(!has_key(r, "state"), "SC7 no state key");
    check(!action_is(r, "on") && !action_is(r, "off"), "SC7 no on/off literal");
}

void test_sc7_exposes() {
    const auto& def = devices::livingwise::kDef_LVS_SC7;
    check(def_exposes(def, "action"), "SC7 exposes action");
    check(!def_exposes(def, "state"), "SC7 no longer exposes state");
    check(def.to_zigbee_count == 0, "SC7 has no to_zigbee (dropped kTzOnOff)");
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, "action") == 0)
            check(def.exposes[i].type == ExposeType::String, "SC7 action expose is String");
}

}  // namespace

int main() {
    std::printf("test_livingwise_parity\n");
    test_contact();
    test_occupancy();
    test_sc7_actions();
    test_sc7_exposes();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
