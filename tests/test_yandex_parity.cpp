// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Yandex (Alice ecosystem) sensor + TRV family.
// z2m-source: yandex.ts
//
// Bugs fixed (vendor-parity sweep):
//   - YNDX-00526 (contact) and YNDX-00527 (leak) lowered the generic
//     kFzIasZone converter, which emits the bare key "alarm". z2m wires
//     m.iasZoneAlarm({zoneType:"contact"/"water_leak"}) emitting the
//     *semantic* key. With no rename layer the primary sensor state never
//     reached the shadow. Graduated to Tier-2 parents using the typed
//     kFzIasContactAlarm / kFzIasWaterLeakAlarm (emit the semantic key).
//   - YNDX-00528 (motion + illuminance) exposed `occupancy` but registered
//     NO occupancy decoder (the def claimed no generic helper existed). The
//     generic kFzOccupancy (msOccupancySensing 0x0000) now exists; wired it.
//   - YNDX-00518 (TRV) decoded only genOnOff + battery and bound no
//     hvacThermostat (0x0201) decoder — the entire thermostat surface was
//     missing. Wired generic kFzThermostat (read) + kTzThermostat (write)
//     and a flat local_temperature / current_heating_setpoint / system_mode
//     surface (project rule: no ExposeType::Climate).
//
// These tests pin, on real wire shapes, that each device now decodes its
// semantic / primary key (and NOT a dead key), and that the TRV binds and
// decodes hvacThermostat.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::yandex {
extern const PreparedDefinition kDef_YNDX_00518;  // TRV (hvacThermostat)
extern const PreparedDefinition kDef_YNDX_00526;  // contact (IAS alarm_1)
extern const PreparedDefinition kDef_YNDX_00527;  // water_leak (IAS alarm_1)
extern const PreparedDefinition kDef_YNDX_00528;  // occupancy (msOccupancySensing)
}  // namespace zhc::devices::yandex

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// ── IAS Zone Status Change Notification (cluster 0x0500) ─────────────
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

// Assert: alarm_1 (bit 0) -> semantic key, "alarm" absent, tamper /
// battery_low reflect bits 2 / 3.
//   invert=false (default, e.g. water_leak): bit0 SET -> key true.
//   invert=true  (zoneType:"contact"): z2m publishes contact = !bit0, so
//     bit0 SET (open) -> contact:false; bit0 CLEAR (closed) -> contact:true.
//   Only the `contact` converter inverts; occupancy/smoke/water_leak/gas/co/
//   vibration stay raw bit0 — pass invert=false for those.
void check_alarm1(const PreparedDefinition& def, const char* sem,
                  bool invert = false) {
    assert(def_exposes(def, sem));            // regression guard vs bare "alarm"

    auto on = dispatch_ias(def, ias_notif(0x0001));   // bit0 SET
    assert(on.any_matched);
    assert((invert ? b_false : b_true)(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // bit0 CLEAR
    assert(off.any_matched);
    assert((invert ? b_true : b_false)(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));  // bit0 clear + tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert((invert ? b_true : b_false)(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

// ── IAS sensors: semantic key, not bare "alarm" ─────────────────────
static void test_contact()    { check_alarm1(devices::yandex::kDef_YNDX_00526, "contact", /*invert=*/true); }
static void test_water_leak() { check_alarm1(devices::yandex::kDef_YNDX_00527, "water_leak"); }

static void test_no_bare_alarm_exposed() {
    // The fix renamed the expose off "alarm"; make sure neither sensor still
    // declares the dead bare key.
    assert(!def_exposes(devices::yandex::kDef_YNDX_00526, "alarm"));
    assert(!def_exposes(devices::yandex::kDef_YNDX_00527, "alarm"));
}

// ── YNDX-00528 — occupancy via msOccupancySensing (NOT IAS) ─────────
static void test_occupancy_cluster() {
    const auto& def = devices::yandex::kDef_YNDX_00528;
    assert(def_exposes(def, "occupancy"));
    assert(def_binds(def, 0x0406));   // msOccupancySensing
    assert(!def_binds(def, 0x0500));  // never an IAS device

    // msOccupancySensing Report Attributes: attr 0x0000 (bitmap8) bit 0.
    const std::uint8_t occ_present[] = {
        0x18, 0x42, 0x0A,  // fc, tsn, cmd=Report Attributes
        0x00, 0x00,        // attr 0x0000
        0x18,              // type bitmap8
        0x01,              // value 1 = occupied
    };
    auto raw = make_frame(0x0406, 1, occ_present);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msOccupancySensing";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(b_true(r.merged.find("occupancy")));

    // cleared
    const std::uint8_t occ_absent[] = {
        0x18, 0x43, 0x0A, 0x00, 0x00, 0x18, 0x00,
    };
    auto raw0 = make_frame(0x0406, 1, occ_absent);
    DecodedMessage msg0{};
    assert(decode_frame(raw0, {}, msg0));
    msg0.cluster = "msOccupancySensing";
    auto r0 = dispatch_from_zigbee(msg0, {}, def, raw0, ctx);
    assert(r0.any_matched);
    assert(b_false(r0.merged.find("occupancy")));
}

// ── YNDX-00518 — flat thermostat surface over hvacThermostat ────────
static void test_trv_thermostat() {
    const auto& def = devices::yandex::kDef_YNDX_00518;

    // Flat exposes (project rule: no ExposeType::Climate).
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "state"));        // genOnOff retained
    assert(def_binds(def, 0x0201));           // hvacThermostat
    // NOTE: surface is flat (project rule). There is no ExposeType::Climate in
    // this codebase; the flat-key asserts above are the parity guarantee.

    // hvacThermostat Report Attributes carrying LocalTemperature (0x0000,
    // s16 = 2150 -> 21.50 C), OccupiedHeatingSetpoint (0x0012, s16 = 2200)
    // and SystemMode (0x001C, enum8 = 4 heat).
    const std::uint8_t therm[] = {
        0x18, 0x42, 0x0A,
        0x00, 0x00, 0x29, 0x66, 0x08,  // attr 0x0000 int16  2150
        0x12, 0x00, 0x29, 0x98, 0x08,  // attr 0x0012 int16  2200
        0x1C, 0x00, 0x30, 0x04,        // attr 0x001C enum8  4
    };
    auto raw = make_frame(0x0201, 1, therm);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);

    const Value* lt = r.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Int && lt->i == 2150);
    const Value* sp = r.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Int && sp->i == 2200);
    const Value* sm = r.merged.find("system_mode");
    assert(sm && sm->type == ValueType::Uint && sm->u == 4);
}

int main() {
    test_contact();
    test_water_leak();
    test_no_bare_alarm_exposed();
    test_occupancy_cluster();
    test_trv_thermostat();
    return 0;
}
