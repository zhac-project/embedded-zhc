// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Calex family. Two real z2m<->ez gaps caught here:
//
//   * 421792 — LED A60 RGB lamp. z2m drives it with
//     m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"]}}),
//     i.e. the full colour + colour-temperature axis. The auto-port wired
//     only kFzOnOff + kFzBrightness and dropped the entire lightingColorCtrl
//     (0x0300) channel. The fix restores kFzColor + kFzColorTemperature
//     (+ kTzColor + kTzColorTemp) and the hue/saturation/color_x/color_y/
//     color_temp exposes.
//
//   * 421782 — "Smart Wall Switch" RGB wall controller. z2m models it as a
//     REMOTE (toZigbee:[], fromZigbee=[command_off, command_on, command_step,
//     command_move_to_color_temp, command_move, command_stop,
//     command_enhanced_move_to_hue_and_saturation], exposes:[e.action]). The
//     auto-port mis-ported it as a controllable on/off switch (kFzOnOff +
//     kTzOnOff + a dead `state` expose). The fix wires the seven generic
//     command converters + an `action` expose.
//
// 421786 (plain m.light(), on/off + brightness only) is a regression guard:
// it must stay on/off + brightness with NO colour axis.
//
// z2m-source: zigbee-herdsman-converters/src/devices/calex.ts.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::calex {
extern const PreparedDefinition kDef_D421782;
extern const PreparedDefinition kDef_D421786;
extern const PreparedDefinition kDef_D421792;
}  // namespace zhc::devices::calex

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (cluster-specific=false, Report=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// Cluster-specific command frame: fc=0x01, tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : body) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool action_is(const DispatchResult& r, const char* expect) {
    const Value* v = r.merged.find("action");
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

constexpr std::uint16_t GEN_ON_OFF   = 0x0006;
constexpr std::uint16_t GEN_LEVEL    = 0x0008;
constexpr std::uint16_t COLOR_CTRL   = 0x0300;

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

// ── 421792: RGB lamp — the dropped 0x0300 colour + colour-temp axis ──
void check_421792() {
    const auto& def = devices::calex::kDef_D421792;

    // Light base stays (regression).
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // Colour axis restored.
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "color_temp"));

    // lightingColorCtrl currentX (attr 0x0003, u16) → color_x = raw/65535.
    auto rx = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0003, 0x21, u16(32768)));
    assert(rx.any_matched);
    const Value* cx = rx.merged.find("color_x");
    assert(cx && cx->type == ValueType::Float && cx->f > 0.49f && cx->f < 0.51f);

    // lightingColorCtrl colorTemperature (attr 0x0007, u16 mireds) → color_temp.
    auto rct = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0007, 0x21, u16(250)));
    assert(rct.any_matched);
    const Value* ct = rct.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 250);

    // lightingColorCtrl currentSaturation (attr 0x0001, u8) → saturation.
    const std::uint8_t sat[] = {0xFE};
    auto rs = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0001, 0x20, sat));
    assert(rs.any_matched);
    const Value* sv = rs.merged.find("saturation");
    assert(sv && sv->type == ValueType::Uint && sv->u == 254);
}

// ── 421782: RGB wall controller — command-style action remote ────────
void check_421782() {
    const auto& def = devices::calex::kDef_D421782;
    assert(def_exposes(def, "action"));
    // Not a controllable on/off switch — the dead `state` expose is gone.
    assert(!def_exposes(def, "state"));

    // genOnOff On (cmd 0x01) → "on".
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {}));
    assert(action_is(ron, "on"));
    // genOnOff Off (cmd 0x00) → "off".
    auto roff = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {}));
    assert(action_is(roff, "off"));

    // genLevelCtrl Step (cmd 0x02), mode=0 → "brightness_step_up".
    const std::uint8_t step[] = {0x00, 0x19, 0x00, 0x00};  // mode=up, size, transtime u16
    auto rstep = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x02, step));
    assert(action_is(rstep, "brightness_step_up"));
    // genLevelCtrl Move (cmd 0x01), mode=1 → "brightness_move_down".
    const std::uint8_t mv[] = {0x01, 0x32};
    auto rmv = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x01, mv));
    assert(action_is(rmv, "brightness_move_down"));
    // genLevelCtrl Stop (cmd 0x03) → "brightness_stop".
    auto rstop = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x03, {}));
    assert(action_is(rstop, "brightness_stop"));

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A) → "color_temperature_move".
    const std::uint8_t mtct[] = {0xFA, 0x00, 0x00, 0x00};  // colortemp u16 + transtime u16
    auto rctm = dispatch_zcl(def, COLOR_CTRL, 1, cmd_frame(0x0A, mtct));
    assert(action_is(rctm, "color_temperature_move"));

    // lightingColorCtrl EnhancedMoveToHueAndSat (cmd 0x43) →
    // "enhanced_move_to_hue_and_saturation".
    const std::uint8_t emhs[] = {0x00, 0x80, 0xFE, 0x00, 0x00};  // ehue u16 + sat u8 + tt u16
    auto remhs = dispatch_zcl(def, COLOR_CTRL, 1, cmd_frame(0x43, emhs));
    assert(action_is(remhs, "enhanced_move_to_hue_and_saturation"));
}

// ── 421786: plain m.light() — on/off + brightness, NO colour (guard) ─
void check_421786() {
    const auto& def = devices::calex::kDef_D421786;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // Plain m.light(): no colour / colour-temp axis.
    assert(!def_exposes(def, "hue"));
    assert(!def_exposes(def, "color_x"));
    assert(!def_exposes(def, "color_temp"));
}

}  // namespace

int main() {
    check_421792();
    check_421782();
    check_421786();
    return 0;
}
