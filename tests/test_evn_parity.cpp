// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the EVN family. Pins the two real z2m<->ZHC gaps the
// auto-generated ports carried:
//
//   * ZB24100VS "multicolor controller with power supply" is, in z2m,
//     m.light({colorTemp:{range:[160,450]}, color:{modes:["xy","hs"]}}) —
//     a full RGB + colour-temperature light. The generated def dropped the
//     entire lightingColorCtrl (0x0300) axis (ported as on/off+brightness
//     only): no colour, no colour-temperature read or write. Restored via
//     kFzColor + kFzColorTemperature (fz) / kTzColor + kTzColorTemp (tz),
//     the colour exposes, and a 0x0300 bind.
//
//   * ZBHS4RGBW is a 4-channel RGBW battery remote (toZigbee: [], action
//     only) — the CLIENT of genOnOff / genLevelCtrl / lightingColorCtrl.
//     The generator wrong-bundled it as a settable on/off switch
//     (kFzOnOff + kTzOnOff + writable `state`), so every button press was
//     dead and it falsely advertised a relay. Rewired to the command_*
//     decoders + an `action` expose with endpoint_map {ep1..ep4} +
//     endpoint_action_suffix (z2m multiEndpoint + endpoint() map).
//
// z2m-source: zigbee-herdsman-converters/src/devices/evn.ts
//             + converters/fromZigbee.ts command_* / fz.color_colortemp +
//             lib/modernExtend.ts m.light.

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

namespace zhc::devices::evn {
extern const PreparedDefinition kDef_ZBHS4RGBW;   // 4-ch RGBW remote (action)
extern const PreparedDefinition kDef_ZB24100VS;   // RGB+CCT light
}  // namespace zhc::devices::evn

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

bool has_fz_cluster(const PreparedDefinition& def, const char* cluster) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i]->cluster && std::strcmp(def.from_zigbee[i]->cluster, cluster) == 0)
            return true;
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

// ── ZBHS4RGBW: 4-channel RGBW remote → action_<epN>, no settable state. ───
void check_remote() {
    const auto& def = zhc::devices::evn::kDef_ZBHS4RGBW;

    // Shape: action expose, NO phantom relay, empty toolbox (toZigbee: []).
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));      // phantom relay must be gone
    assert(def.to_zigbee_count == 0);        // cannot be controlled
    assert(def.from_zigbee_count > 0);
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count == 4);

    // genOnOff On on EP3 → action_ep3 = "on", no bare action leak.
    RuntimeContext ctx1{};
    auto on3 = dispatch_zcl_ep(ctx1, def, 0x0006, 3, "genOnOff", cmd_frame(0x01));
    assert(on3.any_matched);
    assert(action_is(on3, "action_ep3", "on"));
    assert(on3.merged.find("action") == nullptr);

    // genOnOff Off on EP1 → action_ep1 = "off".
    RuntimeContext ctx2{};
    auto off1 = dispatch_zcl_ep(ctx2, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(off1.any_matched);
    assert(action_is(off1, "action_ep1", "off"));

    // genLevelCtrl Step up on EP2 (cmd 0x02): mode=0,size=10,tt=0 →
    // action_ep2 = "brightness_step_up".
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 4> step = {0x00, 0x0A, 0x00, 0x00};
    auto su = dispatch_zcl_ep(ctx3, def, 0x0008, 2, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
    assert(su.any_matched);
    assert(action_is(su, "action_ep2", "brightness_step_up"));

    // genLevelCtrl Move up on EP4 (cmd 0x01, mode=0, rate=50) →
    // action_ep4 = "brightness_move_up".
    RuntimeContext ctx4{};
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};
    auto mvr = dispatch_zcl_ep(ctx4, def, 0x0008, 4, "genLevelCtrl",
                               cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mvr.any_matched);
    assert(action_is(mvr, "action_ep4", "brightness_move_up"));

    // genLevelCtrl Stop on EP1 → action_ep1 = "brightness_stop".
    RuntimeContext ctx5{};
    auto st = dispatch_zcl_ep(ctx5, def, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched);
    assert(action_is(st, "action_ep1", "brightness_stop"));

    // lightingColorCtrl MoveToColor (cmd 0x07): cx u16, cy u16, tt u16 on EP2
    // → action_ep2 = "color_move".
    RuntimeContext ctx6{};
    const std::array<std::uint8_t, 6> col = {0x34, 0x12, 0x78, 0x56, 0x00, 0x00};
    auto cmv = dispatch_zcl_ep(ctx6, def, 0x0300, 2, "lightingColorCtrl",
                               cmd_frame(0x07, std::span<const std::uint8_t>(col.data(), col.size())));
    assert(cmv.any_matched);
    assert(action_is(cmv, "action_ep2", "color_move"));

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A): ct u16 + tt u16 on EP3
    // → action_ep3 = "color_temperature_move".
    RuntimeContext ctx7{};
    const std::array<std::uint8_t, 4> ct = {0x9A, 0x01, 0x00, 0x00};  // ct=410
    auto ctm = dispatch_zcl_ep(ctx7, def, 0x0300, 3, "lightingColorCtrl",
                               cmd_frame(0x0A, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(ctm.any_matched);
    assert(action_is(ctm, "action_ep3", "color_temperature_move"));

    // genScenes-less device: recall comes over genOnOff? No — z2m uses
    // command_recall on genScenes. genScenes Recall (cmd 0x05): group u16 +
    // scene u8=1 on EP1 → action_ep1 = "recall_1".
    RuntimeContext ctx8{};
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x01};
    auto re = dispatch_zcl_ep(ctx8, def, 0x0005, 1, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re.any_matched);
    assert(action_is(re, "action_ep1", "recall_1"));

    // Battery (genPowerCfg attr 0x0021 = 200 → 100%) stays GLOBAL on EP1.
    RuntimeContext ctx9{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx9, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
    assert(br.merged.find("battery_ep1") == nullptr);   // battery never suffixed

    // Binds: genOnOff / genLevelCtrl / lightingColorCtrl / genPowerCfg on EP1.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));
    assert(def_binds(def, 1, 0x0001));
}

// ── ZB24100VS: RGB + colour-temperature light. ────────────────────────────
void check_light() {
    const auto& def = zhc::devices::evn::kDef_ZB24100VS;

    // Core light exposes plus the restored colour axis.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));

    // The colour converters are wired on both directions.
    assert(has_fz_cluster(def, "lightingColorCtrl"));
    assert(def.to_zigbee_count >= 4);   // onoff + brightness + color + colortemp
    assert(def_binds(def, 1, 0x0300));  // lightingColorCtrl bound

    // colorTempMireds report (attr 0x0007 = 370) → color_temp = 370.
    RuntimeContext ctx1{};
    const std::array<std::uint8_t, 2> ctv = {0x72, 0x01};  // 0x0172 = 370
    auto ct = dispatch_zcl_ep(ctx1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0007, 0x21,
                                          std::span<const std::uint8_t>(ctv.data(), ctv.size())));
    assert(ct.any_matched);
    const Value* c = ct.merged.find("color_temp");
    assert(c && c->type == ValueType::Uint && c->u == 370);

    // currentHue (attr 0x0000 = 127) + currentSaturation (attr 0x0001 = 254)
    // → hue / saturation decoded (kFzColor, hs mode).
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 1> hv = {0x7F};
    auto h = dispatch_zcl_ep(ctx2, def, 0x0300, 1, "lightingColorCtrl",
                             attr_report(0x0000, 0x20,
                                         std::span<const std::uint8_t>(hv.data(), hv.size())));
    assert(h.any_matched);
    assert(h.merged.find("hue") != nullptr);

    // Brightness still works (genLevelCtrl currentLevel attr 0x0000 = 128).
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 1> lv = {0x80};
    auto br = dispatch_zcl_ep(ctx3, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20,
                                          std::span<const std::uint8_t>(lv.data(), lv.size())));
    assert(br.any_matched);
    assert(br.merged.find("brightness") != nullptr);
}

}  // namespace

int main() {
    check_remote();
    check_light();
    return 0;
}
