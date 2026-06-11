// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Gidealed family. The real z2m<->ez gap caught here:
//
//   * ZC05M (model "A11") is a full RGB+CCT LED strip controller. z2m
//     models it as
//       m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"]}})
//     i.e. on/off + brightness + colorTemp (mireds) + color (xy/hs).
//     The auto-port dropped BOTH the colorTemp and color axes, wiring
//     only kFzOnOff + kFzBrightness and binding only 0x0006 + 0x0008, so
//     every color / color_temp report was silently dropped and the
//     device could not be tuned. Fixed by restoring kFzColorTemperature
//     + kFzColor (+ tz writers), the color_temp / color_xy exposes, and
//     the lightingColorCtrl (0x0300) binding to match z2m exactly.
//
// z2m-source: zigbee-herdsman-converters/src/devices/gidealed.ts.

#include <array>
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

namespace zhc::devices::gidealed {
extern const PreparedDefinition kDef_ZC05M;
}  // namespace zhc::devices::gidealed

using namespace zhc;

namespace {

constexpr std::uint16_t GEN_ON_OFF     = 0x0006;
constexpr std::uint16_t GEN_LEVEL      = 0x0008;
constexpr std::uint16_t LIGHTING_COLOR = 0x0300;

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool has_uint(const DispatchResult& r, const char* key, std::uint32_t val) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Uint && v->u == val;
}

bool has_float(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Float;
}

// Attribute-report frame: fc=0x18 (server->client, global), tsn, cmd=0x0a
// (Report Attributes), then attr records [attr_id u16 LE, type, value].
std::vector<std::uint8_t> attr_report(std::span<const std::uint8_t> records) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0a};
    for (auto b : records) v.push_back(b);
    return v;
}

void test_zc05m() {
    const auto& def = devices::gidealed::kDef_ZC05M;

    // Full RGB+CCT light: all four axes exposed.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));   // <- dropped before the fix
    assert(def_exposes(def, "color_xy"));      // <- dropped before the fix
    // Not a remote / action device.
    assert(!def_exposes(def, "action"));

    // lightingColorCtrl must be bound (was missing before the fix).
    assert(def_binds(def, GEN_ON_OFF));
    assert(def_binds(def, GEN_LEVEL));
    assert(def_binds(def, LIGHTING_COLOR));

    // It is a controllable light: a write path exists.
    assert(def.to_zigbee != nullptr && def.to_zigbee_count == 4);

    // colorTemperature (attr 0x0007, u16=250 mireds) → color_temp:250.
    const std::uint8_t ct_rec[] = {0x07, 0x00, 0x21, 0xFA, 0x00};
    auto rct = dispatch_zcl(def, LIGHTING_COLOR, 1, attr_report(ct_rec));
    assert(rct.any_matched);
    assert(has_uint(rct, "color_temp", 250));

    // currentX (attr 0x0003) + currentY (attr 0x0004), u16 each.
    // 0x8000 / 65535 ≈ 0.5 → emitted as Float color_x / color_y.
    const std::uint8_t xy_rec[] = {
        0x03, 0x00, 0x21, 0x00, 0x80,   // currentX = 0x8000
        0x04, 0x00, 0x21, 0x00, 0x80,   // currentY = 0x8000
    };
    auto rxy = dispatch_zcl(def, LIGHTING_COLOR, 1, attr_report(xy_rec));
    assert(rxy.any_matched);
    assert(has_float(rxy, "color_x"));
    assert(has_float(rxy, "color_y"));

    // currentHue (attr 0x0000) + currentSaturation (attr 0x0001), u8 each.
    const std::uint8_t hs_rec[] = {
        0x00, 0x00, 0x20, 0x7F,   // currentHue = 127
        0x01, 0x00, 0x20, 0xFE,   // currentSaturation = 254
    };
    auto rhs = dispatch_zcl(def, LIGHTING_COLOR, 1, attr_report(hs_rec));
    assert(rhs.any_matched);
    assert(has_uint(rhs, "hue", 127));
    assert(has_uint(rhs, "saturation", 254));

    // Brightness still decodes (genLevelCtrl attr 0x0000 currentLevel).
    const std::uint8_t lvl_rec[] = {0x00, 0x00, 0x20, 0x80};
    auto rlvl = dispatch_zcl(def, GEN_LEVEL, 1, attr_report(lvl_rec));
    assert(rlvl.any_matched);
    assert(has_uint(rlvl, "brightness", 0x80));
}

}  // namespace

int main() {
    test_zc05m();
    return 0;
}
