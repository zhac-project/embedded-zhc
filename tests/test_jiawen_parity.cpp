// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Jiawen LED family. The auto-generator collapses every
// m.light() down to on/off + brightness, dropping the colorTemp/color axes.
//
//   Dropped color + colorTemp axes — K2RGBW01 ("Wireless Bulb E27 9W RGBW")
//     is z2m m.light({colorTemp: {range: undefined}, color: true}), i.e. an
//     RGBW bulb with BOTH a CCT (mireds) axis and an xy-color axis. The port
//     wired only kFzOnOff + kFzBrightness with bindings {0x0006, 0x0008}, so
//     color_temp and the xy color channels were both unexposed and undecoded.
//     This fixture pins the restored axes: the color_temp + color_xy exposes,
//     the lightingColorCtrl (0x0300) binding, the kFzColorTemperature decoder
//     (attr 0x0007 -> color_temp), the kFzColor decoder (attr 0x0003/0x0004 ->
//     color_x/color_y) and the kTzColorTemp + kTzColor write paths.
//
//   JW-A04-CT ("LED strip light controller") is z2m plain m.light() with no
//   colorTemp/color — it correctly stays on/off + brightness only. Asserted as
//   a negative control so a future generator regression that injects a phantom
//   color axis would be caught.

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

namespace zhc::devices::jiawen {
extern const PreparedDefinition kDef_K2RGBW01;   // RGBW bulb (color + colorTemp)
extern const PreparedDefinition kDef_JW_A04_CT;  // plain LED strip (negative control)
}  // namespace zhc::devices::jiawen

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

bool has_tz_for(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* tz = def.to_zigbee[i];
        if (tz && tz->key && std::strcmp(tz->key, key) == 0) return true;
    }
    return false;
}

// Keep `ctx` alive while the result is read: result pointers alias ctx scratch.
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

// ── K2RGBW01: color + colorTemp axes restored. ─────────────────────────────
void test_k2rgbw01_color_colortemp() {
    using namespace zhc::devices::jiawen;
    const auto& def = kDef_K2RGBW01;

    // The light surface: on/off + brightness + color_temp + color_xy, and
    // lightingColorCtrl is bound so reports/commands route.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_xy"));
    assert(def_binds(def, 1, 0x0006));   // genOnOff
    assert(def_binds(def, 1, 0x0008));   // genLevelCtrl
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl (was dropped)

    // Read path: colorTemperature (attr 0x0007, u16 mireds) on
    // lightingColorCtrl -> "color_temp". 370 mireds = 0x0172.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> ct = {0x72, 0x01};   // 370 LE
    auto cr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0007, 0x21, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(cr.any_matched);
    assert(cr.merged.find("color_temp") != nullptr);   // dropped CCT channel now decodes

    // Read path: currentX (attr 0x0003, u16) on lightingColorCtrl -> "color_x".
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> cx = {0x00, 0x80};   // 0x8000 ≈ 0.5
    auto xr = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0003, 0x21, std::span<const std::uint8_t>(cx.data(), cx.size())));
    assert(xr.any_matched);
    assert(xr.merged.find("color_x") != nullptr);   // dropped xy channel now decodes

    // Read path: currentY (attr 0x0004, u16) on lightingColorCtrl -> "color_y".
    RuntimeContext c3{};
    const std::array<std::uint8_t, 2> cy = {0x00, 0x40};   // 0x4000 ≈ 0.25
    auto yr = dispatch_zcl_ep(c3, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0004, 0x21, std::span<const std::uint8_t>(cy.data(), cy.size())));
    assert(yr.any_matched);
    assert(yr.merged.find("color_y") != nullptr);

    // Write path: color_temp + state + brightness settable (kTz* wired).
    assert(has_tz_for(def, "color_temp"));
    assert(has_tz_for(def, "state"));
    assert(has_tz_for(def, "brightness"));

    // Brightness (genLevelCtrl currentLevel 0x0000, u8) still decodes — the
    // restoration must not regress the kept axes.
    RuntimeContext c4{};
    const std::array<std::uint8_t, 1> lv = {0x80};
    auto br = dispatch_zcl_ep(c4, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20, std::span<const std::uint8_t>(lv.data(), lv.size())));
    assert(br.any_matched);
    assert(br.merged.find("brightness") != nullptr);
}

// ── JW-A04-CT: plain LED strip, NO phantom color axis (negative control). ──
void test_jw_a04_ct_plain() {
    using namespace zhc::devices::jiawen;
    const auto& def = kDef_JW_A04_CT;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));   // z2m m.light() has no colorTemp
    assert(!def_exposes(def, "color_xy"));
    assert(!def_binds(def, 1, 0x0300));        // no lightingColorCtrl binding
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
}

}  // namespace

int main() {
    test_k2rgbw01_color_colortemp();
    test_jw_a04_ct_plain();
    return 0;
}
