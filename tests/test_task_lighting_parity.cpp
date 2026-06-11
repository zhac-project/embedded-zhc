// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Task Lighting. The auto-generated port collapsed the device's
// m.light({colorTemp: {range: [150, 500]}, color: true}) extend down to a bare
// on/off + brightness bundle, dropping the colorTemp and color (xy) axes.
//
//   T-TWAV-60W-PSR ("WAV Smart Receiver"): full ColorCTLight restored —
//     * colorTemp range 150-500 mired -> kFzColorTemperature -> color_temp
//     * color: true => modes:["xy"] (modernExtend default) -> kFzColor ->
//       color_x / color_y (NO hue/saturation, since "hs" not in modes)
//     * lightingColorCtrl (0x0300) binding added.

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

namespace zhc::devices::task_lighting {
extern const PreparedDefinition kDef_T_TWAV_60W_PSR;  // color + colorTemp light
}  // namespace zhc::devices::task_lighting

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

void test_exposes_and_bindings() {
    using namespace zhc::devices::task_lighting;
    const auto& def = kDef_T_TWAV_60W_PSR;

    // on/off + brightness kept.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // colorTemp + color (xy) axes restored.
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    // color: true => xy mode only — NO hue/saturation surface.
    assert(!def_exposes(def, "hue"));
    assert(!def_exposes(def, "saturation"));

    // lightingColorCtrl binding added alongside on/off + level.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));

    // settable light => write path present.
    assert(def.to_zigbee_count >= 4);
}

void test_color_temp_decode() {
    using namespace zhc::devices::task_lighting;
    const auto& def = kDef_T_TWAV_60W_PSR;

    // colorTemperature (0x0007, u16 mireds) on lightingColorCtrl -> color_temp.
    // 370 mired -> 0x0172 LE.
    RuntimeContext c{};
    const std::array<std::uint8_t, 2> ct = {0x72, 0x01};
    auto r = dispatch_zcl_ep(c, def, 0x0300, 1, "lightingColorCtrl",
                             attr_report(0x0007, 0x21,
                                         std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(r.any_matched);
    assert(r.merged.find("color_temp") != nullptr);
}

void test_color_xy_decode() {
    using namespace zhc::devices::task_lighting;
    const auto& def = kDef_T_TWAV_60W_PSR;

    // currentX (0x0003, u16) on lightingColorCtrl -> color_x.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> x = {0x34, 0x52};  // 0x5234
    auto rx = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0003, 0x21,
                                          std::span<const std::uint8_t>(x.data(), x.size())));
    assert(rx.any_matched);
    assert(rx.merged.find("color_x") != nullptr);

    // currentY (0x0004, u16) on lightingColorCtrl -> color_y.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> y = {0xCD, 0x3A};  // 0x3ACD
    auto ry = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0004, 0x21,
                                          std::span<const std::uint8_t>(y.data(), y.size())));
    assert(ry.any_matched);
    assert(ry.merged.find("color_y") != nullptr);
}

}  // namespace

int main() {
    test_exposes_and_bindings();
    test_color_temp_decode();
    test_color_xy_decode();
    return 0;
}
