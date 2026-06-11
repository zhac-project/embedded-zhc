// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Letsled HLC929-Z-RGBW-4C-IA-OTA-3.0 RGBW down light.
//
//   Dropped color/colorTemp axis — z2m models this as a full RGBW light:
//     extend: [m.light({colorTemp: {range: [153, 370]}, color: true})]
//   i.e. on/off + level + colorTemp + color (xy + hs). The auto-generator
//   collapsed m.light() to on/off + brightness only, dropping the entire
//   0x0300 lightingColorCtrl axis (both CT and color). This fixture pins the
//   restored CT/color surface + decoders.

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

namespace zhc::devices::letsled {
extern const PreparedDefinition kDef_HLC929_Z_RGBW_4C_IA_OTA_3_0;
}  // namespace zhc::devices::letsled

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

bool def_has_tz_cluster(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i)
        if (def.to_zigbee[i] && def.to_zigbee[i]->cluster_id == cluster_id) return true;
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

// ── Surface: full RGBW light, not on/off + brightness only. ────────────────
void test_color_surface() {
    using namespace zhc::devices::letsled;
    const auto& def = kDef_HLC929_Z_RGBW_4C_IA_OTA_3_0;

    // on/off + level kept.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // colorTemp + color axes restored.
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));

    // bindings: onOff + level + colorCtrl.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl (CT + color)

    // write path includes the colorCtrl tz (color + colorTemp).
    assert(def_has_tz_cluster(def, 0x0006));
    assert(def_has_tz_cluster(def, 0x0008));
    assert(def_has_tz_cluster(def, 0x0300));
}

// ── fz: colorTemp (0x0007) decodes to color_temp. ──────────────────────────
void test_color_temp_decode() {
    using namespace zhc::devices::letsled;
    const auto& def = kDef_HLC929_Z_RGBW_4C_IA_OTA_3_0;

    // colorTemperature (0x0007, u16) = 250 mireds.
    RuntimeContext c{};
    const std::array<std::uint8_t, 2> ct = {0xFA, 0x00};   // 250 LE
    auto r = dispatch_zcl_ep(c, def, 0x0300, 1, "lightingColorCtrl",
                             attr_report(0x0007, 0x21,
                                         std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(r.any_matched);
    assert(r.merged.find("color_temp") != nullptr);
}

// ── fz: hue/saturation (0x0000/0x0001) decode to hue/saturation. ───────────
void test_color_hs_decode() {
    using namespace zhc::devices::letsled;
    const auto& def = kDef_HLC929_Z_RGBW_4C_IA_OTA_3_0;

    // currentHue (0x0000, u8) = 120.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> hue = {120};
    auto hr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0000, 0x20,
                                          std::span<const std::uint8_t>(hue.data(), hue.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("hue") != nullptr);

    // currentSaturation (0x0001, u8) = 200.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> sat = {200};
    auto sr = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0001, 0x20,
                                          std::span<const std::uint8_t>(sat.data(), sat.size())));
    assert(sr.any_matched);
    assert(sr.merged.find("saturation") != nullptr);
}

// ── on/off + brightness still decode (regression). ─────────────────────────
void test_onoff_brightness_decode() {
    using namespace zhc::devices::letsled;
    const auto& def = kDef_HLC929_Z_RGBW_4C_IA_OTA_3_0;

    // genOnOff onOff (0x0000, bool) = 1 -> state true.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    const Value* st = sr.merged.find("state");
    assert(st && st->type == ValueType::Bool && st->b);

    // genLevelCtrl currentLevel (0x0000, u8) = 128 -> brightness.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> lv = {128};
    auto br = dispatch_zcl_ep(c2, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20,
                                          std::span<const std::uint8_t>(lv.data(), lv.size())));
    assert(br.any_matched);
    assert(br.merged.find("brightness") != nullptr);
}

}  // namespace

int main() {
    test_color_surface();
    test_color_temp_decode();
    test_color_hs_decode();
    test_onoff_brightness_decode();
    return 0;
}
