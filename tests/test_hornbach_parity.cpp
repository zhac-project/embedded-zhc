// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the HORNBACH (FLAIR Viyu) smart LED bulb family.
// z2m-source: hornbach.ts — every def is `m.light({colorTemp, color?})`.
//
// Bug fixed: all 14 generated defs were wired with only kFzOnOff +
// kFzBrightness (clusters 0x0006/0x0008), dropping the lightingColorCtrl
// (0x0300) axis entirely. z2m gives every hornbach bulb colorTemp, and the
// six *_RGBW_* / "RGB" variants full hue/saturation colour as well.
//
// All 14 graduated to two shared bundles in definitions/hornbach/_shared.cpp:
//   CTLight      — on/off + brightness + color_temp           (8 defs)
//   ColorCTLight — + color (hue/sat/xy)                        (6 defs)
//
// These tests pin, on real lightingColorCtrl AttributeReport wire shapes:
//   * every def decodes color_temp (attr 0x0007) and exposes it + binds 0x0300
//   * the RGBW defs additionally decode hue (0x0000) / saturation (0x0001)
//   * the CCT defs do NOT carry the colour expose/converter
// and verifies the converter/binding wiring structurally.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::hornbach {
// CCT (CTLight bundle).
extern const PreparedDefinition kDef_D10011723;  // VIYU-A60-806-CCT
extern const PreparedDefinition kDef_D10447293;  // VIYU_MUSICA_CCT (ceiling)
// RGBW (ColorCTLight bundle).
extern const PreparedDefinition kDef_D10454471;  // VIYU_A60_806_RGBW
extern const PreparedDefinition kDef_D10297667;  // VIYU_C35_470_RGBW
}  // namespace zhc::devices::hornbach

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

// lightingColorCtrl (0x0300) Report Attributes for colorTemperature
// (attr 0x0007, uint16):
//   fc(1)=0x18 tsn(1) cmd(1)=0x0A  attr_le(2)=0x0007 type(1)=0x21 value_le(2)
DispatchResult dispatch_color_temp(const PreparedDefinition& def, std::uint16_t mireds) {
    const std::uint8_t bytes[] = {
        0x18, 0x42, 0x0A,
        0x07, 0x00,            // attr 0x0007 (colorTemperature)
        0x21,                  // type uint16
        static_cast<std::uint8_t>(mireds & 0xFF),
        static_cast<std::uint8_t>(mireds >> 8),
    };
    auto raw = make_frame(0x0300, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// lightingColorCtrl Report Attributes for currentHue (attr 0x0000, uint8)
// + currentSaturation (attr 0x0001, uint8) in one frame:
//   fc tsn cmd  0x0000 0x20 hue   0x0001 0x20 sat
DispatchResult dispatch_hue_sat(const PreparedDefinition& def,
                                std::uint8_t hue, std::uint8_t sat) {
    const std::uint8_t bytes[] = {
        0x18, 0x42, 0x0A,
        0x00, 0x00, 0x20, hue,   // attr 0x0000 (currentHue) uint8
        0x01, 0x00, 0x20, sat,   // attr 0x0001 (currentSaturation) uint8
    };
    auto raw = make_frame(0x0300, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool has_uint(const Value* v, std::uint64_t expected) {
    return v && v->type == ValueType::Uint && v->u == expected;
}

// Every hornbach def: on/off + brightness + color_temp, 0x0300 bound,
// color_temp decodes off the lightingColorCtrl report.
void check_common_light(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));

    assert(def_binds(def, 1, 0x0006));  // genOnOff
    assert(def_binds(def, 1, 0x0008));  // genLevelCtrl
    assert(def_binds(def, 1, 0x0300));  // lightingColorCtrl

    auto ct = dispatch_color_temp(def, 370);
    assert(ct.any_matched);
    assert(has_uint(ct.merged.find("color_temp"), 370));
}

// CCT bundle: colour axis must be absent (no hue/sat exposes, hue report
// produces nothing).
void check_cct(const PreparedDefinition& def) {
    check_common_light(def);
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "color_hs"));

    auto hs = dispatch_hue_sat(def, 100, 200);
    assert(hs.merged.find("hue") == nullptr);
    assert(hs.merged.find("saturation") == nullptr);
}

// ColorCT (RGBW) bundle: colour axis present and hue/sat decode.
void check_rgbw(const PreparedDefinition& def) {
    check_common_light(def);
    assert(def_exposes(def, "color_xy"));
    assert(def_exposes(def, "color_hs"));

    auto hs = dispatch_hue_sat(def, 100, 200);
    assert(hs.any_matched);
    assert(has_uint(hs.merged.find("hue"), 100));
    assert(has_uint(hs.merged.find("saturation"), 200));
}

}  // namespace

int main() {
    check_cct(devices::hornbach::kDef_D10011723);
    check_cct(devices::hornbach::kDef_D10447293);
    check_rgbw(devices::hornbach::kDef_D10454471);
    check_rgbw(devices::hornbach::kDef_D10297667);
    return 0;
}
