// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the RGB Genie scene/rotary remote family.
//
// RGB Genie is one m.light() dimmer (ZB-1026) plus seven battery- or
// mains-powered scene/colour remotes. The remotes are senders (clients of
// genOnOff / genLevelCtrl / lightingColorCtrl / genScenes) that emit
// `action` events — NOT light sinks. The auto-generator collapsed every
// device to "state + brightness + kFzOnOff/kTzOnOff" (a settable on/off
// light), which is the wrong shape for a sender. That was already corrected
// to the action+battery / action+colour bundles in
// definitions/rgb_genie/_shared.{hpp,cpp} + the Tier-2 generated defs.
//
// This suite pins the corrected wiring at the dispatch_from_zigbee boundary
// and, crucially, LOCKS the ColorCtrl move-style decoders that earlier port
// comments wrongly described as "not yet wired in _generic":
//
//   * fz.command_move_hue              (lightingColorCtrl cmd 0x01)
//       → action = "hue_move" / "hue_down" / "hue_stop" + action_rate
//   * fz.command_move_to_color         (lightingColorCtrl cmd 0x07)
//       → action = "color_move" + action_color_x/y
//   * fz.command_move_color_temperature(lightingColorCtrl cmd 0x4B)
//       → action = "color_temperature_move_up/_down/_stop"
//
// These ARE wired today (kFzCommandMoveHue / kFzCommandMoveToColor /
// kFzCommandMoveColorTemperature in _generic, pulled into
// kFzActionBatteryRGB + kFzActionRGBNoBattery). The stale "PARTIAL" notes
// were removed; this test is the regression guard so they cannot silently
// fall out of the bundle.
//
// Also pinned: remote shape (action expose, no settable state, empty
// toolbox), single-EP scene recall, multi-EP endpoint suffixing for ZB-3008
// (3 EP) and ZB-5028 (4 EP), and battery decode on the battery models.
//
// z2m-source: zigbee-herdsman-converters/src/devices/rgb_genie.ts
//             + converters/fromZigbee.ts command_* / battery
//             + lib/modernExtend.ts light (ZB-1026).

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

namespace zhc::devices::rgb_genie {
extern const PreparedDefinition kDef_ZB_1026;   // m.light() dimmer (sink)
extern const PreparedDefinition kDef_ZB_5001;   // action + battery
extern const PreparedDefinition kDef_ZB_5004;   // action + battery
extern const PreparedDefinition kDef_ZB_5121;   // action + battery + step
extern const PreparedDefinition kDef_ZB_5122;   // action + battery + colour
extern const PreparedDefinition kDef_ZB_3008;   // mains, 3-EP colour scene
extern const PreparedDefinition kDef_ZB_3009;   // mains, 1-EP colour scene
extern const PreparedDefinition kDef_ZB_5028;   // battery, 4-EP colour scene
}  // namespace zhc::devices::rgb_genie

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
    if (!v || v->type != ValueType::Uint) return false;
    return v->u == expected;
}

// ── Shared invariant for every remote: action expose, NO settable state,
//    empty toolbox (the device cannot be controlled). ──────────────────────
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));        // phantom relay must be gone
    assert(!def_exposes(def, "brightness"));   // not a light
    assert(def.to_zigbee_count == 0);          // toZigbee: [] — no write path
    assert(def.from_zigbee_count > 0);
}

// ── ZB-1026: the lone real light (sink), NOT an action device. ────────────
void check_light() {
    using namespace zhc::devices::rgb_genie;
    const auto& def = kDef_ZB_1026;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "action"));
    assert(def.to_zigbee_count > 0);   // controllable light has a write path

    // genOnOff onOff attr report 1 → state true (it is a sink: generic
    // kFzOnOff lowers onOff to a Bool `state`, NOT an action string).
    RuntimeContext ctx{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto st = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(on.data(), on.size())));
    assert(st.any_matched);
    const Value* sv = st.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && sv->b);
}

// ── ZB-5001 / ZB-5004: single-EP action + battery remotes. ────────────────
void check_battery_remote(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));
    assert(def.endpoint_map == nullptr);   // single EP → no suffix

    RuntimeContext ctx{};
    // genOnOff On (cmd 0x01) → action = "on".
    auto on = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(action_is(on, "action", "on"));
    assert(on.merged.find("action_1") == nullptr);

    // genOnOff Off (cmd 0x00) → action = "off".
    auto off = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(action_is(off, "action", "off"));

    // genLevelCtrl Move up (cmd 0x01): mode=0, rate=50 → "brightness_move_up".
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};
    auto mu = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mu.any_matched);
    assert(action_is(mu, "action", "brightness_move_up"));

    // genLevelCtrl Stop (cmd 0x03) → "brightness_stop".
    auto sp = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03));
    assert(action_is(sp, "action", "brightness_stop"));

    // genScenes Recall scene 2 → action = "recall_2".
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x02};
    auto re = dispatch_zcl_ep(ctx, def, 0x0005, 1, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(action_is(re, "action", "recall_2"));

    // Battery: genPowerCfg attr 0x0021 = 200 → 100%.
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    assert(uint_is(br, "battery", 100));

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0001));
}

// ── ZB-5121: action + battery + brightness step. ──────────────────────────
void check_5121() {
    using namespace zhc::devices::rgb_genie;
    const auto& def = kDef_ZB_5121;
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));

    RuntimeContext ctx{};
    // genLevelCtrl Step up (cmd 0x02): mode=0, size=10, tt=0 →
    // action = "brightness_step_up".
    const std::array<std::uint8_t, 4> step = {0x00, 0x0A, 0x00, 0x00};
    auto su = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
    assert(su.any_matched);
    assert(action_is(su, "action", "brightness_step_up"));

    // Step down (mode=1) → "brightness_step_down".
    const std::array<std::uint8_t, 4> stepd = {0x01, 0x0A, 0x00, 0x00};
    auto sd = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(stepd.data(), stepd.size())));
    assert(action_is(sd, "action", "brightness_step_down"));
}

// ── ColorCtrl move-style decoders. THE regression guard for the formerly
//    "PARTIAL / not yet wired" claim — exercised on a battery (ZB-5122) and
//    a mains (ZB-3009) colour remote so both bundles are covered. ──────────
void check_color_move(const PreparedDefinition& def) {
    RuntimeContext ctx{};

    // command_move_hue (cmd 0x01): move_mode=1 (move), rate=20 → "hue_move".
    const std::array<std::uint8_t, 2> mh = {0x01, 0x14};
    auto hm = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(mh.data(), mh.size())));
    assert(hm.any_matched);
    assert(action_is(hm, "action", "hue_move"));
    assert(uint_is(hm, "action_rate", 20));

    // command_move_hue move_mode=0 → "hue_stop".
    const std::array<std::uint8_t, 2> mhs = {0x00, 0x00};
    auto hs = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(mhs.data(), mhs.size())));
    assert(action_is(hs, "action", "hue_stop"));

    // command_move_to_color (cmd 0x07): colorx=0x1234, colory=0x5678, tt=0x000A
    // → action = "color_move" + raw action_color_x/y.
    const std::array<std::uint8_t, 6> mc = {0x34, 0x12, 0x78, 0x56, 0x0A, 0x00};
    auto cm = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x07, std::span<const std::uint8_t>(mc.data(), mc.size())));
    assert(cm.any_matched);
    assert(action_is(cm, "action", "color_move"));
    assert(uint_is(cm, "action_color_x", 0x1234));
    assert(uint_is(cm, "action_color_y", 0x5678));

    // command_move_color_temperature (cmd 0x4B): move_mode=1 (up), rate=5,
    // min=153, max=500 → "color_temperature_move_up".
    const std::array<std::uint8_t, 7> mct =
        {0x01, 0x05, 0x00, 0x99, 0x00, 0xF4, 0x01};
    auto ctm = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                               cmd_frame(0x4B, std::span<const std::uint8_t>(mct.data(), mct.size())));
    assert(ctm.any_matched);
    assert(action_is(ctm, "action", "color_temperature_move_up"));
    assert(uint_is(ctm, "action_rate", 5));

    // move_mode=3 (down) → "color_temperature_move_down".
    const std::array<std::uint8_t, 7> mctd =
        {0x03, 0x05, 0x00, 0x99, 0x00, 0xF4, 0x01};
    auto ctd = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                               cmd_frame(0x4B, std::span<const std::uint8_t>(mctd.data(), mctd.size())));
    assert(action_is(ctd, "action", "color_temperature_move_down"));

    // ColorCtrl cluster must be bound on EP1.
    assert(def_binds(def, 1, 0x0300));
}

// ── ZB-3008: mains, 3-EP colour scene remote → endpoint-suffixed actions. ──
void check_3008() {
    using namespace zhc::devices::rgb_genie;
    const auto& def = kDef_ZB_3008;
    check_remote_shape(def);
    assert(!def_exposes(def, "battery"));      // mains-powered, no battery
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count == 3);

    // On press on EP2 → action_ep2 = "on", no bare `action`.
    RuntimeContext ctx1{};
    auto on2 = dispatch_zcl_ep(ctx1, def, 0x0006, 2, "genOnOff", cmd_frame(0x01));
    assert(on2.any_matched);
    assert(action_is(on2, "action_ep2", "on"));
    assert(on2.merged.find("action") == nullptr);

    // Recall scene 1 on EP3 → action_ep3 = "recall_1".
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x01};
    auto re3 = dispatch_zcl_ep(ctx2, def, 0x0005, 3, "genScenes",
                               cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re3.any_matched);
    assert(action_is(re3, "action_ep3", "recall_1"));

    // hue_move on EP1 → action_ep1 (colour decoder fires under multi-EP too).
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 2> mh = {0x01, 0x14};
    auto hm = dispatch_zcl_ep(ctx3, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(mh.data(), mh.size())));
    assert(hm.any_matched);
    assert(action_is(hm, "action_ep1", "hue_move"));
}

// ── ZB-5028: battery, 4-EP colour scene remote. ───────────────────────────
void check_5028() {
    using namespace zhc::devices::rgb_genie;
    const auto& def = kDef_ZB_5028;
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));
    assert(def.endpoint_map && def.endpoint_map_count == 4);

    // Recall scene 3 on EP4 → action_ep4 = "recall_3".
    RuntimeContext ctx1{};
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x03};
    auto re = dispatch_zcl_ep(ctx1, def, 0x0005, 4, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re.any_matched);
    assert(action_is(re, "action_ep4", "recall_3"));

    // command_move_to_color on EP1 → action_ep1 = "color_move".
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 6> mc = {0x34, 0x12, 0x78, 0x56, 0x0A, 0x00};
    auto cm = dispatch_zcl_ep(ctx2, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x07, std::span<const std::uint8_t>(mc.data(), mc.size())));
    assert(cm.any_matched);
    assert(action_is(cm, "action_ep1", "color_move"));

    // Battery (genPowerCfg attr 0x0021 = 200 → 100%) stays GLOBAL (per
    // dispatch policy battery is never endpoint-suffixed).
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx3, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    assert(uint_is(br, "battery", 100));
    assert(br.merged.find("battery_ep1") == nullptr);   // never suffixed
}

}  // namespace

int main() {
    check_light();
    check_battery_remote(zhc::devices::rgb_genie::kDef_ZB_5001);
    check_battery_remote(zhc::devices::rgb_genie::kDef_ZB_5004);
    check_5121();
    check_color_move(zhc::devices::rgb_genie::kDef_ZB_5122);   // battery bundle
    check_color_move(zhc::devices::rgb_genie::kDef_ZB_3009);   // mains bundle
    check_3008();
    check_5028();
    return 0;
}
