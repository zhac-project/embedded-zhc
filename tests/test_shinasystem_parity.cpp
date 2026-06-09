// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the ShinaSystem (SiHAS) sensor / gas-valve family.
// z2m-source: shinasystem.ts.
//
// Bugs fixed (generator misses), pinned here on real wire shapes:
//
//  * USM-300ZB — z2m `fromZigbee:[…fz.occupancy]` + `extend:[m.illuminance]`
//    was lowered to battery+temp+humidity only. The `occupancy` expose had
//    no decoder and the illuminance channel was dropped entirely. Re-added
//    kFzOccupancy + kFzIlluminance (+ illuminance expose + 0x0400 binding).
//
//  * OSM-300ZB — z2m `extend:[m.occupancy(), m.battery()]` lowered to
//    battery only; `occupancy` expose present but no decoder. Re-added
//    kFzOccupancy (msOccupancySensing, like heiman HS1MIS-3.0).
//
//  * DMS-300ZB — dual PIR. z2m reads `occupancy_in` (msOccupancySensing) +
//    `occupancy_out` (ssIasZone bit0); the def was battery-only (both
//    channels dead). Re-added the two stateless channel decoders. The
//    derived occupancy_or/_and (cross-message state) + occupancy_timeout
//    stay deferred as infra.
//
//  * GCM-300Z gas valve — two bugs: `to_zigbee` was left null (every write
//    dead), and the valve rode the generic on/off `state` key while the
//    expose is `gas_valve_state` ("OPEN"/"CLOSE"). Wired to_zigbee and
//    swapped in kFzSihasGasValveState / kTzSihasGasValveState.
//
// These assert the decoded keys reach the shadow (and the bare "state"
// key does NOT), and that the valve write encodes the right onOff command.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::shinasystem {
extern const PreparedDefinition kDef_USM_300ZB;
extern const PreparedDefinition kDef_OSM_300ZB;
extern const PreparedDefinition kDef_DMS_300ZB;
extern const PreparedDefinition kDef_GCM_300Z;
}  // namespace zhc::devices::shinasystem

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

// Dispatch a decoded ZCL frame through a def's from_zigbee chain.
DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster,
                            std::uint8_t src_ep, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool is_str(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
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

// Report-Attributes frame: fc=0x18, tsn, cmd=0x0A, attr(LE u16), type, value…
// msOccupancySensing 0x0000 (bitmap8) and genOnOff 0x0000 (bool) both fit.
std::array<std::uint8_t, 7> attr_report_u8(std::uint16_t attr, std::uint8_t type,
                                           std::uint8_t value) {
    return { 0x18, 0x42, 0x0A,
             static_cast<std::uint8_t>(attr & 0xFF),
             static_cast<std::uint8_t>(attr >> 8),
             type, value };
}

// genAnalogInput / illuminance u16 attr report (8 bytes): value LE.
std::array<std::uint8_t, 8> attr_report_u16(std::uint16_t attr, std::uint8_t type,
                                            std::uint16_t value) {
    return { 0x18, 0x42, 0x0A,
             static_cast<std::uint8_t>(attr & 0xFF),
             static_cast<std::uint8_t>(attr >> 8),
             type,
             static_cast<std::uint8_t>(value & 0xFF),
             static_cast<std::uint8_t>(value >> 8) };
}

// 9-byte IAS Zone Status Change Notification (cmd 0x00).
std::array<std::uint8_t, 9> ias_notif(std::uint16_t zone_status) {
    return { 0x09, 0x42, 0x00,
             static_cast<std::uint8_t>(zone_status & 0xFF),
             static_cast<std::uint8_t>(zone_status >> 8),
             0x00, 0x00, 0x00, 0x00 };
}

}  // namespace

// ── USM-300ZB — occupancy + illuminance decoders restored ────────────
static void test_usm_occupancy_and_illuminance() {
    const auto& def = devices::shinasystem::kDef_USM_300ZB;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "illuminance"));
    assert(def_binds(def, 0x0406));   // msOccupancySensing
    assert(def_binds(def, 0x0400));   // msIlluminanceMeasurement

    // occupancy (msOccupancySensing 0x0000 bitmap8 bit0 = occupied).
    auto occ = attr_report_u8(0x0000, 0x18, 0x01);
    auto ro = dispatch_zcl(def, 0x0406, 1, "msOccupancySensing",
                           {occ.data(), occ.size()});
    assert(ro.any_matched);
    assert(b_true(ro.merged.find("occupancy")));

    auto occ0 = attr_report_u8(0x0000, 0x18, 0x00);
    auto rc = dispatch_zcl(def, 0x0406, 1, "msOccupancySensing",
                           {occ0.data(), occ0.size()});
    assert(rc.any_matched);
    assert(b_false(rc.merged.find("occupancy")));

    // illuminance (msIlluminanceMeasurement 0x0000 u16).
    auto lux = attr_report_u16(0x0000, 0x21, 1234);
    auto rl = dispatch_zcl(def, 0x0400, 1, "msIlluminanceMeasurement",
                           {lux.data(), lux.size()});
    assert(rl.any_matched);
    const Value* v = rl.merged.find("illuminance");
    assert(v && v->type == ValueType::Uint && v->u == 1234);
}

// ── OSM-300ZB — occupancy via msOccupancySensing ─────────────────────
static void test_osm_occupancy() {
    const auto& def = devices::shinasystem::kDef_OSM_300ZB;
    assert(def_exposes(def, "occupancy"));
    assert(def_binds(def, 0x0406));
    assert(!def_binds(def, 0x0500));   // not an IAS device

    auto occ = attr_report_u8(0x0000, 0x18, 0x01);
    auto r = dispatch_zcl(def, 0x0406, 1, "msOccupancySensing",
                          {occ.data(), occ.size()});
    assert(r.any_matched);
    assert(b_true(r.merged.find("occupancy")));
}

// ── DMS-300ZB — dual PIR: occupancy_in (msOccup) + occupancy_out (IAS) ─
static void test_dms_dual_motion() {
    const auto& def = devices::shinasystem::kDef_DMS_300ZB;
    assert(def_exposes(def, "occupancy_in"));
    assert(def_exposes(def, "occupancy_out"));
    assert(!def_exposes(def, "occupancy"));   // phantom key removed

    // IN sensor: msOccupancySensing on ep1.
    auto in1 = attr_report_u8(0x0000, 0x18, 0x01);
    auto ri = dispatch_zcl(def, 0x0406, 1, "msOccupancySensing",
                           {in1.data(), in1.size()});
    assert(ri.any_matched);
    assert(b_true(ri.merged.find("occupancy_in")));
    assert(ri.merged.find("occupancy") == nullptr);   // no bare/generic key

    auto in0 = attr_report_u8(0x0000, 0x18, 0x00);
    auto ri0 = dispatch_zcl(def, 0x0406, 1, "msOccupancySensing",
                            {in0.data(), in0.size()});
    assert(b_false(ri0.merged.find("occupancy_in")));

    // OUT sensor: ssIasZone status-change, zoneStatus bit0.
    auto out1 = ias_notif(0x0001);
    auto ro = dispatch_zcl(def, 0x0500, 1, "ssIasZone",
                           {out1.data(), out1.size()});
    assert(ro.any_matched);
    assert(b_true(ro.merged.find("occupancy_out")));
    // The IN decoder must not also fire on an IAS frame.
    assert(ro.merged.find("occupancy_in") == nullptr);

    auto out0 = ias_notif(0x0000);
    auto ro0 = dispatch_zcl(def, 0x0500, 1, "ssIasZone",
                            {out0.data(), out0.size()});
    assert(b_false(ro0.merged.find("occupancy_out")));
}

// ── GCM-300Z — gas_valve_state read + write ──────────────────────────
static void test_gcm_gas_valve() {
    const auto& def = devices::shinasystem::kDef_GCM_300Z;
    assert(def_exposes(def, "gas_valve_state"));
    assert(def.to_zigbee != nullptr && def.to_zigbee_count > 0);   // wiring bug

    // Read: genOnOff 0x0000 (bool) 1 -> "OPEN", 0 -> "CLOSE", keyed
    // gas_valve_state (NOT the generic "state").
    auto on = attr_report_u8(0x0000, 0x10, 0x01);
    auto ron = dispatch_zcl(def, 0x0006, 1, "genOnOff", {on.data(), on.size()});
    assert(ron.any_matched);
    assert(is_str(ron.merged.find("gas_valve_state"), "OPEN"));
    assert(ron.merged.find("state") == nullptr);

    auto off = attr_report_u8(0x0000, 0x10, 0x00);
    auto roff = dispatch_zcl(def, 0x0006, 1, "genOnOff", {off.data(), off.size()});
    assert(is_str(roff.merged.find("gas_valve_state"), "CLOSE"));

    // Write: "OPEN" -> onOff command 0x01; "CLOSE" -> 0x00.
    RuntimeContext ctx{};
    std::array<std::uint8_t, 16> frame{};
    Value vo{}; vo.type = ValueType::StringRef; vo.str = "OPEN";
    auto wo = dispatch_to_zigbee(def, "gas_valve_state", vo, ctx,
                                 {frame.data(), frame.size()});
    assert(wo.ok && wo.cluster_id == 0x0006 && wo.frame_size == 3 && frame[2] == 0x01);

    Value vc{}; vc.type = ValueType::StringRef; vc.str = "CLOSE";
    auto wc = dispatch_to_zigbee(def, "gas_valve_state", vc, ctx,
                                 {frame.data(), frame.size()});
    assert(wc.ok && wc.frame_size == 3 && frame[2] == 0x00);

    // And a config write (close_timeout) now reaches a converter too.
    Value vt{}; vt.type = ValueType::Uint; vt.u = 5;
    auto wt = dispatch_to_zigbee(def, "close_timeout", vt, ctx,
                                 {frame.data(), frame.size()});
    assert(wt.ok && wt.cluster_id == 0x0006);
}

int main() {
    test_usm_occupancy_and_illuminance();
    test_osm_occupancy();
    test_dms_dual_motion();
    test_gcm_gas_valve();
    return 0;
}
