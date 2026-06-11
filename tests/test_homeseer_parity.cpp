// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the HomeSeer DS150ZB door (contact) sensor.
// z2m-source: homeseer.ts #DS150ZB
//   extend: [m.battery(),
//            m.iasZoneAlarm({zoneType:"contact",
//                            zoneAttributes:["alarm_1","battery_low"]})]
//
// Bug fixed:
//  (c) IAS dead-key — the auto-port lowered the generic kFzIasZone converter,
//      which emits the bare key "alarm", while z2m's iasZoneAlarm with
//      zoneType:"contact" publishes the semantic key "contact" and INVERTS it
//      (invertAlarmPayload = zoneType==="contact" → contact = !zoneStatus bit0).
//      With no rename layer the contact state never reached the shadow.
//      Graduated to a Tier-2 override swapping in kFzIasContactAlarm.
//  (e) Phantom exposes — the auto-port declared "alarm", "tamper" and "voltage".
//      z2m exposes none of these: m.battery() defaults voltage=false (battery %
//      only) and the iasZoneAlarm zoneAttributes are ["alarm_1","battery_low"]
//      (no tamper). Exposes aligned to battery + contact + battery_low.
//
// These tests pin, on real wire shapes, that DS150ZB decodes the inverted
// "contact" semantic key (and NOT the bare "alarm" key), reads battery_low from
// zoneStatus bit3, and that the dead "alarm"/"tamper"/"voltage" exposes are gone.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::homeseer {
extern const PreparedDefinition kDef_DS150ZB;  // battery + contact
}  // namespace zhc::devices::homeseer

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

// ── DS150ZB contact sensor ──────────────────────────────────────────
// kFzIasContactAlarm: z2m publishes contact = !(zoneStatus bit0). So a
// bit-0-set frame (door open) yields contact:false and a bit-0-clear frame
// (door closed) yields contact:true. The bare "alarm" key must NOT appear.
static void test_ds150_contact_notif() {
    const auto& def = devices::homeseer::kDef_DS150ZB;
    assert(def_exposes(def, "contact"));
    assert(def_binds(def, 0x0500));   // ssIasZone bound

    auto open = dispatch_ias_notif(def, ias_notif(0x0001));   // bit0 set -> open
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));
    assert(open.merged.find("alarm") == nullptr);             // dead-key gone
    assert(b_false(open.merged.find("battery_low")));

    auto closed = dispatch_ias_notif(def, ias_notif(0x0000)); // bit0 clear -> closed
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));
}

// battery_low rides IAS zoneStatus bit3; contact stays inverted bit0.
// kFzIasContactAlarm matches the Zone Status Change Notification (cmd 0x00).
static void test_ds150_battery_low() {
    const auto& def = devices::homeseer::kDef_DS150ZB;

    auto bl = dispatch_ias_notif(def, ias_notif(0x0008));     // bit3 set -> battery_low
    assert(bl.any_matched);
    assert(b_true(bl.merged.find("battery_low")));
    assert(b_true(bl.merged.find("contact")));                // bit0 clear -> closed
}

// Phantom-expose guard: the dead "alarm", "tamper" and "voltage" exposes
// declared by the auto-port must be gone; z2m exposes none of them here.
static void test_ds150_no_phantom_exposes() {
    const auto& def = devices::homeseer::kDef_DS150ZB;
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "battery_low"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "tamper"));
    assert(!def_exposes(def, "voltage"));
}

int main() {
    test_ds150_contact_notif();
    test_ds150_battery_low();
    test_ds150_no_phantom_exposes();
    return 0;
}
