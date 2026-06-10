// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Domraem DOM-Z-105P "5 in 1" LED controller family.
// Every variant is a plain z2m m.light(...) device, but the auto-generator
// collapses m.light() to on/off + brightness only, dropping the colorTemp
// and/or color axes. z2m wires the full set per modelID:
//
//   RGB    m.light({color:{modes:["hs"]}})                  -> + hue/saturation
//   RGBW   m.light({colorTemp:[158,495], color:{xy,hs}})    -> + CT + xy + hs
//   RGBWC  m.light({colorTemp:[158,495], color:{xy,hs}})    -> + CT + xy + hs
//   CCT    m.light({colorTemp:[158,495]})                   -> + color_temp
//   WW/CW  m.light({colorTemp:[158,500]})                   -> + color_temp
//   DIMMER m.light()                                        -> on/off + level (clean)
//
// This fixture pins that the restored converters decode the regained axes and
// that each variant carries exactly the axes z2m declares (RGB has no
// color_temp; CCT/WW-CW have no hue/color_x).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::domraem {
extern const PreparedDefinition kDef_DOM_Z_105P_RGB;     // hs color, no CT
extern const PreparedDefinition kDef_DOM_Z_105P_RGBW;    // CT + xy + hs
extern const PreparedDefinition kDef_DOM_Z_105P_RGBCCT;  // CT + xy + hs (modelID RGBWC)
extern const PreparedDefinition kDef_DOM_Z_105P_CCT;     // CT only
extern const PreparedDefinition kDef_DOM_Z_105P_WW_CW;   // CT only
}  // namespace zhc::devices::domraem

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

bool has_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

bool has_tz(const PreparedDefinition& def, const TzConverter* conv) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i)
        if (def.to_zigbee[i] == conv) return true;
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

// lightingColorCtrl (0x0300) colorTemperature (0x0007, u16 mireds) -> color_temp.
void check_color_temp(const PreparedDefinition& def) {
    assert(def_exposes(def, "color_temp"));
    assert(def_binds(def, 1, 0x0300));
    assert(has_fz(def, &::zhc::generic::kFzColorTemperature));
    assert(has_tz(def, &::zhc::generic::kTzColorTemp));

    RuntimeContext c{};
    const std::array<std::uint8_t, 2> ct = {0x9A, 0x01};  // 410 mireds LE
    auto r = dispatch_zcl_ep(c, def, 0x0300, 1, "lightingColorCtrl",
                             attr_report(0x0007, 0x21,
                                         std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(r.any_matched);
    assert(r.merged.find("color_temp") != nullptr);
}

// lightingColorCtrl currentHue (0x0000) + currentSaturation (0x0001) -> hue/saturation.
void check_hue_sat(const PreparedDefinition& def) {
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));
    assert(def_binds(def, 1, 0x0300));
    assert(has_fz(def, &::zhc::generic::kFzColor));
    assert(has_tz(def, &::zhc::generic::kTzColor));

    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> h = {0x80};  // hue 128
    auto hr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0000, 0x20,
                                          std::span<const std::uint8_t>(h.data(), h.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("hue") != nullptr);

    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> s = {0xC0};  // saturation 192
    auto sr = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0001, 0x20,
                                          std::span<const std::uint8_t>(s.data(), s.size())));
    assert(sr.any_matched);
    assert(sr.merged.find("saturation") != nullptr);
}

// lightingColorCtrl currentX (0x0003) + currentY (0x0004) -> color_x/color_y.
void check_xy(const PreparedDefinition& def) {
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));

    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> x = {0x00, 0x80};  // 0x8000 LE
    auto xr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0003, 0x21,
                                          std::span<const std::uint8_t>(x.data(), x.size())));
    assert(xr.any_matched);
    assert(xr.merged.find("color_x") != nullptr);

    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> y = {0x00, 0x40};  // 0x4000 LE
    auto yr = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0004, 0x21,
                                          std::span<const std::uint8_t>(y.data(), y.size())));
    assert(yr.any_matched);
    assert(yr.merged.find("color_y") != nullptr);
}

// Every variant keeps the base on/off + brightness surface + bindings.
void check_base(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
}

void test_rgb() {
    using namespace zhc::devices::domraem;
    const auto& def = kDef_DOM_Z_105P_RGB;
    check_base(def);
    check_hue_sat(def);
    // RGB is hs-only: NO colorTemp, NO xy axes.
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));
    assert(!has_fz(def, &::zhc::generic::kFzColorTemperature));
}

void test_rgbw() {
    using namespace zhc::devices::domraem;
    for (const PreparedDefinition* defp : {&kDef_DOM_Z_105P_RGBW, &kDef_DOM_Z_105P_RGBCCT}) {
        const auto& def = *defp;
        check_base(def);
        check_color_temp(def);
        check_hue_sat(def);
        check_xy(def);
    }
}

void test_cct() {
    using namespace zhc::devices::domraem;
    for (const PreparedDefinition* defp : {&kDef_DOM_Z_105P_CCT, &kDef_DOM_Z_105P_WW_CW}) {
        const auto& def = *defp;
        check_base(def);
        check_color_temp(def);
        // Tunable-white only: NO color axes.
        assert(!def_exposes(def, "hue"));
        assert(!def_exposes(def, "saturation"));
        assert(!def_exposes(def, "color_x"));
        assert(!has_fz(def, &::zhc::generic::kFzColor));
    }
}

}  // namespace

int main() {
    test_rgb();
    test_rgbw();
    test_cct();
    return 0;
}
