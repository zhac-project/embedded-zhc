// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the TP-Link IAS-Zone sensors (MS100 motion, CS100 contact).
// z2m-source: tplink.ts.
//
// Bug fixed: both defs were auto-ported wiring the generic kFzIasZone, which
// emits the bare `alarm` key. z2m instead wires:
//   - MS100: fz.ias_occupancy_alarm_1  -> `occupancy` (zoneStatus bit0),
//            + m.illuminance() (msIlluminanceMeasurement 0x0400) -> `illuminance`.
//   - CS100: fz.ias_contact_alarm_1    -> `contact` = !(zoneStatus bit0)  [INVERTED].
// The semantic occupancy/contact exposes were therefore dead. Both defs were
// graduated to Tier-2 parent overrides using the typed converters
// kFzIasMotionAlarm / kFzIasContactAlarm (and kFzIlluminance for MS100).
//
// These tests pin, on a real Zone-Status-Change-Notification wire shape:
//   1. MS100 decodes `occupancy` from bit0 (not the bare `alarm`), with the
//      shared tamper/battery_low bits, and the illuminance channel is live.
//   2. CS100 decodes `contact` INVERTED from bit0 (bit0 clear -> contact:true)
//      with battery_low, and never emits the bare `alarm`.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tplink {
extern const PreparedDefinition kDef_MS100;  // motion / occupancy (bit0)
extern const PreparedDefinition kDef_CS100;  // contact (inverted bit0)
}  // namespace zhc::devices::tplink

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

// msIlluminanceMeasurement (0x0400) attribute report of measuredValue (0x0000,
// u16 LE). ZCL header fc(1)=0x08 tsn(1) cmd(1)=0x0A, then attr(2 LE) type(1)=u16
// value(2 LE).
struct AttrFrame { std::array<std::uint8_t, 8> bytes; };

AttrFrame illum_report(std::uint16_t value) {
    AttrFrame f{};
    f.bytes = { 0x08, 0x10, 0x0A,
                0x00, 0x00,       // attr 0x0000 (measuredValue)
                0x21,             // type uint16
                static_cast<std::uint8_t>(value & 0xFF),
                static_cast<std::uint8_t>(value >> 8) };
    return f;
}

DispatchResult dispatch_illum(const PreparedDefinition& def, const AttrFrame& f) {
    auto raw = make_frame(0x0400, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msIlluminanceMeasurement";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// MS100: occupancy from bit0, plus a live illuminance channel. Never `alarm`.
void test_ms100_occupancy_and_illuminance() {
    const auto& def = devices::tplink::kDef_MS100;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "illuminance"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "alarm"));

    // bit0 set -> occupancy true.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // bit0 clear -> occupancy false.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));

    // tamper(bit2) + battery_low(bit3) without the occupancy bit.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("occupancy")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));

    // illuminance channel decodes (was dropped by the Tier-1 auto-port).
    auto lux = dispatch_illum(def, illum_report(0x0064));  // raw 100
    assert(lux.any_matched);
    const Value* iv = lux.merged.find("illuminance");
    assert(iv && iv->type == ValueType::Uint && iv->u == 100);
}

// CS100: contact is INVERTED from bit0 (z2m fz.ias_contact_alarm_1). Closed
// door = magnet present = bit0 clear = contact:true. Never `alarm`.
void test_cs100_contact_inverted() {
    const auto& def = devices::tplink::kDef_CS100;
    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "occupancy"));
    assert(!def_exposes(def, "tamper"));  // CS100 z2m exposes no tamper

    // bit0 CLEAR -> contact true (closed).
    auto closed = dispatch_ias(def, ias_notif(0x0000));
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));
    assert(closed.merged.find("alarm") == nullptr);
    assert(b_false(closed.merged.find("battery_low")));

    // bit0 SET -> contact false (open).
    auto open = dispatch_ias(def, ias_notif(0x0001));
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));

    // battery_low(bit3) with door open.
    auto low = dispatch_ias(def, ias_notif(0x0009));  // bit0 + bit3
    assert(low.any_matched);
    assert(b_false(low.merged.find("contact")));
    assert(b_true(low.merged.find("battery_low")));
}

}  // namespace

int main() {
    test_ms100_occupancy_and_illuminance();
    test_cs100_contact_inverted();
    return 0;
}
