// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for iLightsIn HSSA18-Z-MID (Zhaga module) — a light + multi-
// sensor whose auto-port dropped three whole channels and used a dead IAS key.
//
// z2m extend = [m.light(), m.illuminance(), m.occupancy(),
//   m.iasZoneAlarm({zoneType:"occupancy",
//                   zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]})].
//
// Bugs fixed:
//   * m.illuminance() (msIlluminanceMeasurement 0x0400) was DROPPED entirely.
//   * m.occupancy()   (msOccupancySensing 0x0406) was DROPPED entirely.
//   * The IAS alarm was wired to the generic kFzIasZone (bare key `alarm`,
//     bit 0 only). z2m's iasZoneAlarm with both alarm_1+alarm_2 publishes the
//     SPLIT keys occupancy_alarm_1 (bit 0) + occupancy_alarm_2 (bit 1) +
//     tamper (bit 2) + battery_low (bit 3). Wired the typed converters.
//
// HLC610 (plain m.light()) is on/off + brightness only with no color/CT —
// auto-port is correct; no gap. Asserted as a regression guard.
//
// z2m-source: ilightsin.ts + lib/modernExtend.ts iasZoneAlarm.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/ilightsin_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> ilightsin_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::ilightsin::kIlightsinRegistry,
        devices::ilightsin::kIlightsinRegistryCount);
}

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

InboundApsFrame make_frame(std::uint16_t cluster, std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// 9-byte IAS Zone Status Change Notification (ServerToClient, fc=0x09):
//   fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1) zoneId(1) delay_le(2)
std::array<std::uint8_t, 9> ias_notif(std::uint16_t zone_status) {
    return { 0x09, 0x42, 0x00,
             static_cast<std::uint8_t>(zone_status & 0xFF),
             static_cast<std::uint8_t>(zone_status >> 8),
             0x00, 0x00, 0x00, 0x00 };
}

const PreparedDefinition* hssa18() {
    const auto* d = find_definition("13D", nullptr, ilightsin_registry());
    assert(d && "zigbeeModel 13D must resolve to HSSA18-Z-MID");
    return d;
}

const PreparedDefinition* hlc610() {
    const auto* d = find_definition("053", nullptr, ilightsin_registry());
    assert(d && "zigbeeModel 053 must resolve to HLC610");
    return d;
}

}  // namespace

// HSSA18 exposes the full restored surface; the dead bare `alarm` key is gone.
static void test_hssa18_exposes_declared() {
    const auto* d = hssa18();
    assert(std::strcmp(d->model, "HSSA18-Z-MID") == 0);
    assert(def_exposes(*d, "state"));
    assert(def_exposes(*d, "brightness"));
    assert(def_exposes(*d, "illuminance"));         // dropped by auto-port
    assert(def_exposes(*d, "occupancy"));           // dropped by auto-port
    assert(def_exposes(*d, "occupancy_alarm_1"));   // split IAS key
    assert(def_exposes(*d, "occupancy_alarm_2"));   // split IAS key (bit 1)
    assert(def_exposes(*d, "tamper"));
    assert(def_exposes(*d, "battery_low"));
    // The generic dead key must NOT be exposed.
    assert(!def_exposes(*d, "alarm"));
}

// HSSA18 binds illuminance + occupancy + IAS clusters (auto-port dropped the
// first two).
static void test_hssa18_bindings() {
    const auto* d = hssa18();
    assert(def_binds(*d, 0x0400));  // msIlluminanceMeasurement — was dropped
    assert(def_binds(*d, 0x0406));  // msOccupancySensing       — was dropped
    assert(def_binds(*d, 0x0500));  // ssIasZone
    assert(def_binds(*d, 0x0006));  // genOnOff
    assert(def_binds(*d, 0x0008));  // genLevelCtrl
}

// msIlluminanceMeasurement 0x0400 attr 0x0000 (u16) -> illuminance.
static void test_hssa18_illuminance() {
    const auto* d = hssa18();
    // Report Attributes: fc, tsn, cmd=0x0A, attr 0x0000, type u16 (0x21), value 500.
    const std::uint8_t kIllu[] = { 0x18, 0x42, 0x0A, 0x00, 0x00, 0x21, 0xF4, 0x01 };
    auto r = dispatch_zcl(*d, 0x0400, "msIlluminanceMeasurement", kIllu);
    assert(r.any_matched);
    const Value* lx = r.merged.find("illuminance");
    assert(lx && lx->type == ValueType::Uint && lx->u == 500);
}

// msOccupancySensing 0x0406 attr 0x0000 (bitmap8) bit 0 = occupied.
static void test_hssa18_occupancy() {
    const auto* d = hssa18();
    const std::uint8_t kOcc[]   = { 0x18, 0x42, 0x0A, 0x00, 0x00, 0x18, 0x01 };
    auto r = dispatch_zcl(*d, 0x0406, "msOccupancySensing", kOcc);
    assert(r.any_matched);
    assert(b_true(r.merged.find("occupancy")));

    const std::uint8_t kClear[] = { 0x18, 0x43, 0x0A, 0x00, 0x00, 0x18, 0x00 };
    auto r0 = dispatch_zcl(*d, 0x0406, "msOccupancySensing", kClear);
    assert(r0.any_matched);
    assert(b_false(r0.merged.find("occupancy")));
}

// IAS zoneStatus split: bit 0 -> occupancy_alarm_1, bit 1 -> occupancy_alarm_2,
// bit 2 -> tamper, bit 3 -> battery_low. The generic stub dropped bit 1 and
// used the bare `alarm` key.
static void test_hssa18_ias_split_alarms() {
    const auto* d = hssa18();

    // bit 0 only -> alarm_1 true, alarm_2 false.
    auto a1 = dispatch_zcl(*d, 0x0500, "ssIasZone",
                           std::span<const std::uint8_t>(ias_notif(0x0001)));
    assert(a1.any_matched);
    assert(b_true(a1.merged.find("occupancy_alarm_1")));
    assert(b_false(a1.merged.find("occupancy_alarm_2")));
    assert(a1.merged.find("alarm") == nullptr);   // never emit bare alarm

    // bit 1 only -> alarm_2 true, alarm_1 false (regression: stub dropped bit 1).
    auto a2 = dispatch_zcl(*d, 0x0500, "ssIasZone",
                           std::span<const std::uint8_t>(ias_notif(0x0002)));
    assert(a2.any_matched);
    assert(b_true(a2.merged.find("occupancy_alarm_2")));
    assert(b_false(a2.merged.find("occupancy_alarm_1")));

    // bits 2+3 -> tamper + battery_low, both alarms clear.
    auto tb = dispatch_zcl(*d, 0x0500, "ssIasZone",
                           std::span<const std::uint8_t>(ias_notif(0x000C)));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find("occupancy_alarm_1")));
    assert(b_false(tb.merged.find("occupancy_alarm_2")));
}

// HLC610 regression: plain m.light() = on/off + brightness only, no color/CT.
static void test_hlc610_light_only() {
    const auto* d = hlc610();
    assert(std::strcmp(d->model, "HLC610") == 0);
    assert(def_exposes(*d, "state"));
    assert(def_exposes(*d, "brightness"));
    assert(!def_exposes(*d, "color_temp"));
    assert(!def_exposes(*d, "color_xy"));
    assert(!def_exposes(*d, "color_hs"));
    assert(!def_exposes(*d, "color"));
}

int main() {
    test_hssa18_exposes_declared();
    test_hssa18_bindings();
    test_hssa18_illuminance();
    test_hssa18_occupancy();
    test_hssa18_ias_split_alarms();
    test_hlc610_light_only();
    return 0;
}
