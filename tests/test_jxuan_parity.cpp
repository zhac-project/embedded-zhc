// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the J.XUAN sensor / button / plug family.
// z2m-source: zigbee-herdsman-converters/src/devices/jxuan.ts +
//             converters/fromZigbee.ts.
//
// Bugs fixed (each device graduated generated/ -> Tier-2 parent):
//
//  * IAS dead-key (PRZ01): z2m wires fz.ias_occupancy_alarm_1_with_timeout
//    (zoneStatus bit 0) and exposes e.occupancy(). The generated port wired
//    the generic kFzIasZone (bare alarm_1/alarm_2 keys), so the advertised
//    `occupancy` state was never populated. Now wired to kFzIasMotionAlarm
//    (emits `occupancy` from bit 0 + tamper/battery_low).
//
//  * IAS dead-key (DSZ01): z2m wires fz.ias_contact_alarm_1 (contact = !bit0)
//    and exposes e.contact(). The generated port wired kFzIasZone, so
//    `contact` was dead. Now wired to kFzIasContactAlarm (emits the
//    already-z2m-inverted `contact` + tamper/battery_low).
//
//  * Phantom on/off + dead action (WSZ01): WSZ01 is a battery button whose
//    z2m def decodes fz.WSZ01_on_off_action (manuSpecific cluster 0xFE05,
//    attr id 1 -> action {0:release,1:single,2:double,3:hold}) + fz.battery
//    with an EMPTY toZigbee. The port mis-rendered it as a controllable
//    genOnOff switch (kFzOnOff + kTzOnOff + phantom `state`) and dropped the
//    action. Now wired to the vendor kFzWsz01Action (emits `action`) +
//    kFzBattery, no toZigbee, full model id restored.
//
// These tests pin the corrected wire-shape decode for each fix.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::jxuan {
extern const PreparedDefinition kDef_PRZ01;  // occupancy (IAS bit0) + battery
extern const PreparedDefinition kDef_DSZ01;  // contact (IAS !bit0) + battery
extern const PreparedDefinition kDef_WSZ01;  // button action + battery
}  // namespace zhc::devices::jxuan

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool str_eq(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str
           && std::strcmp(v->str, expect) == 0;
}

// ── 9-byte IAS Zone Status Change Notification (ssIasZone cmd 0x00) ───
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = {0x09, tsn, 0x00,
               static_cast<std::uint8_t>(zone_status & 0xFF),
               static_cast<std::uint8_t>(zone_status >> 8),
               0x00, 0x00, 0x00, 0x00};
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

// ── WSZ01 button: attributeReport on manuSpecific cluster 0xFE05, attr 1 ─
//   fc(1)=0x18 tsn(1) cmd(1)=0x0A  attr_le(2)=0x0001  type(1)=0x20(u8) val(1)
std::array<std::uint8_t, 7> wsz_report(std::uint8_t code) {
    return {0x18, 0x42, 0x0A, 0x01, 0x00, 0x20, code};
}
DispatchResult dispatch_wsz(const PreparedDefinition& def, std::uint8_t code) {
    auto bytes = wsz_report(code);
    auto raw = make_frame(0xFE05, 1,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0xFE05);  // "manuSpecificJxuan"
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── PRZ01 — occupancy on IAS bit0 (fz.ias_occupancy_alarm_1) ─────────
void test_prz01_occupancy() {
    const auto& def = devices::jxuan::kDef_PRZ01;
    assert(def_exposes(def, "occupancy"));
    assert(!def_exposes(def, "alarm"));       // dead bare key gone
    assert(!def_exposes(def, "contact"));     // not a contact sensor
    assert(def_binds(def, 0x0500));

    // bit 0 set -> occupancy true
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));

    // bit 0 clear -> occupancy false
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));

    // tamper (bit 2) + battery_low (bit 3) surface.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find("occupancy")));
}

// ── DSZ01 — contact on IAS bit0, z2m-inverted (contact = !bit0) ──────
void test_dsz01_contact() {
    const auto& def = devices::jxuan::kDef_DSZ01;
    assert(def_exposes(def, "contact"));
    assert(!def_exposes(def, "alarm"));       // dead bare key gone
    assert(!def_exposes(def, "occupancy"));   // not a motion sensor
    assert(def_binds(def, 0x0500));

    // bit 0 set -> magnet away -> contact false (z2m inverts: !bit0)
    auto open = dispatch_ias(def, ias_notif(0x0001));
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));

    // bit 0 clear -> magnet present -> contact true
    auto closed = dispatch_ias(def, ias_notif(0x0000));
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));

    // tamper + battery_low still surface.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── WSZ01 — button action via 0xFE05 attr 1 lookup ──────────────────
void test_wsz01_action() {
    const auto& def = devices::jxuan::kDef_WSZ01;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));       // not a controllable switch
    assert(def.to_zigbee_count == 0);         // no TZ (button, not switch)
    assert(!def_binds(def, 0x0006));          // no phantom genOnOff bind
    assert(def_binds(def, 0x0001));           // battery (genPowerCfg)

    auto rel = dispatch_wsz(def, 0);
    assert(rel.any_matched);
    assert(str_eq(rel.merged.find("action"), "release"));

    auto single = dispatch_wsz(def, 1);
    assert(single.any_matched);
    assert(str_eq(single.merged.find("action"), "single"));

    auto dbl = dispatch_wsz(def, 2);
    assert(dbl.any_matched);
    assert(str_eq(dbl.merged.find("action"), "double"));

    auto hold = dispatch_wsz(def, 3);
    assert(hold.any_matched);
    assert(str_eq(hold.merged.find("action"), "hold"));

    // Unknown code -> converter declines, no bogus action leaks, and no
    // phantom on/off state is ever published.
    auto unknown = dispatch_wsz(def, 0xAA);
    assert(unknown.merged.find("action") == nullptr);
    assert(unknown.merged.find("state") == nullptr);
}

}  // namespace

int main() {
    test_prz01_occupancy();
    test_dsz01_contact();
    test_wsz01_action();
    return 0;
}
