// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Paulmann remote-control family.
//
// Paulmann is overwhelmingly a lighting vendor (m.light(...) / m.onOff()
// over stock ZCL) — those ports are CLEAN against z2m and exercised by the
// generic suites. The one real z2m<->ZHC gap was the *remotes*: 501.37,
// 501.34, 500.67, 501.40 and 501.41 are all battery-powered scene/RGB
// remotes with `toZigbee: []` (they cannot be controlled), yet the
// auto-generator wrong-bundled every one of them as a settable on/off
// light — kFzOnOff + kTzOnOff + a writable `state` expose — and dropped
// the `action` decode entirely. Result: every button press was dead and
// the device falsely advertised a relay.
//
// Fix (mirrors the vesternet REM-013 / innr RC remote fixes): graduate
// each def, swap the on/off fz/tz for the genOnOff / genLevelCtrl /
// lightingColorCtrl / genScenes command decoders, expose `action`, drop
// the phantom state + toolbox, and — for the multi-button models that
// z2m marks multiEndpoint / deviceEndpoints — add endpoint_map +
// endpoint_action_suffix so each button keeps identity (action_<n>).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * every remote exposes `action` and NO settable `state`,
//   * every remote has an empty toolbox (to_zigbee_count == 0),
//   * genOnOff / genLevelCtrl / lightingColorCtrl / genScenes commands
//     decode to the right `action` labels,
//   * multi-button remotes (501.37/34/40) suffix per endpoint (action_<n>)
//     and never leak a bare `action`,
//   * single-endpoint remotes (500.67/501.41) emit a bare `action`,
//   * battery stays a global key (never suffixed).
//
// z2m-source: zigbee-herdsman-converters/src/devices/paulmann.ts
//             #501.37 / #501.34 / #500.67 / #501.40 / #501.41 +
//             converters/fromZigbee.ts command_* +
//             lib/modernExtend.ts commandsOnOff/LevelCtrl/ColorCtrl/Scenes.

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

namespace zhc::devices::paulmann {
extern const PreparedDefinition kDef_D501_37;
extern const PreparedDefinition kDef_D501_34;
extern const PreparedDefinition kDef_D500_67;
extern const PreparedDefinition kDef_D501_40;
extern const PreparedDefinition kDef_D501_41;
}  // namespace zhc::devices::paulmann

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

// ── Shared invariants for every remote: action expose, NO settable state,
//    empty toolbox (the device cannot be controlled). ──────────────────────
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // phantom relay must be gone
    assert(def.to_zigbee_count == 0);     // toZigbee: [] — no write path
    assert(def.from_zigbee_count > 0);
}

// ── 501.37 / 501.34: 2-rocker switches → action_1 / action_2. ─────────────
// genOnOff On (cmd 0x01) and genLevelCtrl Move (cmd 0x05) probed per EP.
void check_two_rocker(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count == 2);

    // On press on EP1 → action_1 = "on", no bare `action`.
    RuntimeContext ctx1{};
    auto on1 = dispatch_zcl_ep(ctx1, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on1.any_matched);
    assert(action_is(on1, "action_1", "on"));
    assert(on1.merged.find("action") == nullptr);

    // Off press on EP2 → action_2 = "off".
    RuntimeContext ctx2{};
    auto off2 = dispatch_zcl_ep(ctx2, def, 0x0006, 2, "genOnOff", cmd_frame(0x00));
    assert(off2.any_matched);
    assert(action_is(off2, "action_2", "off"));
    assert(off2.merged.find("action") == nullptr);

    // genLevelCtrl Move up on EP2 (cmd 0x01, move_mode=0, rate=50) →
    // action_2 = "brightness_move_up".
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};
    auto mvr = dispatch_zcl_ep(ctx3, def, 0x0008, 2, "genLevelCtrl",
                               cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mvr.any_matched);
    assert(action_is(mvr, "action_2", "brightness_move_up"));

    // genLevelCtrl Stop on EP1 → action_1 = "brightness_stop".
    RuntimeContext ctx4{};
    auto st = dispatch_zcl_ep(ctx4, def, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched);
    assert(action_is(st, "action_1", "brightness_stop"));

    // Binds: genOnOff + genLevelCtrl on both EP1 and EP2.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 2, 0x0006));
    assert(def_binds(def, 2, 0x0008));
}

// ── 500.67: single-EP RGB remote → bare action. ──────────────────────────
void check_500_67(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(!def.endpoint_action_suffix);   // single endpoint → no suffix
    assert(def.endpoint_map == nullptr);

    RuntimeContext ctx{};
    // genOnOff Toggle (cmd 0x02) → bare action = "toggle".
    auto tg = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x02));
    assert(tg.any_matched);
    assert(action_is(tg, "action", "toggle"));
    assert(tg.merged.find("action_1") == nullptr);

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A): ct u16 + transition u16
    // → action = "color_temperature_move".
    const std::array<std::uint8_t, 4> ct = {0x9A, 0x01, 0x00, 0x00};  // ct=410
    auto cm = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x0A, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(cm.any_matched);
    assert(action_is(cm, "action", "color_temperature_move"));

    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl bound
}

// ── 501.40: 4-button RGB remote → action_1..action_4 + battery global. ────
void check_501_40(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count == 4);

    // genOnOff On on EP3 → action_3 = "on".
    RuntimeContext ctx1{};
    auto on3 = dispatch_zcl_ep(ctx1, def, 0x0006, 3, "genOnOff", cmd_frame(0x01));
    assert(on3.any_matched);
    assert(action_is(on3, "action_3", "on"));
    assert(on3.merged.find("action") == nullptr);

    // genScenes Recall (cmd 0x05): group u16 + scene u8=2 on EP4 →
    // action_4 = "recall_2".
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x02};
    auto re = dispatch_zcl_ep(ctx2, def, 0x0005, 4, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re.any_matched);
    assert(action_is(re, "action_4", "recall_2"));

    // Battery (genPowerCfg attr 0x0021 = 200 → 100%) stays global on EP1.
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx3, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
    assert(br.merged.find("battery_1") == nullptr);   // battery never suffixed
}

// ── 501.41: single-EP white remote → bare action incl. color-temp move. ───
void check_501_41(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(!def.endpoint_action_suffix);
    assert(def.endpoint_map == nullptr);

    RuntimeContext ctx{};
    // lightingColorCtrl MoveColorTemperature (cmd 0x4B), move_mode=0 (stop)
    // → action = "color_temperature_move_stop" (the normal stop path; the
    // 5-byte raw firmware quirk is documented as out of scope in the def).
    const std::array<std::uint8_t, 1> stop = {0x00};
    auto cstop = dispatch_zcl_ep(ctx, def, 0x0300, 1, "lightingColorCtrl",
                                 cmd_frame(0x4B, std::span<const std::uint8_t>(stop.data(), stop.size())));
    assert(cstop.any_matched);
    assert(action_is(cstop, "action", "color_temperature_move_stop"));

    // genScenes Recall scene 1 → action = "recall_1".
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x01};
    auto re = dispatch_zcl_ep(ctx, def, 0x0005, 1, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re.any_matched);
    assert(action_is(re, "action", "recall_1"));

    // genLevelCtrl Step up (cmd 0x02): mode=0,size=10,tt=0 →
    // action = "brightness_step_up".
    const std::array<std::uint8_t, 4> step = {0x00, 0x0A, 0x00, 0x00};
    auto su = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
    assert(su.any_matched);
    assert(action_is(su, "action", "brightness_step_up"));
}

}  // namespace

int main() {
    using namespace zhc::devices::paulmann;
    check_two_rocker(kDef_D501_37);
    check_two_rocker(kDef_D501_34);
    check_500_67(kDef_D500_67);
    check_501_40(kDef_D501_40);
    check_501_41(kDef_D501_41);
    return 0;
}
