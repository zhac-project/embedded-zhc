// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Aurora "AOne" non-light family.
//
// Aurora is overwhelmingly a lighting vendor (m.light(...) bulbs / GU10 /
// filament + dimmer modules) — those ports are CLEAN against z2m and
// exercised by the generic suites. The real z2m<->ZHC gaps were on the
// sensors, the remote and the wireless dimmers:
//
//   * AU-A1ZBPIRS (PIR)     — z2m fz.ias_occupancy_alarm_1 + m.illuminance().
//       Auto-gen wired the generic catch-all kFzIasZone (publishes the
//       un-typed `alarm` bit-0 key, NOT `occupancy`) and dropped the lux
//       channel. Fixed: kFzIasMotionAlarm (→ occupancy) + kFzIlluminance.
//   * AU-A1ZBDWS (contact)  — z2m fz.ias_contact_alarm_1 + fz.battery.
//       Same generic-`alarm` bug. Fixed: kFzIasContactAlarm (→ contact).
//   * AU-A1ZBRC  (remote)   — z2m command_on/off/step/recall/store + battery,
//       toZigbee:[]. Auto-gen wrong-bundled it as a settable on/off light
//       (kFzOnOff + kTzOnOff + writable `state`) and dropped `action`.
//   * AU-A1ZBR1GW (1-gang dimmer, single EP) — z2m batteryRotaryDimmer(1):
//       command_on/off/step/step_color_temperature + battery. Same dead
//       on/off-switch mis-port. Fixed: command-action decoders + bare action.
//   * AU-A1ZBR2GW (2-gang dimmer, multi-EP {right:1, left:2}) — z2m
//       batteryRotaryDimmer(1,2). Action stream was explicitly deferred.
//       Fixed: command decoders + endpoint_action_suffix → action_right /
//       action_left, battery global.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * sensors expose the typed key (occupancy / contact) and NO bare `alarm`,
//   * the PIR surfaces illuminance,
//   * remote + dimmers expose `action`, NO settable `state`, empty toolbox,
//   * genOnOff / genLevelCtrl / lightingColorCtrl / genScenes commands
//     decode to the right `action` labels,
//   * the 2-gang dimmer suffixes per endpoint (action_right / action_left)
//     and keeps battery global (never suffixed).
//
// z2m-source: zigbee-herdsman-converters/src/devices/aurora_lighting.ts
//             #AU-A1ZBPIRS / #AU-A1ZBDWS / #AU-A1ZBRC / #AU-A1ZBR1GW /
//             #AU-A1ZBR2GW + converters/fromZigbee.ts ias_*_alarm_1 /
//             command_* + lib/modernExtend.ts illuminance.

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

namespace zhc::devices::aurora_lighting {
extern const PreparedDefinition kDef_AU_A1ZBPIRS;
extern const PreparedDefinition kDef_AU_A1ZBDWS;
extern const PreparedDefinition kDef_AU_A1ZBRC;
extern const PreparedDefinition kDef_AU_A1ZBR1GW;
extern const PreparedDefinition kDef_AU_A1ZBR2GW;
}  // namespace zhc::devices::aurora_lighting

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

// NOTE: multi-endpoint key suffixing allocates the rewritten `<key>_<label>`
// in RuntimeContext scratch, so result key pointers alias `ctx`. Keep `ctx`
// alive for as long as the result is read.
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

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then the command payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// ssIasZone commandStatusChangeNotification (cmd 0x00): fc=0x09 (C->S,
// cluster-specific, ServerToClient set by direction), tsn, cmd, then
// zoneStatus u16 + extendedStatus u8 + zoneId u8 + delay u16.
std::vector<std::uint8_t> ias_status(std::uint16_t zone_status) {
    return {0x09, 0x42, 0x00,
            static_cast<std::uint8_t>(zone_status & 0xFF),
            static_cast<std::uint8_t>(zone_status >> 8),
            0x00, 0x00, 0x00, 0x00};
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

bool bool_is(const DispatchResult& r, const char* key, bool expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::Bool) return false;
    return v->b == expected;
}

// ── Shared invariants for every remote/dimmer: action expose, NO settable
//    state, empty toolbox (the device cannot be controlled). ──────────────
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // phantom relay must be gone
    assert(def.to_zigbee_count == 0);     // toZigbee: [] — no write path
    assert(def.from_zigbee_count > 0);
}

// ── AU-A1ZBPIRS: PIR → occupancy + illuminance, typed (no bare `alarm`). ──
void check_pir() {
    using namespace zhc::devices::aurora_lighting;
    const auto& def = kDef_AU_A1ZBPIRS;

    // Exposes: occupancy + tamper + battery_low + illuminance; NO `alarm`.
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "illuminance"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(!def_exposes(def, "alarm"));
    // z2m exposes no battery numeric (no fz.battery on this PIR).
    assert(!def_exposes(def, "battery"));

    // IAS zone status: bit 0 set → occupancy true, tamper false.
    RuntimeContext ctx1{};
    auto occ = dispatch_zcl_ep(ctx1, def, 0x0500, 1, "ssIasZone", ias_status(0x0001));
    assert(occ.any_matched);
    assert(bool_is(occ, "occupancy", true));
    assert(bool_is(occ, "tamper", false));
    assert(occ.merged.find("alarm") == nullptr);   // typed, not generic

    // Tamper bit (bit 2) set, motion clear.
    RuntimeContext ctx2{};
    auto tmp = dispatch_zcl_ep(ctx2, def, 0x0500, 1, "ssIasZone", ias_status(0x0004));
    assert(bool_is(tmp, "occupancy", false));
    assert(bool_is(tmp, "tamper", true));

    // Illuminance: msIlluminanceMeasurement attr 0x0000 (u16) = 500.
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 2> lux = {0xF4, 0x01};  // 500
    auto il = dispatch_zcl_ep(ctx3, def, 0x0400, 1, "msIlluminanceMeasurement",
                              attr_report(0x0000, 0x21,
                                          std::span<const std::uint8_t>(lux.data(), lux.size())));
    assert(il.any_matched);
    const Value* lv = il.merged.find("illuminance");
    assert(lv && lv->type == ValueType::Uint && lv->u == 500);

    // Binds: ssIasZone + msIlluminanceMeasurement.
    assert(def_binds(def, 1, 0x0500));
    assert(def_binds(def, 1, 0x0400));
}

// ── AU-A1ZBDWS: contact sensor → contact + battery, typed. ───────────────
void check_contact() {
    using namespace zhc::devices::aurora_lighting;
    const auto& def = kDef_AU_A1ZBDWS;

    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));   // z2m fz.battery → e.battery()
    assert(!def_exposes(def, "alarm"));

    // IAS zone status bit 0 set → contact true.
    RuntimeContext ctx1{};
    auto c = dispatch_zcl_ep(ctx1, def, 0x0500, 1, "ssIasZone", ias_status(0x0001));
    assert(c.any_matched);
    assert(bool_is(c, "contact", true));
    assert(c.merged.find("alarm") == nullptr);

    // battery_low bit (bit 3) set.
    RuntimeContext ctx2{};
    auto bl = dispatch_zcl_ep(ctx2, def, 0x0500, 1, "ssIasZone", ias_status(0x0008));
    assert(bool_is(bl, "battery_low", true));
    assert(bool_is(bl, "contact", false));

    // Battery numeric: genPowerCfg attr 0x0021 = 200 → 100%.
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx3, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);

    assert(def_binds(def, 1, 0x0500));
    assert(def_binds(def, 1, 0x0001));
}

// ── AU-A1ZBRC: single-EP remote → bare action (on/off/step/recall/store). ─
void check_remote() {
    using namespace zhc::devices::aurora_lighting;
    const auto& def = kDef_AU_A1ZBRC;
    check_remote_shape(def);
    assert(!def.endpoint_action_suffix);   // single EP → no suffix
    assert(def.endpoint_map == nullptr);

    RuntimeContext ctx{};
    // genOnOff On (cmd 0x01) → action = "on".
    auto on = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(action_is(on, "action", "on"));
    assert(on.merged.find("action_1") == nullptr);

    // genLevelCtrl Step up (cmd 0x02): mode=0,size=10,tt=0 →
    // action = "brightness_step_up".
    const std::array<std::uint8_t, 4> step = {0x00, 0x0A, 0x00, 0x00};
    auto su = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
    assert(su.any_matched);
    assert(action_is(su, "action", "brightness_step_up"));

    // genScenes Recall scene 1 → action = "recall_1".
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x01};
    auto re = dispatch_zcl_ep(ctx, def, 0x0005, 1, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re.any_matched);
    assert(action_is(re, "action", "recall_1"));

    // genScenes Store scene 1 (cmd 0x04) → action = "store_1".
    auto st = dispatch_zcl_ep(ctx, def, 0x0005, 1, "genScenes",
                              cmd_frame(0x04, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(st.any_matched);
    assert(action_is(st, "action", "store_1"));

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0005));
    assert(def_binds(def, 1, 0x0001));
}

// ── AU-A1ZBR1GW: single-EP rotary dimmer → bare action incl color-temp. ──
void check_dimmer_1g() {
    using namespace zhc::devices::aurora_lighting;
    const auto& def = kDef_AU_A1ZBR1GW;
    check_remote_shape(def);
    assert(!def.endpoint_action_suffix);
    assert(def.endpoint_map == nullptr);

    RuntimeContext ctx{};
    // genOnOff Off (cmd 0x00) → action = "off".
    auto off = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(off.any_matched);
    assert(action_is(off, "action", "off"));

    // lightingColorCtrl StepColorTemperature (cmd 0x4C): step_mode=0x01 (up),
    // step_size u16, transition u16, min u16, max u16 →
    // action = "color_temperature_step_up".
    const std::array<std::uint8_t, 9> sct =
        {0x01, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto cu = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x4C, std::span<const std::uint8_t>(sct.data(), sct.size())));
    assert(cu.any_matched);
    assert(action_is(cu, "action", "color_temperature_step_up"));

    // Battery (genPowerCfg attr 0x0021 = 200 → 100%) stays bare.
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));
    assert(def_binds(def, 1, 0x0001));
}

// ── AU-A1ZBR2GW: multi-EP rotary dimmer → action_right / action_left,
//    battery global. ────────────────────────────────────────────────────
void check_dimmer_2g() {
    using namespace zhc::devices::aurora_lighting;
    const auto& def = kDef_AU_A1ZBR2GW;
    check_remote_shape(def);
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count == 2);

    // On press on EP1 (right) → action_right = "on", no bare `action`.
    RuntimeContext ctx1{};
    auto on1 = dispatch_zcl_ep(ctx1, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on1.any_matched);
    assert(action_is(on1, "action_right", "on"));
    assert(on1.merged.find("action") == nullptr);

    // Step-up on EP2 (left) → action_left = "brightness_step_up".
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 4> step = {0x00, 0x0A, 0x00, 0x00};
    auto su2 = dispatch_zcl_ep(ctx2, def, 0x0008, 2, "genLevelCtrl",
                               cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
    assert(su2.any_matched);
    assert(action_is(su2, "action_left", "brightness_step_up"));
    assert(su2.merged.find("action") == nullptr);
    assert(su2.merged.find("action_right") == nullptr);

    // color_temperature_step_down on EP1 → action_right.
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 9> sct =
        {0x03, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto cd = dispatch_zcl_ep(ctx3, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x4C, std::span<const std::uint8_t>(sct.data(), sct.size())));
    assert(cd.any_matched);
    assert(action_is(cd, "action_right", "color_temperature_step_down"));

    // Battery is reported on EP1 only and must stay GLOBAL (never suffixed).
    RuntimeContext ctx4{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx4, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
    assert(br.merged.find("battery_right") == nullptr);   // never suffixed

    // Binds: command clusters on both EP1 and EP2; battery on EP1 only.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 2, 0x0006));
    assert(def_binds(def, 2, 0x0008));
    assert(def_binds(def, 2, 0x0300));
    assert(def_binds(def, 1, 0x0001));
}

}  // namespace

int main() {
    check_pir();
    check_contact();
    check_remote();
    check_dimmer_1g();
    check_dimmer_2g();
    return 0;
}
