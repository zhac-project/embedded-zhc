// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Bosch Smart Home family. Pins the real gaps the
// auto-generated / staged ports carried against the z2m wire shape:
//
//   * A whole row of NON-light devices were mis-classed onto the Bosch
//     "Light" bundle ({state,brightness,color_temp} + onOff/level/color
//     converters) and therefore decoded NOTHING the device actually
//     sends while advertising phantom brightness/color_temp:
//       - BSEN-CV / BSEN-C2D  (door/window contacts)
//       - BSEN-W              (water alarm)
//       - BSP-FD              (smart plug)
//       - BTH-RM / BTH-RM230Z (room thermostats)
//       - BSIR-EZ             (outdoor siren)
//     Re-targeted to the correct bosch/_shared bundle per device.
//
//   * The IAS security sensors (BSEN-C2/CV/C2D contacts, BSEN-M motion,
//     BSD-2 smoke, BSEN-W water, ISW-ZPR1-WP13 + RADION TriTech motion)
//     wired the generic `kFzIasZone`, which emits a bare `alarm`
//     (zoneStatus bit 0) that the SPA cannot map to a device class. z2m
//     emits the *semantic* key for the zone type: contact / occupancy /
//     smoke / water_leak. Fixed with the typed bundles kFzBoschContact /
//     kFzBoschMotion / kFzBoschSmoke / kFzBoschWaterLeak (bit 0 → semantic
//     key, bit 2 → tamper, bit 3 → battery_low).
//
// Deferred (INFRA, manuSpec custom cluster, no generic converter):
// Twinguard (8750001213, 0xE000 air-quality+smoke), BSIR-EZ siren command
// surface (Bosch alarmControl), BMCT-* manuSpec switch modes. See
// docs/BOSCH_PARITY.md.
//
// z2m-source: zigbee-herdsman-converters/src/devices/bosch.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::bosch {
extern const PreparedDefinition kDef_BSEN_C2;   // door/window contact
extern const PreparedDefinition kDef_BSEN_CV;   // door/window contact plus
extern const PreparedDefinition kDef_BSEN_C2D;  // door/window contact plus(+vib)
extern const PreparedDefinition kDef_BSEN_M;    // motion detector
extern const PreparedDefinition kDef_BSD_2;     // smoke alarm
extern const PreparedDefinition kDef_BSEN_W;    // water alarm
extern const PreparedDefinition kDef_BSIR_EZ;   // outdoor siren
extern const PreparedDefinition kDef_BSP_FD;    // smart plug
extern const PreparedDefinition kDef_BTH_RM;    // room thermostat (battery)
extern const PreparedDefinition kDef_BTH_RM230Z;// room thermostat (230V)
extern const PreparedDefinition kDef_ISW_ZPR1_WP13;   // motion
extern const PreparedDefinition kDef_RADION_TriTech_ZB;// motion
}  // namespace zhc::devices::bosch

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// Generic check for a typed IAS sensor: the semantic key fires on
// zoneStatus bit 0, the dead generic `alarm` key is gone, and tamper /
// battery_low ride bits 2 / 3.
void check_typed_ias(const PreparedDefinition& def, const char* sem_key) {
    assert(def_exposes(def, sem_key));
    assert(!def_exposes(def, "alarm"));   // dead generic key must be gone
    assert(def_binds(def, 0x0500));       // ssIasZone
    assert(def_binds(def, 0x0001));       // genPowerCfg (battery)

    // bit 0 set -> semantic key true; never a bare `alarm`.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem_key)));
    assert(on.merged.find("alarm") == nullptr);

    // bit 0 clear -> semantic key false.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem_key)));

    // bits 2+3 -> tamper + battery_low, semantic key clear.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find(sem_key)));
}

}  // namespace

// ── contacts: zoneStatus bit 0 -> `contact` (not bare `alarm`) ───────
static void test_contact_c2()  { check_typed_ias(devices::bosch::kDef_BSEN_C2,  "contact"); }
static void test_contact_cv()  { check_typed_ias(devices::bosch::kDef_BSEN_CV,  "contact"); }
static void test_contact_c2d() { check_typed_ias(devices::bosch::kDef_BSEN_C2D, "contact"); }

// ── motion: zoneStatus bit 0 -> `occupancy` ──────────────────────────
static void test_motion_m()    { check_typed_ias(devices::bosch::kDef_BSEN_M,            "occupancy"); }
static void test_motion_isw()  { check_typed_ias(devices::bosch::kDef_ISW_ZPR1_WP13,     "occupancy"); }
static void test_motion_rad()  { check_typed_ias(devices::bosch::kDef_RADION_TriTech_ZB, "occupancy"); }

// ── smoke: zoneStatus bit 0 -> `smoke` ───────────────────────────────
static void test_smoke()       { check_typed_ias(devices::bosch::kDef_BSD_2, "smoke"); }

// ── water: zoneStatus bit 0 -> `water_leak` ──────────────────────────
static void test_water()       { check_typed_ias(devices::bosch::kDef_BSEN_W, "water_leak"); }

// ── siren: kept on IAS-zone state (alarm), no phantom light ──────────
static void test_siren() {
    const auto& def = devices::bosch::kDef_BSIR_EZ;
    // Must no longer be a light.
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    // `alarm` is the correct semantic key for a siren's alarm-active bit.
    assert(def_exposes(def, "alarm"));
    assert(def_binds(def, 0x0500));

    // The siren rides the generic kFzIasZone, which reads the ZoneStatus
    // *attribute* (0x0002), not the cmd-0x00 notification. Feed a Report
    // Attributes frame: attr 0x0002 (u16) bit 0 set -> alarm.
    const std::uint8_t kZoneStatus[] = {
        0x18, 0x10, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x02, 0x00, 0x21,        // attr 0x0002, type uint16
        0x01, 0x00,             // value 0x0001 (bit 0 = alarm)
    };
    auto raw = make_frame(0x0500, 1, kZoneStatus);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    auto on = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(on.any_matched);
    assert(b_true(on.merged.find("alarm")));
}

// ── smart plug: state + power/energy, binds onOff + metering ─────────
static void test_plug() {
    const auto& def = devices::bosch::kDef_BSP_FD;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(!def_exposes(def, "brightness"));   // no longer a light
    assert(!def_exposes(def, "color_temp"));
    assert(def_binds(def, 0x0006));            // genOnOff
    assert(def_binds(def, 0x0702));            // seMetering

    // genOnOff attr 0x0000 (OnOff bool) = 1 -> state ON.
    const std::uint8_t kOn[] = {
        0x18, 0x10, 0x0A,  // fc, tsn, cmd=Report Attributes
        0x00, 0x00,        // attr 0x0000
        0x10,              // type bool
        0x01,              // value 1
    };
    auto raw = make_frame(0x0006, 1, kOn);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* st = r.merged.find("state");
    assert(st != nullptr);   // decoded (generic kFzOnOff)
}

// ── room thermostats: flat local_temperature / system_mode decode ────
static void check_thermostat(const PreparedDefinition& def) {
    // Flat thermostat surface (project rule — no ExposeType::Climate).
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(!def_exposes(def, "brightness"));   // no longer a light
    assert(!def_exposes(def, "color_temp"));
    assert(def_binds(def, 0x0201));            // hvacThermostat

    // hvacThermostat Report Attributes:
    //   attr 0x0000 LocalTemperature (s16) = 2150 (=21.50 C)
    //   attr 0x001C SystemMode (u8 enum)   = 4   (heat)
    const std::uint8_t kReport[] = {
        0x18, 0x20, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00, 0x29,        // attr 0x0000, type int16
        0x66, 0x08,             // 2150 LE
        0x1C, 0x00, 0x30,        // attr 0x001C, type enum8
        0x04,                    // heat
    };
    auto raw = make_frame(0x0201, 1, kReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* lt = r.merged.find("local_temperature");
    assert(lt != nullptr);
    assert((lt->type == ValueType::Int  && lt->i == 2150) ||
           (lt->type == ValueType::Uint && lt->u == 2150));
    const Value* sm = r.merged.find("system_mode");
    assert(sm != nullptr);
}

static void test_thermostat_rm()    { check_thermostat(devices::bosch::kDef_BTH_RM); }
static void test_thermostat_rm230() { check_thermostat(devices::bosch::kDef_BTH_RM230Z); }

int main() {
    test_contact_c2();
    test_contact_cv();
    test_contact_c2d();
    test_motion_m();
    test_motion_isw();
    test_motion_rad();
    test_smoke();
    test_water();
    test_siren();
    test_plug();
    test_thermostat_rm();
    test_thermostat_rm230();
    return 0;
}
