// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the ROOME contact sensor family.
// z2m-source: roome.ts
//   - HSC1-WD-0 (MULTI-MECI--EA01) : fz.ias_contact_alarm_1 + fz.battery,
//                                     exposes contact/battery_low/tamper.
//
// Bug fixed:
//  (c) IAS dead-key — the auto-port lowered the generic kFzIasZone converter,
//      which emits the bare key "alarm", while z2m's fz.ias_contact_alarm_1
//      publishes the semantic key "contact" (inverted: contact = !(zoneStatus
//      bit0)), plus tamper (bit2) and battery_low (bit3). With no rename layer
//      the contact state never reached the shadow. Graduated to a Tier-2
//      override swapping in kFzIasContactAlarm (ssIasZone command 0x00 →
//      z2m polarity) and renaming the dead "alarm" expose to "contact".
//
// These tests pin, on a real wire shape (IAS Zone Status Change Notification,
// command 0x00), that HSC1-WD-0 decodes the inverted "contact" semantic key
// (and NOT the bare "alarm" key), plus tamper/battery_low.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::roome {
extern const PreparedDefinition kDef_HSC1_WD_0;  // contact + battery
}  // namespace zhc::devices::roome

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// 9-byte IAS Zone Status Change Notification (command 0x00):
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

DispatchResult dispatch_ias_notif(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── HSC1-WD-0 contact sensor ─────────────────────────────────────────
// fz.ias_contact_alarm_1: z2m publishes contact = !(zoneStatus bit0). So a
// bit-0-set frame (door open) yields contact:false and a bit-0-clear frame
// (door closed) yields contact:true. The bare "alarm" key must NOT appear.
static void test_hsc1_contact() {
    const auto& def = devices::roome::kDef_HSC1_WD_0;
    assert(def_exposes(def, "contact"));
    assert(!def_exposes(def, "alarm"));            // dead-key expose renamed away
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_binds(def, 0x0500));                // ssIasZone bound
    assert(def_binds(def, 0x0001));                // genPowerCfg (battery) bound

    auto open = dispatch_ias_notif(def, ias_notif(0x0001));   // bit0 set -> open
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));
    assert(open.merged.find("alarm") == nullptr);             // dead-key gone
    assert(b_false(open.merged.find("tamper")));
    assert(b_false(open.merged.find("battery_low")));

    auto closed = dispatch_ias_notif(def, ias_notif(0x0000)); // bit0 clear -> closed
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));

    auto tb = dispatch_ias_notif(def, ias_notif(0x000C));     // tamper(bit2)+batt_low(bit3)
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("contact")));                // bit0 clear -> closed
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

int main() {
    test_hsc1_contact();
    return 0;
}
