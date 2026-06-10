// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Ynoa (Lytimages) family.
//
// Ynoa ships LED bulbs, two action-only remotes and a metering plug. The
// auto-port had three z2m<->ZHC gaps:
//
//   1. The two remotes (LA-5KEY-RGBW, 8718801528334) were wrong-bundled as
//      settable on/off lights — kFzOnOff + kTzOnOff + a writable `state` —
//      so every key press was dead and they falsely advertised a relay.
//      z2m drives a bound light via genOnOff / genLevelCtrl /
//      lightingColorCtrl cluster commands surfaced on `action`, toZigbee:[].
//      Graduated each def, swapped to the command decoders + an `action`
//      expose, dropped the phantom state + toolbox.
//
//   2. The four bulbs are tunable-white (CCT), one full RGB+CCT, yet the
//      auto-port wired only OnOff + Brightness and dropped the
//      color-temperature axis (and color on the RGBW). Added
//      kFzColorTemperature (+ kFzColor on the RGBW) + the color_temp /
//      color_xy exposes + the lightingColorCtrl (0x0300) binding.
//
//   3. The 5KEY exposes battery_low (z2m e.battery_low()), populated by
//      z2m fz.battery from genPowerCfg batteryAlarmState (0x0035). The
//      generic kFzBattery did not decode 0x0035; extended it (reusable).
//
// The metering plug (LA-PLUG-10Amp) was already CLEAN — z2m
// electricityMeter cluster "both" = seMetering 0x0702 + haElectricalMeasurement
// 0x0B04, and the ez def already wires kFzMetering + kFzElectricalMeasurement
// (energy/power + voltage/current). Pinned here as a regression guard.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ynoa.ts +
//             converters/fromZigbee.ts command_* / battery /
//             electrical_measurement / metering.

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

namespace zhc::devices::ynoa {
extern const PreparedDefinition kDef_LA_5KEY_RGBW;
extern const PreparedDefinition kDef_D8718801528334;
extern const PreparedDefinition kDef_D8718801528204;
extern const PreparedDefinition kDef_D8718801528273;
extern const PreparedDefinition kDef_LA_A60_CCT;
extern const PreparedDefinition kDef_LA_GU10_RGBW;
extern const PreparedDefinition kDef_LA_PLUG_10Amp;
}  // namespace zhc::devices::ynoa

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, std::uint8_t src_ep,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S), tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// ZCL attribute report builder (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool uint_is(const DispatchResult& r, const char* key, std::uint64_t expected) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Uint && v->u == expected;
}

bool bool_is(const DispatchResult& r, const char* key, bool expected) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Bool && v->b == expected;
}

bool has_float(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Float;
}

// ── Remote shape: action expose, NO settable state, empty toolbox. ─────────
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(def.from_zigbee_count > 0);
    assert(!def.endpoint_action_suffix);   // single endpoint → bare action
    assert(def.endpoint_map == nullptr);
}

// ── LA-5KEY-RGBW: on/off + level/CT/color moves → bare action. ─────────────
void check_5key(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "battery_low"));

    // genOnOff On (cmd 0x01) → action = "on".
    RuntimeContext c1{};
    auto on = dispatch_zcl(c1, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched && action_is(on, "action", "on"));

    // genOnOff Off (cmd 0x00) → "off".
    RuntimeContext c2{};
    auto off = dispatch_zcl(c2, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(off.any_matched && action_is(off, "action", "off"));

    // genLevelCtrl MoveToLevel (cmd 0x00): level u8 + tt u16 → "brightness_move_to_level".
    RuntimeContext c3{};
    const std::array<std::uint8_t, 3> lvl = {0x80, 0x00, 0x00};
    auto ml = dispatch_zcl(c3, def, 0x0008, 1, "genLevelCtrl",
                           cmd_frame(0x00, std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    assert(ml.any_matched && action_is(ml, "action", "brightness_move_to_level"));

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A): ct u16 + tt u16 → "color_temperature_move".
    RuntimeContext c4{};
    const std::array<std::uint8_t, 4> ct = {0x9A, 0x01, 0x00, 0x00};
    auto ctm = dispatch_zcl(c4, def, 0x0300, 1, "lightingColorCtrl",
                            cmd_frame(0x0A, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(ctm.any_matched && action_is(ctm, "action", "color_temperature_move"));

    // lightingColorCtrl MoveToColor (cmd 0x07): x u16 + y u16 + tt u16 → "color_move".
    RuntimeContext c5{};
    const std::array<std::uint8_t, 6> cm = {0x00, 0x40, 0x00, 0x40, 0x00, 0x00};
    auto cmv = dispatch_zcl(c5, def, 0x0300, 1, "lightingColorCtrl",
                            cmd_frame(0x07, std::span<const std::uint8_t>(cm.data(), cm.size())));
    assert(cmv.any_matched && action_is(cmv, "action", "color_move"));

    // genPowerCfg battery % (attr 0x0021 = 0xC8 → 100%).
    RuntimeContext c6{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl(c6, def, 0x0001, 1, "genPowerCfg",
                           attr_report(0x0021, 0x20, std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched && uint_is(br, "battery", 100));

    // genPowerCfg batteryAlarmState (attr 0x0035 bitmap32, bit0 set) → battery_low=true.
    RuntimeContext c7{};
    const std::array<std::uint8_t, 4> as = {0x01, 0x00, 0x00, 0x00};
    auto ar = dispatch_zcl(c7, def, 0x0001, 1, "genPowerCfg",
                           attr_report(0x0035, 0x1B, std::span<const std::uint8_t>(as.data(), as.size())));
    assert(ar.any_matched && bool_is(ar, "battery_low", true));

    // batteryAlarmState all-clear → battery_low=false.
    RuntimeContext c8{};
    const std::array<std::uint8_t, 4> as0 = {0x00, 0x00, 0x00, 0x00};
    auto ar0 = dispatch_zcl(c8, def, 0x0001, 1, "genPowerCfg",
                            attr_report(0x0035, 0x1B, std::span<const std::uint8_t>(as0.data(), as0.size())));
    assert(ar0.any_matched && bool_is(ar0, "battery_low", false));

    assert(def_binds(def, 1, 0x0001));   // genPowerCfg bound
}

// ── 8718801528334: one-button dimmer → on/off + move/stop. ─────────────────
void check_dimmer(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));

    RuntimeContext c1{};
    auto on = dispatch_zcl(c1, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched && action_is(on, "action", "on"));

    // genLevelCtrl Move up (cmd 0x01): mode=0, rate=50 → "brightness_move_up".
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};
    auto mu = dispatch_zcl(c2, def, 0x0008, 1, "genLevelCtrl",
                           cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mu.any_matched && action_is(mu, "action", "brightness_move_up"));

    // genLevelCtrl Stop (cmd 0x03) → "brightness_stop".
    RuntimeContext c3{};
    auto st = dispatch_zcl(c3, def, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched && action_is(st, "action", "brightness_stop"));

    assert(def_binds(def, 1, 0x0001));
}

// ── CCT bulb: color_temp expose + decode (lightingColorCtrl attr 0x0007). ──
void check_cct_bulb(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def.to_zigbee_count >= 3);   // OnOff + Brightness + ColorTemp
    assert(def_binds(def, 1, 0x0300));  // lightingColorCtrl bound

    // colorTemperature attr 0x0007 (u16 = 370 mireds) → color_temp = 370.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> v = {0x72, 0x01};  // 0x0172 = 370
    auto r = dispatch_zcl(c1, def, 0x0300, 1, "lightingColorCtrl",
                          attr_report(0x0007, 0x21, std::span<const std::uint8_t>(v.data(), v.size())));
    assert(r.any_matched && uint_is(r, "color_temp", 370));
}

// ── RGBW bulb: CCT + xy color decode. ──────────────────────────────────────
void check_rgbw_bulb(const PreparedDefinition& def) {
    check_cct_bulb(def);
    assert(def_exposes(def, "color_xy"));

    // currentX attr 0x0003 + currentY attr 0x0004 (u16) → color_x / color_y floats.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> x = {0x00, 0x40};  // 0x4000
    auto rx = dispatch_zcl(c1, def, 0x0300, 1, "lightingColorCtrl",
                           attr_report(0x0003, 0x21, std::span<const std::uint8_t>(x.data(), x.size())));
    assert(rx.any_matched && has_float(rx, "color_x"));

    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> y = {0x00, 0x40};
    auto ry = dispatch_zcl(c2, def, 0x0300, 1, "lightingColorCtrl",
                           attr_report(0x0004, 0x21, std::span<const std::uint8_t>(y.data(), y.size())));
    assert(ry.any_matched && has_float(ry, "color_y"));
}

// ── Plug regression guard: 0x0702 + 0x0B04 both decode. ────────────────────
void check_plug(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_binds(def, 1, 0x0702));   // seMetering
    assert(def_binds(def, 1, 0x0B04));   // haElectricalMeasurement

    // haElectricalMeasurement rmsVoltage attr 0x0505 (u16) — 0x0B04 half alive.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> volt = {0xE6, 0x00};  // 230
    auto rv = dispatch_zcl(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                           attr_report(0x0505, 0x21, std::span<const std::uint8_t>(volt.data(), volt.size())));
    assert(rv.any_matched);
    assert(rv.merged.find("voltage") != nullptr);
}

}  // namespace

int main() {
    using namespace zhc::devices::ynoa;
    check_5key(kDef_LA_5KEY_RGBW);
    check_dimmer(kDef_D8718801528334);
    check_cct_bulb(kDef_D8718801528204);
    check_cct_bulb(kDef_D8718801528273);
    check_cct_bulb(kDef_LA_A60_CCT);
    check_rgbw_bulb(kDef_LA_GU10_RGBW);
    check_plug(kDef_LA_PLUG_10Amp);
    return 0;
}
