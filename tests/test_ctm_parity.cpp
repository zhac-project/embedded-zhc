// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the CTM Lyng (Datek Wireless AS) family. Pins the
// real gaps the auto-generated ports carried against the z2m wire shape:
//
//   * Stove guards mKomfy 2.0 (6254380) + mKomfy 2.5 (mkomfy25) wired
//     the generic `kFzIasZone`, which emits a bare `alarm` (zoneStatus
//     bit 0) that never matched any expose, and dropped both real
//     stove-guard signals. z2m uses `m.iasZoneAlarm({zoneType:"generic",
//     manufacturerZoneAttributes:[{bit:0,"high_temperature"},
//     {bit:1,"power_cut_off"}], zoneAttributes:["tamper","battery_low"]})`
//     → bit 0 = high_temperature, bit 1 = power_cut_off, bit 2 = tamper,
//     bit 3 = battery_low. Fixed with `kFzCtmStoveGuardZone`.
//
//   * AX Water Sensor + AX Valve Controller wired `kFzIasWaterLeakAlarm`
//     (leak on bit 0) and declared a spurious `tamper`. z2m's
//     `fzLocal.ctm_water_leak_alarm` puts the leak on bit 1
//     ("water_leak"), bit 0 = "active_water_leak", bit 3 = "battery_low",
//     and emits no tamper. Fixed with `kFzCtmWaterLeak`.
//
//   * MBD-S + CTM_MBD_Dim are motion detectors whose `occupancy` expose
//     had NO decoder (a stale note claimed msOccupancySensing lacked a
//     generic converter). z2m uses `fz.occupancy`. Fixed by wiring the
//     generic `kFzOccupancy` (msOccupancySensing 0x0406, attr 0x0000).
//
// z2m-source: zigbee-herdsman-converters/src/devices/ctm.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ctm {
extern const PreparedDefinition kDef_mkomfy25;            // 2.5 stove guard
extern const PreparedDefinition kDef_D6254380;            // 2.0 stove guard
extern const PreparedDefinition kDef_AX_Water_Sensor;     // water leak
extern const PreparedDefinition kDef_AX_Valve_Controller; // shutoff valve + leak
extern const PreparedDefinition kDef_MBD_S;               // motion + relay
extern const PreparedDefinition kDef_CTM_MBD_Dim;         // motion + dimmer
}  // namespace zhc::devices::ctm

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

}  // namespace

// ── stove guards: bit 0 high_temperature, bit 1 power_cut_off ────────
static void check_stove_guard(const PreparedDefinition& def) {
    assert(def_exposes(def, "high_temperature"));
    assert(def_exposes(def, "power_cut_off"));
    assert(!def_exposes(def, "alarm"));   // dead generic key must be gone

    // bit 0 only -> high_temperature true, power_cut_off false.
    auto ht = dispatch_ias(def, ias_notif(0x0001));
    assert(ht.any_matched);
    assert(b_true(ht.merged.find("high_temperature")));
    assert(b_false(ht.merged.find("power_cut_off")));
    assert(ht.merged.find("alarm") == nullptr);   // never emit bare alarm

    // bit 1 only -> power_cut_off true, high_temperature false.
    auto pc = dispatch_ias(def, ias_notif(0x0002));
    assert(pc.any_matched);
    assert(b_true(pc.merged.find("power_cut_off")));
    assert(b_false(pc.merged.find("high_temperature")));

    // bits 2+3 -> tamper + battery_low, alarms clear.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find("high_temperature")));
    assert(b_false(tb.merged.find("power_cut_off")));
}

static void test_mkomfy25() { check_stove_guard(devices::ctm::kDef_mkomfy25); }
static void test_d6254380() { check_stove_guard(devices::ctm::kDef_D6254380); }

// ── water-leak family: bit 1 water_leak, bit 0 active_water_leak ─────
static void check_water_leak(const PreparedDefinition& def) {
    assert(def_exposes(def, "water_leak"));
    assert(def_exposes(def, "active_water_leak"));
    assert(!def_exposes(def, "tamper"));   // z2m emits no tamper here

    // bit 1 set -> water_leak true; bit 0 (active) clear.
    auto leak = dispatch_ias(def, ias_notif(0x0002));
    assert(leak.any_matched);
    assert(b_true(leak.merged.find("water_leak")));
    assert(b_false(leak.merged.find("active_water_leak")));
    // Regression guard: the generic kFzIasWaterLeakAlarm read leak from
    // bit 0, so bit-0-only must NOT raise water_leak.
    auto active_only = dispatch_ias(def, ias_notif(0x0001));
    assert(active_only.any_matched);
    assert(b_true(active_only.merged.find("active_water_leak")));
    assert(b_false(active_only.merged.find("water_leak")));

    // bit 3 -> battery_low.
    auto bl = dispatch_ias(def, ias_notif(0x0008));
    assert(bl.any_matched);
    assert(b_true(bl.merged.find("battery_low")));
}

static void test_ax_water() { check_water_leak(devices::ctm::kDef_AX_Water_Sensor); }
static void test_ax_valve() { check_water_leak(devices::ctm::kDef_AX_Valve_Controller); }

// ── MBD motion detectors: occupancy via msOccupancySensing 0x0406 ────
static void check_occupancy(const PreparedDefinition& def) {
    assert(def_exposes(def, "occupancy"));
    // Must bind msOccupancySensing.
    bool binds_occ = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == 0x0406) binds_occ = true;
    assert(binds_occ);

    // Report Attributes: attr 0x0000 (occupancy, bitmap8) bit 0 = occupied.
    const std::uint8_t kOccPresent[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000
        0x18,                    // type bitmap8
        0x01,                    // value 1 = occupied
    };
    auto raw = make_frame(0x0406, 1, kOccPresent);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msOccupancySensing";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(b_true(r.merged.find("occupancy")));

    // value 0 -> unoccupied.
    const std::uint8_t kOccClear[] = {
        0x18, 0x43, 0x0A, 0x00, 0x00, 0x18, 0x00,
    };
    auto raw0 = make_frame(0x0406, 1, kOccClear);
    DecodedMessage msg0{};
    assert(decode_frame(raw0, {}, msg0));
    msg0.cluster = "msOccupancySensing";
    RuntimeContext ctx0{};
    auto r0 = dispatch_from_zigbee(msg0, {}, def, raw0, ctx0);
    assert(r0.any_matched);
    assert(b_false(r0.merged.find("occupancy")));
}

static void test_mbd_s()   { check_occupancy(devices::ctm::kDef_MBD_S); }
static void test_mbd_dim() { check_occupancy(devices::ctm::kDef_CTM_MBD_Dim); }

int main() {
    test_mkomfy25();
    test_d6254380();
    test_ax_water();
    test_ax_valve();
    test_mbd_s();
    test_mbd_dim();
    return 0;
}
