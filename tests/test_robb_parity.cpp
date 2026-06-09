// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for ROBB (ROBB Smarrt / rebranded Sunricher hardware).
// z2m-source: zigbee-herdsman-converters/src/devices/robb.ts.
//
// Bugs fixed (all from the auto-generated port misclassifying the device):
//
//   1. ROB_200-081-0 / ROB_200-084-0 — 4-button WIRELESS wall switches.
//      z2m uses m.commandsOnOff() + m.commandsLevelCtrl() (the device is a
//      genOnOff/genLevelCtrl CLIENT that only SENDS commands). The generator
//      lowered a controllable m.onOff() instead — a writable `state` + kTzOnOff
//      — so the remote looked like an addressable relay and emitted no action.
//      Fixed by wiring the command decoders (on/off + brightness move/stop),
//      exposing `action`, and setting endpoint_action_suffix so each button
//      keeps its identity (action_1..action_4).
//
//   2. ROB_200-070-0 — battery PIR + temperature + humidity + light sensor.
//      The port lowered only kFzBattery + kFzIasZone, dropping occupancy,
//      temperature, humidity AND illuminance. Fixed by adding all four generic
//      decoders + exposes + per-endpoint binds (twin of Sunricher
//      HK-SENSOR-4IN1-A); temp/humidity/lux land on ep3/4/5 → suffixed keys.
//
//   3. ROB_200-017-0 / ROB_200-017-1 — smart plugs with an internal temp
//      sensor (z2m fz.temperature). The port dropped fz.temperature; fixed by
//      adding kFzTemperature + the `temperature` expose + 0x0402 bind.
//
//   4. ROB_200-016-0 (RGB CCT remote) / ROB_200-018-0 (knob dimmer) — color
//      command-mode actions were missing (016) or wired to the wrong decoder
//      (018 used MoveToHueAndSat instead of MoveToColor). The lightingColorCtrl
//      decoders exist now; fixed by wiring move_to_color / move_to_color_temp /
//      move_hue / color_loop_set / enhanced_move_to_hue_and_saturation /
//      move_to_hue_and_saturation as appropriate.
//
//   5. ROB_200-007-0 / ROB_200-008-0 / ROB_200-025-0 — 8-/4-button remotes
//      that emitted a bare `action` (a kAlwaysGlobalKey), collapsing every
//      button onto one key. z2m distinguishes them per endpoint (on_1..
//      brightness_stop_4); fixed by setting endpoint_action_suffix → action_<n>.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <string>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::robb {
extern const PreparedDefinition kDef_ROB_200_081_0;  // 4-button wireless switch
extern const PreparedDefinition kDef_ROB_200_084_0;  // 4-button wireless switch (black)
extern const PreparedDefinition kDef_ROB_200_070_0;  // PIR + temp + humidity + lux
extern const PreparedDefinition kDef_ROB_200_017_0;  // plug w/ temperature
extern const PreparedDefinition kDef_ROB_200_017_1;  // plug w/ temperature
extern const PreparedDefinition kDef_ROB_200_016_0;  // RGB CCT remote
extern const PreparedDefinition kDef_ROB_200_018_0;  // knob dimmer
extern const PreparedDefinition kDef_ROB_200_007_0;  // 8-button remote
extern const PreparedDefinition kDef_ROB_200_008_0;  // 4-button remote
extern const PreparedDefinition kDef_ROB_200_025_0;  // 8-button remote
}  // namespace zhc::devices::robb

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

bool b_true(const Value* v) { return v && v->type == ValueType::Bool && v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// Mirror dispatch.cpp's multi-endpoint key suffixing so we know which shadow
// key a decode on `src_ep` lands under. Single-EP devices keep the bare key.
std::string merged_key(const PreparedDefinition& def, const char* base,
                       std::uint8_t src_ep) {
    if (def.endpoint_map && def.endpoint_map_count > 0) {
        for (std::uint8_t i = 0; i < def.endpoint_map_count; ++i) {
            if (def.endpoint_map[i].endpoint == src_ep && def.endpoint_map[i].label) {
                return std::string(base) + "_" + def.endpoint_map[i].label;
            }
        }
    }
    return std::string(base);
}

// NOTE: multi-endpoint key suffixing allocates the rewritten `<key>_<label>`
// in RuntimeContext::ep_scratch, so the result's key pointers alias `ctx`.
// The caller must keep `ctx` alive for as long as it reads the result —
// hence ctx is passed in by reference, not created locally here.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
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

// ── Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn,
//    cmd, then the command payload. ───────────────────────────────────────
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── genPowerCfg batteryPercentageRemaining (attr 0x0021) → battery%. ──────
void assert_battery(RuntimeContext& ctx, const PreparedDefinition& def) {
    const std::array<std::uint8_t, 1> val = {0xC8};  // 200 -> 100%
    auto r = dispatch_zcl_ep(ctx, def, 0x0001, 1, "genPowerCfg",
                             attr_report(0x0021, 0x20,
                                         std::span<const std::uint8_t>(val.data(), val.size())));
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
}

// ─────────────────────────────────────────────────────────────────────────
// Bug 1: 4-button wireless switches emit `action_<n>`, not a writable `state`.
// ─────────────────────────────────────────────────────────────────────────
void check_wireless_switch(const PreparedDefinition& def) {
    RuntimeContext ctx{};

    // Regression guard: the dead controllable `state` must be gone, the device
    // exposes `action`, writes back nothing (it is a command client), and opts
    // into per-button suffixing.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(def.endpoint_action_suffix);

    // genOnOff On (cmd 0x01) on endpoint 2 → action_2 = "on" (suffixed key).
    auto on2 = dispatch_zcl_ep(ctx, def, 0x0006, 2, "genOnOff", cmd_frame(0x01));
    assert(on2.any_matched);
    assert(action_is(on2, "action_2", "on"));
    // The plain unsuffixed `action` key must never leak through.
    assert(on2.merged.find("action") == nullptr);

    // genOnOff Off (cmd 0x00) on endpoint 1 → action_1 = "off".
    auto off1 = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(off1.any_matched);
    assert(action_is(off1, "action_1", "off"));

    // genLevelCtrl Move (cmd 0x01, mode=up/0 rate=50) on endpoint 3 →
    // "brightness_move_up" under suffixed key action_3.
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};  // mode 0 = up, rate 50
    auto mv3 = dispatch_zcl_ep(ctx, def, 0x0008, 3, "genLevelCtrl",
                               cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mv3.any_matched);
    assert(action_is(mv3, "action_3", "brightness_move_up"));

    assert_battery(ctx, def);
}

// ─────────────────────────────────────────────────────────────────────────
// Bug 2: PIR sensor surfaces occupancy + temperature + humidity + lux.
// ─────────────────────────────────────────────────────────────────────────
void check_pir_sensor(const PreparedDefinition& def) {
    RuntimeContext ctx{};

    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "illuminance"));

    // Occupancy on ep1 (unmapped → bare key).
    const std::array<std::uint8_t, 1> occ = {0x01};
    auto ro = dispatch_zcl_ep(ctx, def, 0x0406, 1, "msOccupancySensing",
                              attr_report(0x0000, 0x18,
                                          std::span<const std::uint8_t>(occ.data(), occ.size())));
    assert(ro.any_matched);
    assert(b_true(ro.merged.find("occupancy")));

    // Temperature on ep3 → temperature_3 (Float, /100 °C; 0x07D0 = 2000 -> 20°C).
    const std::array<std::uint8_t, 2> t = {0xD0, 0x07};
    auto rt = dispatch_zcl_ep(ctx, def, 0x0402, 3, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29,
                                          std::span<const std::uint8_t>(t.data(), t.size())));
    assert(rt.any_matched);
    assert(rt.merged.find(merged_key(def, "temperature", 3).c_str()) != nullptr);

    // Humidity on ep4 → humidity_4 (Float).
    const std::array<std::uint8_t, 2> h = {0xE8, 0x13};  // 5000 -> 50%
    auto rh = dispatch_zcl_ep(ctx, def, 0x0405, 4, "msRelativeHumidity",
                              attr_report(0x0000, 0x21,
                                          std::span<const std::uint8_t>(h.data(), h.size())));
    assert(rh.any_matched);
    assert(rh.merged.find(merged_key(def, "humidity", 4).c_str()) != nullptr);

    // Illuminance on ep5 → illuminance_5 (Uint).
    const std::array<std::uint8_t, 2> lux = {0xE8, 0x03};  // 1000
    auto rl = dispatch_zcl_ep(ctx, def, 0x0400, 5, "msIlluminanceMeasurement",
                              attr_report(0x0000, 0x21,
                                          std::span<const std::uint8_t>(lux.data(), lux.size())));
    assert(rl.any_matched);
    const Value* lv = rl.merged.find(merged_key(def, "illuminance", 5).c_str());
    assert(lv && lv->type == ValueType::Uint && lv->u == 1000);

    // The original IAS path is retained alongside the new sensors: its zone
    // alarm/tamper/battery_low exposes must still be present (kFzIasZone is
    // unchanged by this fix; enrollment is global, so we assert the exposes
    // rather than re-dispatch an enrollment-dependent notification here).
    assert(def_exposes(def, "alarm"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
}

// ─────────────────────────────────────────────────────────────────────────
// Bug 3: metering plug surfaces its internal temperature.
// ─────────────────────────────────────────────────────────────────────────
void check_plug_temperature(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "state"));   // still a controllable relay
    // 0x0402 msTemperatureMeasurement report → temperature (single EP, bare).
    const std::array<std::uint8_t, 2> t = {0xB8, 0x0B};  // 3000 -> 30°C
    auto rt = dispatch_zcl_ep(ctx, def, 0x0402, 1, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29,
                                          std::span<const std::uint8_t>(t.data(), t.size())));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature") != nullptr);
}

// ─────────────────────────────────────────────────────────────────────────
// Bug 4: color command actions reach consumers.
// ─────────────────────────────────────────────────────────────────────────
// lightingColorCtrl MoveToColor (cmd 0x07): colorX u16, colorY u16, transition u16.
DispatchResult move_to_color(RuntimeContext& ctx, const PreparedDefinition& def,
                             std::uint8_t ep = 1) {
    const std::array<std::uint8_t, 6> p = {0x34, 0x12, 0x78, 0x56, 0x0A, 0x00};
    return dispatch_zcl_ep(ctx, def, 0x0300, ep, "lightingColorCtrl",
                           cmd_frame(0x07, std::span<const std::uint8_t>(p.data(), p.size())));
}
// lightingColorCtrl MoveToColorTemp (cmd 0x0A): colorTemp u16, transition u16.
DispatchResult move_to_color_temp(RuntimeContext& ctx, const PreparedDefinition& def,
                                  std::uint8_t ep = 1) {
    const std::array<std::uint8_t, 4> p = {0xFA, 0x00, 0x0A, 0x00};
    return dispatch_zcl_ep(ctx, def, 0x0300, ep, "lightingColorCtrl",
                           cmd_frame(0x0A, std::span<const std::uint8_t>(p.data(), p.size())));
}

void check_rgb_remote(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(def_exposes(def, "action"));

    auto mc = move_to_color(ctx, def);
    assert(mc.any_matched);
    assert(action_is(mc, "action", "color_move"));

    auto mct = move_to_color_temp(ctx, def);
    assert(mct.any_matched);
    assert(action_is(mct, "action", "color_temperature_move"));

    // hue_move (cmd 0x01, mode 1 = move, rate 40).
    const std::array<std::uint8_t, 2> hue = {0x01, 0x28};
    auto mh = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(hue.data(), hue.size())));
    assert(mh.any_matched);
    assert(action_is(mh, "action", "hue_move"));
}

void check_knob_dimmer(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(def_exposes(def, "action"));

    // The swap: command_move_to_color → "color_move" (z2m's color_move_1; no
    // endpoint_map here so the bare key, matching the deprecated-suffix note in
    // the def). Proves kFzCommandMoveToColor is now wired.
    auto mc = move_to_color(ctx, def);
    assert(mc.any_matched);
    assert(action_is(mc, "action", "color_move"));

    auto mct = move_to_color_temp(ctx, def);
    assert(mct.any_matched);
    assert(action_is(mct, "action", "color_temperature_move"));

    // The wrong decoder is gone: a lightingColorCtrl MoveToHueAndSaturation
    // (cmd 0x06) must NOT be claimed (the def no longer wires that converter).
    const std::array<std::uint8_t, 4> hs = {0x80, 0xFE, 0x0A, 0x00};  // hue,sat,transition
    auto mhs = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                               cmd_frame(0x06, std::span<const std::uint8_t>(hs.data(), hs.size())));
    assert(mhs.merged.find("action") == nullptr);
}

// ─────────────────────────────────────────────────────────────────────────
// Bug 5: 8-/4-button remotes (007/008/025) keep per-button identity via
// endpoint_action_suffix → action_<n>, instead of collapsing onto `action`.
// ─────────────────────────────────────────────────────────────────────────
void check_multibutton_remote(const PreparedDefinition& def, std::uint8_t a,
                              std::uint8_t b) {
    RuntimeContext ctx{};
    assert(def.endpoint_action_suffix);
    assert(def_exposes(def, "action"));
    assert(def.to_zigbee_count == 0);

    char key_a[16], key_b[16];
    std::snprintf(key_a, sizeof(key_a), "action_%u", static_cast<unsigned>(a));
    std::snprintf(key_b, sizeof(key_b), "action_%u", static_cast<unsigned>(b));

    // genOnOff On on endpoint a → action_<a> = "on"; the bare key is gone.
    auto on_a = dispatch_zcl_ep(ctx, def, 0x0006, a, "genOnOff", cmd_frame(0x01));
    assert(on_a.any_matched);
    assert(action_is(on_a, key_a, "on"));
    assert(on_a.merged.find("action") == nullptr);

    // genOnOff Off on endpoint b → action_<b> = "off" (distinct key).
    auto off_b = dispatch_zcl_ep(ctx, def, 0x0006, b, "genOnOff", cmd_frame(0x00));
    assert(off_b.any_matched);
    assert(action_is(off_b, key_b, "off"));

    assert_battery(ctx, def);
}

}  // namespace

int main() {
    using namespace zhc::devices::robb;

    // Bug 1 — 4-button wireless wall switches.
    check_wireless_switch(kDef_ROB_200_081_0);
    check_wireless_switch(kDef_ROB_200_084_0);

    // Bug 2 — PIR + temp + humidity + lux sensor.
    check_pir_sensor(kDef_ROB_200_070_0);

    // Bug 3 — metering plugs with internal temperature.
    check_plug_temperature(kDef_ROB_200_017_0);
    check_plug_temperature(kDef_ROB_200_017_1);

    // Bug 4 — color command actions.
    check_rgb_remote(kDef_ROB_200_016_0);
    check_knob_dimmer(kDef_ROB_200_018_0);

    // Bug 5 — multi-button remotes keep per-button identity (action_<n>).
    check_multibutton_remote(kDef_ROB_200_007_0, 1, 4);
    check_multibutton_remote(kDef_ROB_200_008_0, 1, 2);
    check_multibutton_remote(kDef_ROB_200_025_0, 2, 3);

    std::printf("robb parity tests passed\n");
    return 0;
}
