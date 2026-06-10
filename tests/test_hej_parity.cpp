// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Hej (Goqual) device family.
// z2m-source: hej.ts.
//
// Bug fixed (IAS dead-key): the two KKZ IAS sensors lowered the generic
// kFzIasZone converter — which emits the bare key "alarm" — while their
// exposes declared the *semantic* keys "contact" (KKZ-DO021) /
// "occupancy" (KKZ-MO021). With no rename layer the primary sensor state
// never reached the shadow. z2m decodes these via fz.ias_contact_alarm_1
// / fz.ias_occupancy_alarm_1 (both zoneStatus bit 0), so each def was
// graduated to a Tier-2 override wiring the typed kFzIasContactAlarm /
// kFzIasMotionAlarm converter (which emit the semantic key directly).
//
// The GLSK multi-gang switches already carried correct endpoint_map
// tables; the test below pins (regression guard) that a multi-gang def
// declares its endpoint map and that dispatch suffixes the per-channel
// genOnOff state to state_<label> so channels do not collide on the bare
// "state" key.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::hej {
extern const PreparedDefinition kDef_KKZ_DO021;     // contact   (ias_contact_alarm_1)
extern const PreparedDefinition kDef_KKZ_MO021;     // occupancy (ias_occupancy_alarm_1)
extern const PreparedDefinition kDef_GLSK3ZB_1713;  // 3-gang switch (top/center/bottom)
}  // namespace zhc::devices::hej

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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3 — mirrors test_heiman_parity.
// `invert`: z2m publishes contact = !bit0 for zoneType:"contact" — pass
// true for the contact device; occupancy/other typed alarms stay raw bit0.
void check_alarm1(const PreparedDefinition& def, const char* sem, bool invert = false) {
    assert(def_exposes(def, sem));            // regression guard vs bare "alarm"

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert((invert ? b_false : b_true)(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert((invert ? b_true : b_false)(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert((invert ? b_true : b_false)(tb.merged.find(sem)));  // bit0 clear here
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// genOnOff Report-Attributes carrying attr 0x0000 (onOff, bool) on a
// given source endpoint. fc(1)=0x18 | tsn | cmd=0x0A | attr 0x0000 LE |
// type 0x10 (bool) | value.
DispatchResult dispatch_onoff_report(const PreparedDefinition& def,
                                     std::uint8_t src_ep, bool value) {
    const std::uint8_t bytes[] = {
        0x18, 0x42, 0x0A,
        0x00, 0x00,                          // attr 0x0000
        0x10,                                // type bool
        static_cast<std::uint8_t>(value ? 1 : 0),
    };
    auto raw = make_frame(0x0006, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── KKZ-DO021 — contact (ias_contact_alarm_1, bit 0) ─────────────────
static void test_kkz_do021_contact() {
    check_alarm1(devices::hej::kDef_KKZ_DO021, "contact", /*invert=*/true);
}

// ── KKZ-MO021 — occupancy (ias_occupancy_alarm_1, bit 0) ─────────────
static void test_kkz_mo021_occupancy() {
    check_alarm1(devices::hej::kDef_KKZ_MO021, "occupancy");
}

// ── GLSK3ZB-1713 — 3-gang switch endpoint_map suffix regression ──────
static void test_glsk3_endpoint_suffix() {
    const auto& def = devices::hej::kDef_GLSK3ZB_1713;
    // Multi-gang def must declare its endpoint map so per-channel state
    // does not collide on the bare "state" key.
    assert(def.endpoint_map != nullptr);
    assert(def.endpoint_map_count == 3);

    // genOnOff state report on EP 2 (label "center") -> state_center.
    auto r2 = dispatch_onoff_report(def, 2, true);
    assert(r2.any_matched);
    assert(b_true(r2.merged.find("state_center")));
    assert(r2.merged.find("state") == nullptr);   // bare key suffixed away

    // EP 1 (label "top") -> state_top, independent channel.
    auto r1 = dispatch_onoff_report(def, 1, false);
    assert(r1.any_matched);
    assert(b_false(r1.merged.find("state_top")));
}

int main() {
    test_kkz_do021_contact();
    test_kkz_mo021_occupancy();
    test_glsk3_endpoint_suffix();
    return 0;
}
