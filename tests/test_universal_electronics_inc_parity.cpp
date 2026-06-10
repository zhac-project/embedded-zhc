// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Universal Electronics Inc (One For All / Xfinity)
// sensor + security-keypad family.
// z2m-source: universal_electronics_inc.ts.
//
// Coverage:
//   * XHS1-UE (PIR) — fz.ias_occupancy_alarm_1 (zoneStatus bit0) → `occupancy`,
//     plus tamper/battery_low; not the bare `alarm` key.
//   * XHS2-UE (contact) — fz.ias_contact_alarm_1 (bit0) → `contact`.
//   * XHK1-UE + UEHK2AZ0 (Xfinity security keypads) — ssIasAce commandArm
//     (cluster 0x0501, cmd 0x00) decodes `action`/`action_code`/`action_zone`,
//     and — the bug this pass fixes — `action_transaction`: z2m's
//     fz.command_arm_with_transaction copies the ZCL transaction sequence
//     number into action_transaction. The previous port wired kFzIasAceArm,
//     which dropped the TSN, leaving the declared `action_transaction` expose
//     dead. Both keypads now wire kFzIasAceArmWithTransaction and the TSN
//     (DecodedMessage.transaction_sequence) reaches the shadow.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::universal_electronics_inc {
extern const PreparedDefinition kDef_XHS1_UE;    // PIR     → occupancy
extern const PreparedDefinition kDef_XHS2_UE;    // contact → contact
extern const PreparedDefinition kDef_XHK1_UE;    // keypad  → ACE arm + transaction
extern const PreparedDefinition kDef_UEHK2AZ0;   // keypad  → ACE arm + transaction
}  // namespace zhc::devices::universal_electronics_inc

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool str_is(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}

bool uint_is(const Value* v, std::uint64_t want) {
    return v && v->type == ValueType::Uint && v->u == want;
}

// ── IAS Zone Status Change Notification (cluster 0x0500) ─────────────
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

// alarm_1 (bit0) → semantic key true; tamper/battery_low ride bits 2/3.
// The bare "alarm"/"alarm_1" keys must NOT leak.
// `invert`: z2m publishes contact = !bit0 (zoneType:"contact") — pass true
// for the contact device so bit0-SET → key:false, bit0-CLEAR → key:true.
// occupancy and the other typed alarms stay raw bit0 (invert=false).
void check_alarm1(const PreparedDefinition& def, const char* sem, bool invert = false) {
    assert(def_exposes(def, sem));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert((invert ? b_false : b_true)(on.merged.find(sem)));
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));
    assert(on.merged.find("alarm") == nullptr);
    assert(on.merged.find("alarm_1") == nullptr);

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert((invert ? b_true : b_false)(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert((invert ? b_true : b_false)(tb.merged.find(sem)));  // bit0 clear here
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── ssIasAce commandArm frame (cluster 0x0501) ───────────────────────
// fc=0x01 (cluster-specific, client->server), tsn, cmd=0x00;
//   body: arm_mode(1), code_len(1), code[code_len], zone_id(1).
std::vector<std::uint8_t> ace_arm(std::uint8_t arm_mode, std::uint8_t tsn,
                                  std::span<const std::uint8_t> code,
                                  std::uint8_t zone) {
    std::vector<std::uint8_t> v{0x01, tsn, 0x00};
    v.push_back(arm_mode);
    v.push_back(static_cast<std::uint8_t>(code.size()));
    for (auto b : code) v.push_back(b);
    v.push_back(zone);
    return v;
}
DispatchResult dispatch_ace(const PreparedDefinition& def, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(0x0501, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasAce";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── XHS1-UE / XHS2-UE — typed IAS bit0 alarms ────────────────────────
static void test_xhs1_pir()     { check_alarm1(devices::universal_electronics_inc::kDef_XHS1_UE, "occupancy"); }
static void test_xhs2_contact() { check_alarm1(devices::universal_electronics_inc::kDef_XHS2_UE, "contact", /*invert=*/true); }

// ── XHK1-UE / UEHK2AZ0 — ssIasAce arm WITH transaction passthrough ───
static void check_keypad_arm(const PreparedDefinition& def) {
    // The keypad is an IAS source/sender, never an on/off sink.
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "action_code"));
    assert(def_exposes(def, "action_zone"));
    assert(def_exposes(def, "action_transaction"));   // the fixed channel
    assert(!def_exposes(def, "state"));
    assert(def_binds(def, 0x0501));                   // ssIasAce
    assert(!def_binds(def, 0x0006));                  // no genOnOff
    assert(def.to_zigbee_count == 0);

    // commandArm arm_mode=3 (arm_all_zones), code "1234", zone 7, TSN 0x5A.
    const std::uint8_t code[] = {'1', '2', '3', '4'};
    auto arm = dispatch_ace(def, ace_arm(0x03, 0x5A, code, 0x07));
    assert(arm.any_matched);
    assert(str_is(arm.merged.find("action"), "arm_all_zones"));
    assert(str_is(arm.merged.find("action_code"), "1234"));
    assert(uint_is(arm.merged.find("action_zone"), 0x07));
    // The bug fix: ZCL TSN passes through to action_transaction.
    assert(uint_is(arm.merged.find("action_transaction"), 0x5A));

    // arm_mode=0 (disarm), empty code, different TSN — transaction tracks per frame.
    auto disarm = dispatch_ace(def, ace_arm(0x00, 0x11, {}, 0x00));
    assert(disarm.any_matched);
    assert(str_is(disarm.merged.find("action"), "disarm"));
    assert(uint_is(disarm.merged.find("action_transaction"), 0x11));
}

static void test_xhk1_keypad()    { check_keypad_arm(devices::universal_electronics_inc::kDef_XHK1_UE); }
static void test_uehk2az0_keypad(){ check_keypad_arm(devices::universal_electronics_inc::kDef_UEHK2AZ0); }

int main() {
    test_xhs1_pir();
    test_xhs2_contact();
    test_xhk1_keypad();
    test_uehk2az0_keypad();
    return 0;
}
