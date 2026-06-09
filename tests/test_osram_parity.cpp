// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Osram (LEDVANCE / Lightify) vendor.
// z2m-source: osram.ts.
//
// Three generated ports were dead and got graduated to Tier-2 overrides:
//
//   * SMART+ Switch Mini (AC0251100NJ/...) and SMART+ Switch
//     (4058075816459) are battery-powered REMOTES. z2m wires the genOnOff /
//     genLevelCtrl / lightingColorCtrl command converters and exposes a
//     single `action` enum. The generated ports lowered kFzOnOff (which
//     decodes the genOnOff command into a phantom `state`) and declared only
//     a `state` expose, so every button press was dead. Re-pointed at the
//     generic cluster-command action converters + an `action` enum.
//
//   * SMART+ Motion Sensor (AC01353010G): z2m wires fz.temperature +
//     fz.ias_occupancy_only_alarm_2. The generated port had no temperature
//     decoder/expose/0x0402 binding (temperature dead) and lowered the
//     generic kFzIasZone (emits bare `alarm`, not the `occupancy` key z2m
//     advertises). Re-pointed at kFzTemperature + kFzIasMotionAlarm2.
//
// These tests pin, on real wire shapes, that the remotes decode `action`
// (and NOT a phantom `state`), and that the motion sensor decodes both
// `temperature` and `occupancy` (and NOT the bare `alarm` key).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::osram {
extern const PreparedDefinition kDef_AC0251100NJ_AC0251600NJ_AC0251700NJ;  // Switch Mini
extern const PreparedDefinition kDef_D4058075816459;                       // Switch 4x
extern const PreparedDefinition kDef_AC01353010G;                          // Motion Sensor
}  // namespace zhc::devices::osram

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster,
                        const char* cluster_name,
                        std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool is_str(const Value* v, const char* want) {
    if (!v || v->type != ValueType::StringRef) return false;
    return v->str && std::strcmp(v->str, want) == 0;
}
bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }

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

// ── ZCL command frame builders (ClientToServer, no manu-specific) ────
// fc(1)=0x01 (cluster-specific, client->server)  tsn(1)  cmd(1)  [payload]
std::array<std::uint8_t, 3> cmd0(std::uint8_t cmd) {
    return {0x01, 0x11, cmd};
}
// genLevelCtrl Move: mode(1) rate(1)
std::array<std::uint8_t, 5> cmd_move(std::uint8_t mode) {
    return {0x01, 0x11, 0x01, mode, 0x32};
}
// lightingColorCtrl MoveToColorTemp (0x0A): colortemp_le(2) transition_le(2)
std::array<std::uint8_t, 7> cmd_move_to_ct() {
    return {0x01, 0x11, 0x0A, 0x90, 0x01, 0x05, 0x00};
}
// lightingColorCtrl StepColorTemp (0x4C): mode(1) step_le(2) trans_le(2) min_le(2) max_le(2)
std::array<std::uint8_t, 12> cmd_step_ct(std::uint8_t mode) {
    return {0x01, 0x11, 0x4C, mode, 0x0A, 0x00, 0x05, 0x00, 0x00, 0x00, 0xFF, 0xFF};
}

}  // namespace

// ── Switch Mini — REMOTE emits `action`, never a phantom `state` ─────
static void test_switch_mini_actions() {
    const auto& def = devices::osram::kDef_AC0251100NJ_AC0251600NJ_AC0251700NJ;
    // Regression guards against the generated kFzOnOff + `state` port.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee == nullptr && def.to_zigbee_count == 0);
    assert(def_binds(def, 0x0006) && def_binds(def, 0x0008) && def_binds(def, 0x0300));

    auto on = dispatch(def, 0x0006, "genOnOff", cmd0(0x01));
    assert(on.any_matched);
    assert(is_str(on.merged.find("action"), "on"));
    assert(on.merged.find("state") == nullptr);   // no phantom on/off state

    auto off = dispatch(def, 0x0006, "genOnOff", cmd0(0x00));
    assert(off.any_matched);
    assert(is_str(off.merged.find("action"), "off"));
    assert(off.merged.find("state") == nullptr);

    auto up = dispatch(def, 0x0008, "genLevelCtrl", cmd_move(0x00));
    assert(up.any_matched);
    assert(is_str(up.merged.find("action"), "brightness_move_up"));

    auto ct = dispatch(def, 0x0300, "lightingColorCtrl", cmd_move_to_ct());
    assert(ct.any_matched);
    assert(is_str(ct.merged.find("action"), "color_temperature_move"));
}

// ── Switch 4x — REMOTE emits `action`, color_temp via Step ───────────
static void test_switch_4x_actions() {
    const auto& def = devices::osram::kDef_D4058075816459;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee == nullptr && def.to_zigbee_count == 0);

    auto on = dispatch(def, 0x0006, "genOnOff", cmd0(0x01));
    assert(on.any_matched);
    assert(is_str(on.merged.find("action"), "on"));
    assert(on.merged.find("state") == nullptr);

    auto step_up = dispatch(def, 0x0300, "lightingColorCtrl", cmd_step_ct(0x01));
    assert(step_up.any_matched);
    assert(is_str(step_up.merged.find("action"), "color_temperature_step_up"));

    auto step_dn = dispatch(def, 0x0300, "lightingColorCtrl", cmd_step_ct(0x03));
    assert(step_dn.any_matched);
    assert(is_str(step_dn.merged.find("action"), "color_temperature_step_down"));

    auto dn = dispatch(def, 0x0008, "genLevelCtrl", cmd_move(0x01));
    assert(dn.any_matched);
    assert(is_str(dn.merged.find("action"), "brightness_move_down"));
}

// ── Motion Sensor — temperature decode + occupancy (not bare alarm) ──
static void test_motion_sensor() {
    const auto& def = devices::osram::kDef_AC01353010G;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "occupancy"));
    assert(!def_exposes(def, "alarm"));            // generic kFzIasZone gone
    assert(def_binds(def, 0x0402));                // msTemperatureMeasurement
    assert(def_binds(def, 0x0500));                // ssIasZone

    // msTemperatureMeasurement Report Attributes: attr 0x0000 (int16),
    // 0x07D0 = 2000 -> 20.00 degC.
    const std::uint8_t temp_report[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x29,                    // type int16
        0xD0, 0x07,              // 2000
    };
    auto t = dispatch(def, 0x0402, "msTemperatureMeasurement", temp_report);
    assert(t.any_matched);
    assert(t.merged.find("temperature") != nullptr);

    // ssIasZone Zone Status Change Notification: zoneStatus bit 1 = occupied
    // (z2m ias_occupancy_only_alarm_2). 9-byte frame.
    const std::uint8_t ias_occ[] = {
        0x09, 0x42, 0x00,        // fc, tsn, cmd=0x00 StatusChangeNotification
        0x02, 0x00,              // zoneStatus = 0x0002 (bit 1 set)
        0x00, 0x00, 0x00, 0x00,  // extStatus, zoneId, delay_le
    };
    auto o = dispatch(def, 0x0500, "ssIasZone", ias_occ);
    assert(o.any_matched);
    assert(b_true(o.merged.find("occupancy")));
    assert(o.merged.find("alarm") == nullptr);     // bare key must be gone
}

int main() {
    test_switch_mini_actions();
    test_switch_4x_actions();
    test_motion_sensor();
    return 0;
}
