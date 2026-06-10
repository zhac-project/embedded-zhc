// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Technicolor Xfinity security keypad (XHK1-TC),
// a sibling of the UniversalElectronicsInc XHK1-UE.
// z2m-source: technicolor.ts #XHK1-TC.
//
// The previous auto-port wired kFzOnOff/kTzOnOff (phantom on/off, bound 0x0006),
// the generic dead-key kFzIasZone (bare `alarm`), and dropped temperature
// entirely. z2m's bundle is fz.command_arm_with_transaction + fz.temperature +
// fz.battery + fz.ias_occupancy_alarm_1 + fz.ias_contact_alarm_1 +
// fz.ias_ace_occupancy_with_timeout. This pass rewires the def to the typed IAS
// occupancy/contact converters and kFzIasAceArmWithTransaction so the declared
// `action_transaction` (ZCL TSN) reaches the shadow.
//
// Coverage:
//   * ssIasZone status-change (bit0) → semantic `occupancy` AND `contact`
//     (both typed converters run on the same frame), plus tamper/battery_low;
//     never the bare `alarm`/`alarm_1` keys. Per z2m the `contact` key is
//     INVERTED (contact = !bit0): bit0 set → contact:false (open), bit0 clear
//     → contact:true (closed). `occupancy` stays raw bit0.
//   * ssIasAce commandArm (cluster 0x0501, cmd 0x00) → action / action_code /
//     action_zone / action_transaction (the fixed channel: ZCL TSN passthrough).
//   * Def shape: no `state` expose, no genOnOff bind, no toZigbee, binds 0x0501.

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

namespace zhc::devices::technicolor {
extern const PreparedDefinition kDef_XHK1_TC;   // keypad → IAS + ACE arm + transaction
}  // namespace zhc::devices::technicolor

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

// ── typed IAS bit0 alarms → occupancy + contact on the same frame ────
static void test_ias_occupancy_and_contact() {
    const auto& def = devices::technicolor::kDef_XHK1_TC;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "contact"));

    // bit0 set: BOTH typed converters fire on the same status-change frame.
    // occupancy is raw bit0 (true); contact is z2m-inverted (false on bit0).
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(b_false(on.merged.find("contact")));   // z2m: contact = !bit0 → open
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));
    // The generic dead-key must NOT leak.
    assert(on.merged.find("alarm") == nullptr);
    assert(on.merged.find("alarm_1") == nullptr);

    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));
    assert(b_true(off.merged.find("contact")));   // z2m: contact = !bit0 → closed

    // tamper(bit2) + battery_low(bit3), no alarm bit. bit0 clear → contact:true.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("occupancy")));
    assert(b_true(tb.merged.find("contact")));     // z2m: contact = !bit0 → closed
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── ssIasAce arm WITH transaction passthrough ────────────────────────
static void test_keypad_arm_with_transaction() {
    const auto& def = devices::technicolor::kDef_XHK1_TC;
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

int main() {
    test_ias_occupancy_and_contact();
    test_keypad_arm_with_transaction();
    return 0;
}
