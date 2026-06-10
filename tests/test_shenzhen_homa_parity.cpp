// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Shenzhen Homa LED-driver family. The auto-generated
// ports collapsed every m.light() to on/off + brightness only, dropping the
// 0x0300 (lightingColorCtrl) axis that z2m wires whenever color or colorTemp
// is requested. This fixture pins the restored decoders:
//
//   Missing color (xy) —
//     * HOMA1001_RGB  : m.light({color: true})                    -> kFzColor
//     * HOMA1001_RGBW : m.deviceEndpoints({white:10, rgb:11}) +
//                       m.light({endpointNames:[...], color:true}) -> kFzColor,
//                       color report on EP 11 suffixes to color_x_rgb/_y_rgb
//   Missing colorTemp —
//     * HLD503-Z-CT   : m.light({colorTemp:{range:[150,500]}})    -> kFzColorTemperature
//
// z2m's m.light() default carries NEITHER color NOR colorTemp (both args
// default undefined), so the plain m.light() drivers (CT/SC/HLC610/HLC821/
// HLC833/HLD812) stay on/off + brightness — those ports were already correct
// and are not touched. HLC614-ZLL (m.onOff x3) keeps its l1/l2/l3 endpoint_map.

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

namespace zhc::devices::shenzhen_homa {
extern const PreparedDefinition kDef_HOMA1001_RGB;   // color (xy), single EP
extern const PreparedDefinition kDef_HOMA1001_RGBW;  // color (xy), white/rgb EP map
extern const PreparedDefinition kDef_HLD503_Z_CT;    // color_temp
extern const PreparedDefinition kDef_HOMA1001_CT;    // plain light (no CT/color)
}  // namespace zhc::devices::shenzhen_homa

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

// Keep `ctx` alive while the result is read: multi-endpoint suffixing copies
// keys into RuntimeContext scratch, so result pointers alias `ctx`.
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

// ZCL attribute report (fc=0x18 S->C, tsn, cmd=0x0A) carrying one u16 record.
std::vector<std::uint8_t> attr_report_u16(std::uint16_t attr_id, std::uint16_t value) {
    return {0x18, 0x42, 0x0A,
            static_cast<std::uint8_t>(attr_id & 0xFF),
            static_cast<std::uint8_t>(attr_id >> 8),
            0x21,  // u16
            static_cast<std::uint8_t>(value & 0xFF),
            static_cast<std::uint8_t>(value >> 8)};
}

// lightingColorCtrl report carrying currentX (0x0003) + currentY (0x0004).
std::vector<std::uint8_t> color_xy_report(std::uint16_t cx, std::uint16_t cy) {
    return {0x18, 0x42, 0x0A,
            0x03, 0x00, 0x21, static_cast<std::uint8_t>(cx & 0xFF), static_cast<std::uint8_t>(cx >> 8),
            0x04, 0x00, 0x21, static_cast<std::uint8_t>(cy & 0xFF), static_cast<std::uint8_t>(cy >> 8)};
}

// ── HOMA1001_RGB: single-endpoint color light (bare color_x/color_y). ──────
void test_homa1001_rgb() {
    using namespace zhc::devices::shenzhen_homa;
    const auto& def = kDef_HOMA1001_RGB;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_binds(def, 11, 0x0300));  // color bound on the rgb endpoint

    auto rep = color_xy_report(0x4000, 0x8000);
    RuntimeContext c{};
    auto r = dispatch_zcl_ep(c, def, 0x0300, 11, "lightingColorCtrl",
                             {rep.data(), rep.size()});
    assert(r.any_matched);
    const Value* x = r.merged.find("color_x");
    const Value* y = r.merged.find("color_y");
    assert(x && x->type == ValueType::Float);
    assert(y && y->type == ValueType::Float);
    // No endpoint_map on this def -> keys stay bare.
    assert(r.merged.find("color_x_rgb") == nullptr);
}

// ── HOMA1001_RGBW: white/rgb endpoint map; color on EP 11 -> *_rgb. ────────
void test_homa1001_rgbw() {
    using namespace zhc::devices::shenzhen_homa;
    const auto& def = kDef_HOMA1001_RGBW;
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def.endpoint_map_count == 2);
    assert(def_binds(def, 11, 0x0300));  // color bound on the rgb endpoint

    auto rep = color_xy_report(0x4000, 0x8000);
    RuntimeContext c{};
    auto r = dispatch_zcl_ep(c, def, 0x0300, 11, "lightingColorCtrl",
                             {rep.data(), rep.size()});
    assert(r.any_matched);
    // endpoint_map present -> keys suffixed by the EP-11 label "rgb".
    const Value* x = r.merged.find("color_x_rgb");
    const Value* y = r.merged.find("color_y_rgb");
    assert(x && x->type == ValueType::Float);
    assert(y && y->type == ValueType::Float);
    assert(r.merged.find("color_x") == nullptr);   // bare key was suffixed away
}

// ── HLD503-Z-CT: colorTemp light (color_temp from attr 0x0007). ────────────
void test_hld503_ct() {
    using namespace zhc::devices::shenzhen_homa;
    const auto& def = kDef_HLD503_Z_CT;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));   // CT-only, no xy axis
    assert(def_binds(def, 1, 0x0300));

    auto rep = attr_report_u16(0x0007, 250);  // colorTemperature = 250 mireds
    RuntimeContext c{};
    auto r = dispatch_zcl_ep(c, def, 0x0300, 1, "lightingColorCtrl",
                             {rep.data(), rep.size()});
    assert(r.any_matched);
    const Value* ct = r.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 250);
}

// ── HOMA1001_CT: plain m.light() carries NO color/colorTemp axis. ──────────
void test_homa1001_ct_no_color() {
    using namespace zhc::devices::shenzhen_homa;
    const auto& def = kDef_HOMA1001_CT;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));
}

}  // namespace

int main() {
    test_homa1001_rgb();
    test_homa1001_rgbw();
    test_hld503_ct();
    test_homa1001_ct_no_color();
    return 0;
}
