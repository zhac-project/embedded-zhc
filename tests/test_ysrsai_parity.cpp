// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the YSRSAI LED-controller family. Two real z2m<->ez
// gaps caught here — both phantom-battery / dropped-channel mis-ports:
//
//   * YSR-MINI-01_dimmer (ZB-DL01) — z2m drives it with
//     tuya.modernExtend.tuyaLight({configureReporting:true}), i.e. a plain
//     ZCL dimmable light (genOnOff 0x0006 + genLevelCtrl 0x0008 → state +
//     brightness, mains-powered). The auto-port mis-classified it as a
//     battery sensor (kFzBattery + bare kFzOnOff) with phantom battery /
//     voltage exposes and DROPPED brightness entirely. The fix removes the
//     phantom battery bundle and restores kFzBrightness + the brightness
//     expose (+ kTzOnOff/kTzBrightness write-back).
//
//   * YSR-MINI-01_wwcw (ZB-CT01) — z2m uses
//     tuyaLight({colorTemp:{range:[153,500]}}), a tunable-white light:
//     genOnOff + genLevelCtrl + lightingColorCtrl(0x0300) → state +
//     brightness + color_temp (NO x/y colour). The auto-port again wired
//     kFzBattery + bare onOff with phantom battery/voltage and dropped both
//     brightness and color_temp. The fix restores the CCT light.
//
// YSR-MINI-01_rgbcct (full RGB+CCT) was already ported correctly and acts
// as a regression guard: it must keep the colour + colour-temp axis and
// must NOT carry a battery expose.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ysrsai.ts.

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

namespace zhc::devices::ysrsai {
extern const PreparedDefinition kDef_YSR_MINI_01_dimmer;
extern const PreparedDefinition kDef_YSR_MINI_01_wwcw;
extern const PreparedDefinition kDef_YSR_MINI_01_rgbcct;
}  // namespace zhc::devices::ysrsai

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

constexpr std::uint16_t GEN_ON_OFF = 0x0006;
constexpr std::uint16_t GEN_LEVEL  = 0x0008;
constexpr std::uint16_t COLOR_CTRL = 0x0300;

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

// ── ZB-DL01 dimmer: plain dimmable light, no phantom battery ─────────
void check_dimmer() {
    const auto& def = devices::ysrsai::kDef_YSR_MINI_01_dimmer;

    // Real light exposes.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // Phantom battery bundle removed.
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));
    // No colour axis on a plain dimmer.
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));

    // genOnOff onOff (attr 0x0000, bool) → state.
    const std::uint8_t on[] = {0x01};
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, attr_report(0x0000, 0x10, on));
    assert(ron.any_matched);
    const Value* st = ron.merged.find("state");
    assert(st != nullptr);

    // genLevelCtrl currentLevel (attr 0x0000, u8) → brightness. This is the
    // channel the auto-port dropped.
    const std::uint8_t lvl[] = {0xCC};
    auto rb = dispatch_zcl(def, GEN_LEVEL, 1, attr_report(0x0000, 0x20, lvl));
    assert(rb.any_matched);
    const Value* bv = rb.merged.find("brightness");
    assert(bv && bv->type == ValueType::Uint && bv->u == 204);
}

// ── ZB-CT01 wwcw: tunable-white (CCT) light, no phantom battery ──────
void check_wwcw() {
    const auto& def = devices::ysrsai::kDef_YSR_MINI_01_wwcw;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    // Phantom battery bundle removed.
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));
    // WW/CW only — no x/y colour axis.
    assert(!def_exposes(def, "color_x"));
    assert(!def_exposes(def, "hue"));

    // genLevelCtrl currentLevel (attr 0x0000, u8) → brightness (dropped before).
    const std::uint8_t lvl[] = {0x80};
    auto rb = dispatch_zcl(def, GEN_LEVEL, 1, attr_report(0x0000, 0x20, lvl));
    assert(rb.any_matched);
    const Value* bv = rb.merged.find("brightness");
    assert(bv && bv->type == ValueType::Uint && bv->u == 128);

    // lightingColorCtrl colorTemperature (attr 0x0007, u16 mireds) → color_temp.
    auto rct = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0007, 0x21, u16(300)));
    assert(rct.any_matched);
    const Value* ct = rct.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 300);
}

// ── ZB-CL03 rgbcct: full RGB+CCT (regression guard) ─────────────────
void check_rgbcct() {
    const auto& def = devices::ysrsai::kDef_YSR_MINI_01_rgbcct;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    // Mains LED controller — never a battery device.
    assert(!def_exposes(def, "battery"));

    // Write-only command exposes (z2m parity, commit 0524a1f1). These are
    // access=Set, so the SPA renders them on the Commands tab (not States) —
    // assert they're present in the def so a future re-port can't silently drop
    // them again (this guard was missing when they first went unnoticed).
    assert(def_exposes(def, "effect"));
    assert(def_exposes(def, "do_not_disturb"));
    assert(def_exposes(def, "color_power_on_behavior"));
    // z2m tuyaLight uses color_power_on_behavior, NOT the generic startup attr.
    assert(!def_exposes(def, "power_on_behavior"));

    // lightingColorCtrl currentX (attr 0x0003, u16) → color_x.
    auto rx = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0003, 0x21, u16(32768)));
    assert(rx.any_matched);
    const Value* cx = rx.merged.find("color_x");
    assert(cx && cx->type == ValueType::Float && cx->f > 0.49f && cx->f < 0.51f);
}

}  // namespace

int main() {
    check_dimmer();
    check_wwcw();
    check_rgbcct();
    return 0;
}
