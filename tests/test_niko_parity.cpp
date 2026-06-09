// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Niko (Belgian wired Connected) family.
// z2m-source: zigbee-herdsman-converters/src/devices/niko.ts.
//
// Bug fixed (device graduated generated/ -> Tier-2 parent):
//
//  * IAS dead-key — 552-80401 wireless motion sensor. The generated def
//    lowered the generic kFzIasZone converter, which emits the bare key
//    "alarm" instead of the "occupancy" key the expose declared, leaving
//    occupancy dead. z2m's fz.ias_occupancy_alarm_1 emits
//    occupancy/tamper/battery_low from zoneStatus bits 0/2/3. Re-wired to
//    the typed kFzIasMotionAlarm converter (semantic key "occupancy" +
//    tamper + battery_low) and renamed the expose.
//
// The remaining tests are regression guards pinning the verified-correct
// wire-shape decode for the other Niko suspect classes that this parity
// pass investigated and found already correct:
//
//  * 552-72301 motor control — cover "position" on closuresWindowCovering
//    (0x0102), lift-only (no tilt, matching e.cover_position()), raw
//    0..100 passthrough. z2m's meta {coverInverted:true} combined with
//    the default invert_cover=false is a net no-op (read+write both pass
//    the raw value), so the generic non-inverting converter matches.
//  * 552-720X2 — battery scene controller; genOnOff/genLevelCtrl commands
//    decode to "action" (no phantom controllable "state", no TZ).
//  * 552-721X2 — double connectable switch; per-endpoint on/off bound on
//    cluster 0x0006 for endpoints 1 and 2.

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

namespace zhc::devices::niko {
extern const PreparedDefinition kDef_D552_80401;   // motion — occupancy (FIX)
extern const PreparedDefinition kDef_D552_72301;   // motor control — cover
extern const PreparedDefinition kDef_D552_720X2;   // 2-button scene controller
extern const PreparedDefinition kDef_D552_721X2;   // double switch
}  // namespace zhc::devices::niko

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

bool def_binds_ep(const PreparedDefinition& def, std::uint8_t ep,
                  std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep &&
            def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}
DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── 3-byte genOnOff cluster-specific command, client->server, no body ─
//   fc(1)=0x01 | tsn(1) | cmd(1)
DispatchResult dispatch_onoff_cmd(const PreparedDefinition& def, std::uint8_t src_ep,
                                  std::uint8_t cmd) {
    const std::array<std::uint8_t, 3> bytes = {0x01, 0x42, cmd};
    auto raw = make_frame(0x0006, src_ep,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── 552-80401: IAS dead-key fix (alarm -> occupancy) ─────────────────
static void test_niko_motion_occupancy() {
    const auto& def = devices::niko::kDef_D552_80401;

    // Expose shape: occupancy present, bare "alarm" gone.
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "alarm"));

    // bit 0 set -> occupancy true, tamper/battery_low false, no "alarm".
    auto occ = dispatch_ias(def, ias_notif(0x0001));
    assert(occ.any_matched);
    assert(b_true(occ.merged.find("occupancy")));
    assert(occ.merged.find("alarm") == nullptr);
    assert(b_false(occ.merged.find("tamper")));
    assert(b_false(occ.merged.find("battery_low")));

    // bit 0 clear -> occupancy false.
    auto clear = dispatch_ias(def, ias_notif(0x0000));
    assert(clear.any_matched);
    assert(b_false(clear.merged.find("occupancy")));

    // bits 2|3 -> tamper + battery_low, occupancy false.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("occupancy")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── 552-72301: cover position on 0x0102, lift-only, raw passthrough ──
static void test_niko_cover_position() {
    const auto& def = devices::niko::kDef_D552_72301;
    assert(def_exposes(def, "position"));
    assert(!def_exposes(def, "tilt"));            // z2m exposes lift only
    assert(def_binds(def, 0x0102));               // closuresWindowCovering

    // attr 0x0008 CurrentPositionLiftPercentage (u8) = 30 -> position 30
    // (raw passthrough; coverInverted+default invert_cover is a net no-op).
    const std::uint8_t val[] = {30};
    auto r = dispatch_zcl(def, 0x0102, "closuresWindowCovering", 1,
                          attr_report(0x0008, 0x20, val));
    assert(r.any_matched);
    const Value* p = r.merged.find("position");
    assert(p && p->type == ValueType::Uint && p->u == 30);
    assert(r.merged.find("tilt") == nullptr);
}

// ── 552-720X2: battery scene controller -> action, not state ─────────
static void test_niko_scene_action() {
    const auto& def = devices::niko::kDef_D552_720X2;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));           // no phantom controllable switch
    assert(def.to_zigbee_count == 0);             // send-only device

    // genOnOff cmd 0x01 (On) from ep1 -> action "on".
    auto r = dispatch_onoff_cmd(def, 1, 0x01);
    assert(r.any_matched);
    const Value* a = r.merged.find("action");
    assert(a && a->type == ValueType::StringRef && a->str
           && std::strcmp(a->str, "on") == 0);
    assert(r.merged.find("state") == nullptr);
}

// ── 552-721X2: double switch, per-endpoint on/off binds ──────────────
static void test_niko_dual_switch() {
    const auto& def = devices::niko::kDef_D552_721X2;
    assert(def_exposes(def, "state"));            // controllable relay(s)
    assert(def_binds_ep(def, 1, 0x0006));
    assert(def_binds_ep(def, 2, 0x0006));
}

int main() {
    test_niko_motion_occupancy();
    test_niko_cover_position();
    test_niko_scene_action();
    test_niko_dual_switch();
    return 0;
}
