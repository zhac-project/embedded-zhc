// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the EssentielB (Boulanger) family.
//
// EssentielB ships nine m.light() LED bulbs (CCT + RGBW) and one
// command-driven smart button. The auto-generator collapsed every device
// to its dumbest shape, so the real z2m<->ZHC gaps were:
//
//   * All nine bulbs — z2m m.light({colorTemp[, color:true]}) gives
//     on/off + level + colorTemp (+ color on the RGBW SKUs). The generated
//     ports wired only kFzOnOff + kFzBrightness, dropping the colour-temp
//     axis on every bulb and the full colour axis on the three RGBW SKUs
//     (the lightingColorCtrl 0x0300 cluster was neither decoded nor bound).
//     Restored kFzColorTemperature (+ kFzColor on RGBW) + their TZ + the
//     colour exposes + the 0x0300 binding.
//
//   * EB-SB-1B "smart button" — z2m fz.battery + fz.command_on/off/step/
//     stop/step_color_temperature, exposing battery + e.action([...]). The
//     generated port wired kFzOnOff + an on/off TZ (both dead — the remote
//     sends genOnOff *commands*, never attribute reports, and controls no
//     local relay) and exposed a meaningless controllable `state`. The
//     action surface — the device's whole purpose — was dropped. Re-wired
//     to the generic command-action decoders so the buttons surface as
//     `action`.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * a CCT bulb decodes color_temp; an RGBW bulb decodes color_temp + the
//     colour axes; both bind lightingColorCtrl 0x0300,
//   * the smart button emits the correct `action` for each command family
//     and carries NO controllable `state` and NO toZigbee.
//
// z2m-source: zigbee-herdsman-converters/src/devices/essentialb.ts.

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

namespace zhc::devices::essentialb {
extern const PreparedDefinition kDef_EB_E14_P45_RGBW;   // RGBW bulb (colorTemp + color)
extern const PreparedDefinition kDef_EB_E27_A60_CCT;    // CCT bulb (colorTemp only)
extern const PreparedDefinition kDef_EB_SB_1B;          // smart button (action)
}  // namespace zhc::devices::essentialb

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

const char* str_of(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

// ZCL attribute report builder: fc=0x18 | tsn | cmd=0x0A | one record.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_report(const PreparedDefinition& def, std::uint16_t cluster_id,
                               const char* cluster_name, std::uint8_t src_ep,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Dispatch a cluster-specific command frame (client->server, fc=0x01).
// The decoder leaves `cluster` numeric and several command cmd-ids collide
// across clusters (genOnOff Off 0x00 vs genLevelCtrl ids), so the caller
// stamps the cluster name post-decode — exactly as the radio layer does.
//   bytes = ZCL payload: fc(1)=0x01 | tsn(1) | cmd(1) | body...
DispatchResult dispatch_cmd(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── RGBW bulb: colorTemp + colour axes decode; binds 0x0300 ──────────
static void test_rgbw_bulb() {
    using namespace zhc::devices::essentialb;
    const auto& def = kDef_EB_E14_P45_RGBW;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "saturation"));
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl now bound

    // colorTemperature attr 0x0007 (u16) = 250 mireds -> color_temp.
    const std::array<std::uint8_t, 2> ct = {0xFA, 0x00};  // 250
    auto rc = dispatch_report(def, 0x0300, "lightingColorCtrl", 1,
                              attr_report(0x0007, 0x21, {ct.data(), ct.size()}));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("color_temp");
    assert(cv && cv->type == ValueType::Uint && cv->u == 250);

    // currentX attr 0x0003 (u16) -> color_x (float, raw/65535).
    const std::array<std::uint8_t, 2> cx = {0xFF, 0x7F};  // 0x7FFF
    auto rx = dispatch_report(def, 0x0300, "lightingColorCtrl", 1,
                              attr_report(0x0003, 0x21, {cx.data(), cx.size()}));
    assert(rx.any_matched);
    const Value* xv = rx.merged.find("color_x");
    assert(xv && xv->type == ValueType::Float);

    // currentSaturation attr 0x0001 (u8) = 200 -> saturation.
    const std::array<std::uint8_t, 1> sa = {0xC8};  // 200
    auto rs = dispatch_report(def, 0x0300, "lightingColorCtrl", 1,
                              attr_report(0x0001, 0x20, {sa.data(), sa.size()}));
    assert(rs.any_matched);
    const Value* sv = rs.merged.find("saturation");
    assert(sv && sv->type == ValueType::Uint && sv->u == 200);
}

// ── CCT bulb: colorTemp decodes, NO colour axes; binds 0x0300 ────────
static void test_cct_bulb() {
    using namespace zhc::devices::essentialb;
    const auto& def = kDef_EB_E27_A60_CCT;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));      // CCT-only, no colour axis
    assert(!def_exposes(def, "saturation"));
    assert(def_binds(def, 1, 0x0300));

    // colorTemperature attr 0x0007 (u16) = 400 mireds -> color_temp.
    const std::array<std::uint8_t, 2> ct = {0x90, 0x01};  // 400
    auto rc = dispatch_report(def, 0x0300, "lightingColorCtrl", 1,
                              attr_report(0x0007, 0x21, {ct.data(), ct.size()}));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("color_temp");
    assert(cv && cv->type == ValueType::Uint && cv->u == 400);
}

// ── EB-SB-1B smart button: command stream -> action, no dead state ───
static void test_smart_button() {
    using namespace zhc::devices::essentialb;
    const auto& def = kDef_EB_SB_1B;

    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));        // not a controllable switch
    assert(def.to_zigbee_count == 0);          // no dead on/off TZ
    assert(def_binds(def, 1, 0x0300));         // lightingColorCtrl (step CT)
    assert(def_binds(def, 1, 0x0008));         // genLevelCtrl (step/stop)
    assert(def_binds(def, 1, 0x0001));         // genPowerCfg (battery)

    // genOnOff On (0x01) -> action = "on"; raw state never published.
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto ron = dispatch_cmd(def, 0x0006, "genOnOff", 1, {on.data(), on.size()});
    assert(ron.any_matched);
    const char* aon = str_of(ron, "action");
    assert(aon && std::strcmp(aon, "on") == 0);
    assert(ron.merged.find("state") == nullptr);

    // genOnOff Off (0x00) -> action = "off".
    const std::array<std::uint8_t, 3> off = {0x01, 0x42, 0x00};
    auto roff = dispatch_cmd(def, 0x0006, "genOnOff", 1, {off.data(), off.size()});
    assert(roff.any_matched);
    const char* aoff = str_of(roff, "action");
    assert(aoff && std::strcmp(aoff, "off") == 0);

    // genLevelCtrl Step (0x02): mode=0 (up), size=10, transition u16=0 LE
    //   -> action = "brightness_step_up". Body = mode|size|tt_lo|tt_hi.
    const std::array<std::uint8_t, 7> stup = {0x01, 0x42, 0x02, 0x00, 0x0A, 0x00, 0x00};
    auto rsu = dispatch_cmd(def, 0x0008, "genLevelCtrl", 1, {stup.data(), stup.size()});
    assert(rsu.any_matched);
    const char* asu = str_of(rsu, "action");
    assert(asu && std::strcmp(asu, "brightness_step_up") == 0);

    // genLevelCtrl Step (0x02): mode=1 (down) -> "brightness_step_down".
    const std::array<std::uint8_t, 7> stdn = {0x01, 0x42, 0x02, 0x01, 0x0A, 0x00, 0x00};
    auto rsd = dispatch_cmd(def, 0x0008, "genLevelCtrl", 1, {stdn.data(), stdn.size()});
    assert(rsd.any_matched);
    const char* asd = str_of(rsd, "action");
    assert(asd && std::strcmp(asd, "brightness_step_down") == 0);

    // genLevelCtrl Stop (0x03): no body -> "brightness_stop".
    const std::array<std::uint8_t, 3> stop = {0x01, 0x42, 0x03};
    auto rst = dispatch_cmd(def, 0x0008, "genLevelCtrl", 1, {stop.data(), stop.size()});
    assert(rst.any_matched);
    const char* ast = str_of(rst, "action");
    assert(ast && std::strcmp(ast, "brightness_stop") == 0);

    // lightingColorCtrl StepColorTemperature (0x4C): mode=0x01 (up),
    //   size u16=20 LE, transition u16=0 LE, +min/max tail
    //   -> action = "color_temperature_step_up".
    const std::array<std::uint8_t, 11> ctup =
        {0x01, 0x42, 0x4C, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto rcu = dispatch_cmd(def, 0x0300, "lightingColorCtrl", 1, {ctup.data(), ctup.size()});
    assert(rcu.any_matched);
    const char* acu = str_of(rcu, "action");
    assert(acu && std::strcmp(acu, "color_temperature_step_up") == 0);

    // mode=0x03 (down) -> "color_temperature_step_down".
    const std::array<std::uint8_t, 11> ctdn =
        {0x01, 0x42, 0x4C, 0x03, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto rcd = dispatch_cmd(def, 0x0300, "lightingColorCtrl", 1, {ctdn.data(), ctdn.size()});
    assert(rcd.any_matched);
    const char* acd = str_of(rcd, "action");
    assert(acd && std::strcmp(acd, "color_temperature_step_down") == 0);
}

int main() {
    test_rgbw_bulb();
    test_cct_bulb();
    test_smart_button();
    return 0;
}
