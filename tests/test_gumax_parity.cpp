// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Gumax LST103 lighting system.
//
//   Dropped color/colorTemp axis — z2m models this device as a full
//   ColorCTLight:
//     m.light({colorTemp: {range: [153, 370]},
//              color: {modes: ["xy", "hs"], enhancedHue: true}})
//   i.e. on/off + level + colorTemp + color (xy + hs). The auto-generator
//   collapses m.light() down to on/off + brightness only, dropping the CT
//   and color axes. The graduated def restores kFzColorTemperature + kFzColor
//   (and kTzColorTemp + kTzColor) plus the lightingColorCtrl (0x0300) binding
//   and the color_temp / color_x / color_y / hue / saturation exposes.

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

namespace zhc::devices::gumax {
extern const PreparedDefinition kDef_LST103;
}  // namespace zhc::devices::gumax

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

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// The restored ColorCTLight surface: state + brightness kept, CT + color axes
// regained, and the lightingColorCtrl (0x0300) binding present.
void test_def_shape() {
    using namespace zhc::devices::gumax;
    const auto& def = kDef_LST103;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));

    assert(def_binds(def, 1, 0x0006));   // genOnOff
    assert(def_binds(def, 1, 0x0008));   // genLevelCtrl
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl (CT + color)

    // It is a settable light, not a read-only sensor/remote.
    assert(def.to_zigbee_count >= 4);    // on/off + brightness + colorTemp + color
}

// colorTemperature attr (0x0007, u16 mireds) on lightingColorCtrl -> color_temp.
void test_color_temp_decode() {
    using namespace zhc::devices::gumax;
    const auto& def = kDef_LST103;

    RuntimeContext c{};
    const std::array<std::uint8_t, 2> ct = {0xFE, 0x00};   // 254 mireds LE
    auto r = dispatch_zcl_ep(c, def, 0x0300, 1, "lightingColorCtrl",
                             attr_report(0x0007, 0x21,
                                         std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(r.any_matched);
    const Value* v = r.merged.find("color_temp");
    assert(v && v->type == ValueType::Uint && v->u == 254);
}

// currentX (0x0003) / currentY (0x0004) on lightingColorCtrl -> color_x/color_y.
void test_color_xy_decode() {
    using namespace zhc::devices::gumax;
    const auto& def = kDef_LST103;

    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> x = {0xFF, 0x7F};    // 0x7FFF LE
    auto rx = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0003, 0x21,
                                          std::span<const std::uint8_t>(x.data(), x.size())));
    assert(rx.any_matched);
    const Value* vx = rx.merged.find("color_x");
    assert(vx && vx->type == ValueType::Float);

    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> y = {0x00, 0x40};    // 0x4000 LE
    auto ry = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0004, 0x21,
                                          std::span<const std::uint8_t>(y.data(), y.size())));
    assert(ry.any_matched);
    const Value* vy = ry.merged.find("color_y");
    assert(vy && vy->type == ValueType::Float);
}

// currentHue (0x0000) / currentSaturation (0x0001) -> hue / saturation.
void test_color_hue_sat_decode() {
    using namespace zhc::devices::gumax;
    const auto& def = kDef_LST103;

    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> hue = {0x80};        // 128
    auto rh = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0000, 0x20,
                                          std::span<const std::uint8_t>(hue.data(), hue.size())));
    assert(rh.any_matched);
    const Value* vh = rh.merged.find("hue");
    assert(vh && vh->type == ValueType::Uint && vh->u == 128);

    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> sat = {0xC0};        // 192
    auto rs = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0001, 0x20,
                                          std::span<const std::uint8_t>(sat.data(), sat.size())));
    assert(rs.any_matched);
    const Value* vs = rs.merged.find("saturation");
    assert(vs && vs->type == ValueType::Uint && vs->u == 192);
}

}  // namespace

int main() {
    test_def_shape();
    test_color_temp_decode();
    test_color_xy_decode();
    test_color_hue_sat_decode();
    return 0;
}
