// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Hive (British Gas) family. Two real gaps were fixed:
//
//  1. IAS dead-key. The contact (DWS003) and motion (MOT003) sensors, plus
//     the alarm keypad (KEYPAD001), lowered the generic kFzIasZone — which
//     emits the bare keys "alarm_1"/"alarm_2" — while their exposes declared
//     the semantic keys `contact` / `occupancy`. With no rename layer those
//     exposes never reached the shadow. z2m wires the typed
//     fz.ias_contact_alarm_1 / fz.ias_occupancy_alarm_1 decoders; each def
//     was graduated to a Tier-2 override that swaps in the typed
//     kFzIasContactAlarm / kFzIasMotionAlarm converter. KEYPAD001 declares
//     BOTH occupancy and contact (z2m wires both alarm-1 decoders), so it
//     carries both typed converters; its IAS-ACE arm/panic path stays
//     unwired (no generic converter — INFRA).
//
//  2. Thermostat running_state. Every Hive heating receiver (SLR1/1b/1c/1d,
//     SLR2/2b/2c, OTR1) is wired upstream with fz.thermostat, which decodes
//     attr 0x0029 (ThermostatRunningState), and the z2m exposes carry
//     .withRunningState(["idle","heat"]). The generic kFzThermostat decodes
//     only 0x0000/0x0012/0x001C, so running_state was missing entirely.
//     Each receiver was graduated to a Tier-2 override that wires a
//     vendor-local kFzHiveThermostatExtras (attr 0x0029 enum -> string)
//     alongside the generic decoder, and declares the running_state expose.
//
// z2m-source: zigbee-herdsman-converters/src/devices/hive.ts +
//             converters/fromZigbee.ts fz.thermostat /
//             fz.ias_{contact,occupancy}_alarm_1 + lib/constants.ts
//             thermostatRunningStates.

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

namespace zhc::devices::hive {
extern const PreparedDefinition kDef_DWS003;     // contact   (IAS alarm_1)
extern const PreparedDefinition kDef_MOT003;     // occupancy (IAS alarm_1)
extern const PreparedDefinition kDef_KEYPAD001;  // occupancy + contact
extern const PreparedDefinition kDef_SLR1;       // heating receiver
extern const PreparedDefinition kDef_OTR1;       // single-channel receiver
}  // namespace zhc::devices::hive

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool streq(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

// ── IAS Zone Status Change Notification (9 bytes) ─────────────────────
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

// ── ZCL attribute report: fc=0x18, tsn, cmd=0x0A, then records ────────
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v = {0x18, 0x42, 0x0A};
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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, bare "alarm_1"
// absent, tamper/battery_low reflect bits 2/3.
void check_ias_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));   // regression guard vs generic kFzIasZone

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm_1") == nullptr);     // bare key must be gone
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

// ── DWS003 contact + MOT003 occupancy ────────────────────────────────
void check_contact_sensor()  { check_ias_alarm1(devices::hive::kDef_DWS003, "contact"); }
void check_motion_sensor()   { check_ias_alarm1(devices::hive::kDef_MOT003, "occupancy"); }

// ── KEYPAD001: ONE status-change frame fills BOTH occupancy and contact ─
void check_keypad() {
    const auto& def = devices::hive::kDef_KEYPAD001;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "contact"));

    // bit 0 set: both typed decoders read bit 0 -> occupancy AND contact true.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(b_true(on.merged.find("contact")));
    assert(on.merged.find("alarm_1") == nullptr);

    // clear -> both false.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(b_false(off.merged.find("occupancy")));
    assert(b_false(off.merged.find("contact")));

    // tamper(bit2)+battery_low(bit3) preserved through both converters.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── SLR1 / OTR1 receiver: running_state via kFzHiveThermostatExtras,
//    core surface still via generic kFzThermostat. ─────────────────────
void check_receiver(const PreparedDefinition& def) {
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_state"));   // the parity fix

    // attr 0x0029 ThermostatRunningState, map16. 1 -> "heat".
    const std::uint8_t rs_heat[] = {0x01, 0x00};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0029, 0x19, rs_heat));
    assert(rh.any_matched);
    assert(streq(rh.merged.find("running_state"), "heat"));

    // 0 -> "idle".
    const std::uint8_t rs_idle[] = {0x00, 0x00};
    auto ri = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0029, 0x19, rs_idle));
    assert(streq(ri.merged.find("running_state"), "idle"));

    // Regression: heating setpoint still decodes via generic kFzThermostat.
    //   attr 0x0012 OccupiedHeatingSetpoint, s16 = 0x07D0 = 2000 (raw, /100).
    const std::uint8_t sp[] = {0xD0, 0x07};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    // Regression: local_temperature still decodes via generic kFzThermostat.
    //   attr 0x0000 LocalTemperature, s16 = 0x0834 = 2100.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);
}

}  // namespace

int main() {
    check_contact_sensor();
    check_motion_sensor();
    check_keypad();
    check_receiver(devices::hive::kDef_SLR1);
    check_receiver(devices::hive::kDef_OTR1);
    return 0;
}
