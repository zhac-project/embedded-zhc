// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Sowilo DS L258 "Heimdall Pro" 5-channel RGBWW
// controller.
// z2m-source: sowilo.ts — the sole def is
//   m.light({colorTemp:{range:[150,575]}, color:{modes:["xy","hs"]},
//            turnsOffAtBrightness1:true})
//
// Bug fixed: the auto-generated Tier-1 port wired only kFzOnOff +
// kFzBrightness (clusters 0x0006/0x0008), dropping the lightingColorCtrl
// (0x0300) axis entirely — both the colorTemp and the RGB colour surface.
// The def was graduated to a Tier-2 override restoring color_temp /
// color_xy exposes, the colorTemp + colour converters, and the 0x0300
// binding.
//
// This pins, on real lightingColorCtrl AttributeReport wire shapes:
//   * the full colour surface (state/brightness/color_temp/color_xy) and
//     the 0x0006/0x0008/0x0300 bindings are present;
//   * color_temp (attr 0x0007) decodes;
//   * hue (attr 0x0000) / saturation (attr 0x0001) decode (the colour axis
//     z2m enables via color.modes ["xy","hs"]).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sowilo {
extern const PreparedDefinition kDef_L258;
}  // namespace zhc::devices::sowilo

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

bool has_uint(const Value* v, std::uint64_t expected) {
    return v && v->type == ValueType::Uint && v->u == expected;
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
// + currentSaturation (attr 0x0001, uint8) in one frame.
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

// Full RGBWW surface: state/brightness/color_temp/color_xy, all three
// clusters bound, and the colour axis decodes off lightingColorCtrl.
void test_l258_full_colour_surface() {
    const auto& def = devices::sowilo::kDef_L258;

    // Surface restored.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp") &&
           "L258 must restore color_temp (was dropped by the generated port)");
    assert(def_exposes(def, "color_xy") &&
           "L258 must restore the colour axis (was dropped by the generated port)");

    // Bindings.
    assert(def_binds(def, 1, 0x0006));  // genOnOff
    assert(def_binds(def, 1, 0x0008));  // genLevelCtrl
    assert(def_binds(def, 1, 0x0300) &&
           "L258 must bind lightingColorCtrl (was missing)");

    // colorTemp decodes (attr 0x0007).
    auto ct = dispatch_color_temp(def, 370);
    assert(ct.any_matched);
    assert(has_uint(ct.merged.find("color_temp"), 370));

    // Colour (hue/sat) decodes — the axis z2m enables via color.modes.
    auto hs = dispatch_hue_sat(def, 100, 200);
    assert(hs.any_matched);
    assert(has_uint(hs.merged.find("hue"), 100));
    assert(has_uint(hs.merged.find("saturation"), 200));
}

}  // namespace

int main() {
    test_l258_full_colour_surface();
    return 0;
}
