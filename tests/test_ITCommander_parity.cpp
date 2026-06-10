// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the IT Commander family. Pins the two real gaps the
// auto-generated ports carried against the z2m wire shape:
//
//   * ITCMDR_Contact wired the generic `kFzIasZone`, which emits a bare
//     `alarm` (zoneStatus bit 0) that never matched the `contact`
//     expose, so the door/window state was dead. z2m's
//     `fz.ias_contact_alarm_1` publishes the INVERTED bit-0 as `contact`
//     (closed = magnet present = bit0 clear = contact:true), plus
//     `tamper` (bit 2) and `battery_low` (bit 3). Fixed by wiring the
//     generic `kFzIasContactAlarm` (kLbl_Contact { "contact", bit 0,
//     invert }).
//
//   * ITCMDR_Click is a BUTTON, but the auto-port mis-wired it as a
//     controllable genOnOff switch (`kFzOnOff` + `kTzOnOff` + a `state`
//     StateSet expose). z2m's role is `action` via `fz.itcmdr_clicks`,
//     which maps genMultistateInput (0x0012) PresentValue (attr 0x0055)
//     to single/double/triple/quadruple/hold/release/many. Fixed with
//     the vendor `kFzItcmdrClicks` converter (definitions/ITCommander/
//     _shared.cpp).
//
// z2m-source: zigbee-herdsman-converters/src/devices/ITCommander.ts
//             + converters/fromZigbee.ts (ias_contact_alarm_1,
//             itcmdr_clicks).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ITCommander {
extern const PreparedDefinition kDef_ITCMDR_Contact;
extern const PreparedDefinition kDef_ITCMDR_Click;
}  // namespace zhc::devices::ITCommander

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

// genMultistateInput Report Attributes for PresentValue (attr 0x0055,
// type uint16 0x21):
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attr_le(2)=0x55,0x00  type(1)=0x21
//   value_le(2)
struct MsFrame { std::array<std::uint8_t, 8> bytes; };

MsFrame ms_present(std::uint16_t value, std::uint8_t tsn = 0x10) {
    MsFrame f{};
    f.bytes = { 0x18, tsn, 0x0A, 0x55, 0x00, 0x21,
                static_cast<std::uint8_t>(value & 0xFF),
                static_cast<std::uint8_t>(value >> 8) };
    return f;
}

DispatchResult dispatch_ms(const PreparedDefinition& def, const MsFrame& f) {
    auto raw = make_frame(0x0012, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genMultistateInput";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool is_action(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, want) == 0;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

}  // namespace

// ── ITCMDR_Contact: contact = !(zoneStatus bit0) ────────────────────
static void test_contact() {
    const auto& def = devices::ITCommander::kDef_ITCMDR_Contact;
    assert(def_exposes(def, "contact"));
    assert(!def_exposes(def, "alarm"));   // dead generic key must be gone

    // zoneStatus 0 (bit0 clear) -> contact:true (closed / magnet present).
    auto closed = dispatch_ias(def, ias_notif(0x0000));
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));
    assert(closed.merged.find("alarm") == nullptr);   // never emit bare alarm

    // bit0 set -> contact:false (open).
    auto open = dispatch_ias(def, ias_notif(0x0001));
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));

    // bit2 -> tamper; bit3 -> battery_low.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    // bit0 still clear here -> contact true.
    assert(b_true(tb.merged.find("contact")));
}

// ── ITCMDR_Click: presentValue -> action; NOT an on/off switch ──────
static void test_click() {
    const auto& def = devices::ITCommander::kDef_ITCMDR_Click;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));        // phantom on/off must be gone
    assert(def.to_zigbee_count == 0);          // no dead toggle path

    struct { std::uint16_t pv; const char* action; } cases[] = {
        {0,   "hold"},
        {1,   "single"},
        {2,   "double"},
        {3,   "triple"},
        {4,   "quadruple"},
        {255, "release"},
        {7,   "many"},     // anything unmapped -> "many"
    };
    for (const auto& c : cases) {
        auto r = dispatch_ms(def, ms_present(c.pv));
        assert(r.any_matched);
        assert(is_action(r.merged.find("action"), c.action));
        // Must never emit a phantom on/off state.
        assert(r.merged.find("state") == nullptr);
    }
}

int main() {
    test_contact();
    test_click();
    return 0;
}
