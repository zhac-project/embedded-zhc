// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Sengled IAS-Zone sensor family.
// z2m-source: sengled.ts — the per-zone-type m.iasZoneAlarm({zoneType: ...})
// extends, which decode via fz.ias_<type>_alarm_1 and publish a *semantic*
// key (occupancy / contact).
//
// Bug fixed: four Sengled IAS defs lowered (or, for E13-N11, half-rewrote to)
// the generic kFzIasZone converter — which emits the bare key "alarm" — while
// their expose declared (or should declare) the semantic key. With no rename
// layer the primary sensor state never reached the shadow:
//   - E1M-G7H    motion sensor   -> needs "occupancy" (was alarm/alarm)
//   - E1D-G73WNA contact sensor  -> needs "contact"   (was alarm/alarm)
//   - E2D-G73    contact sensor  -> needs "contact"   (was alarm/alarm)
//   - E13-N11    flood+occupancy -> expose was already "occupancy" but the
//                                   decoder was still kFzIasZone -> dead.
// Each def was graduated to a Tier-2 parent override that swaps in the typed
// kFzIas{Motion,Contact}Alarm converter (emitting the semantic key directly).
// Mirrors the heiman IAS family fix (see test_heiman_parity.cpp).
//
// These tests pin, on real Zone-Status-Change-Notification wire shapes, that
// each device now decodes its semantic key (and NOT the bare "alarm" key),
// that tamper/battery_low still surface, and — for E13-N11 — that the light
// channel (on/off) still decodes alongside the occupancy converter.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sengled {
extern const PreparedDefinition kDef_E1M_G7H;      // occupancy (motion, alarm_1)
extern const PreparedDefinition kDef_E1D_G73WNA;   // contact   (alarm_1)
extern const PreparedDefinition kDef_E2D_G73;      // contact   (alarm_1)
extern const PreparedDefinition kDef_E13_N11;      // light + occupancy (alarm_1)
}  // namespace zhc::devices::sengled

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3, and the bare "alarm" key is never
// produced (regression guard against generic kFzIasZone).
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));  // bare key must not leak into exposes

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

// ── motion / contact alarm_1 (bit 0) decoders ────────────────────────
static void test_e1m_g7h_occupancy()    { check_alarm1(devices::sengled::kDef_E1M_G7H,    "occupancy"); }
static void test_e1d_contact()          { check_alarm1(devices::sengled::kDef_E1D_G73WNA, "contact"); }
static void test_e2d_contact()          { check_alarm1(devices::sengled::kDef_E2D_G73,    "contact"); }

// ── E13-N11 — light + occupancy combo ────────────────────────────────
// The occupancy half must decode the semantic key; the light half must
// still decode on/off from the same def (no converter cross-talk).
static void test_e13_n11_occupancy() {
    check_alarm1(devices::sengled::kDef_E13_N11, "occupancy");
}

static void test_e13_n11_light_still_works() {
    const auto& def = devices::sengled::kDef_E13_N11;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));

    // genOnOff (0x0006) Report Attributes: attr 0x0000 (onOff, bool) = 1.
    const std::uint8_t kOnReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000
        0x10,                    // type bool
        0x01,                    // value 1 = on
    };
    auto raw = make_frame(0x0006, 1, kOnReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(b_true(r.merged.find("state")));
    // occupancy must NOT spuriously appear on an on/off report.
    assert(r.merged.find("occupancy") == nullptr);
}

int main() {
    test_e1m_g7h_occupancy();
    test_e1d_contact();
    test_e2d_contact();
    test_e13_n11_occupancy();
    test_e13_n11_light_still_works();
    return 0;
}
