// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Smart Home Pty family.
//
//   Dropped CT/colour — HGZB-07A is an RGBW downlight whose z2m extend is
//   m.light({colorTemp: {range: undefined}, color: true}) → on/off + level +
//   colorTemp + colour (xy). The auto-generator collapsed m.light() to
//   on/off + brightness only, dropping the lightingColorCtrl (0x0300) axes.
//   The graduated def restores kFzColorTemperature/kTzColorTemp +
//   kFzColor/kTzColor, the color_temp/color_x/color_y/hue/saturation exposes
//   and the 0x0300 binding.
//
//   Clean baseline — HGZB-20-DE is a plain m.onOff() power plug; it must carry
//   the on/off state and NO phantom colour/brightness/metering channels.

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

namespace zhc::devices::smart_home_pty {
extern const PreparedDefinition kDef_HGZB_07A;    // RGBW downlight (CT + colour)
extern const PreparedDefinition kDef_HGZB_20_DE;  // plain on/off plug
}  // namespace zhc::devices::smart_home_pty

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

// ── HGZB-07A: the dropped CT + colour axes are restored and decode. ─────────
void test_hgzb_07a_light() {
    using namespace zhc::devices::smart_home_pty;
    const auto& def = kDef_HGZB_07A;

    // Surface: on/off + level kept, CT + colour restored.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));

    // lightingColorCtrl binding present (was missing).
    assert(def_binds(def, 1, 0x0006));   // genOnOff
    assert(def_binds(def, 1, 0x0008));   // genLevelCtrl
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl

    // colorTemperature (attr 0x0007, u16 mireds) → "color_temp".
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> ct = {0x9A, 0x01};   // 410 LE
    auto ctr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                               attr_report(0x0007, 0x21,
                                           std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(ctr.any_matched);
    assert(ctr.merged.find("color_temp") != nullptr);

    // currentX (attr 0x0003, u16) → "color_x".
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> cx = {0x00, 0x80};   // 0x8000 LE
    auto cxr = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                               attr_report(0x0003, 0x21,
                                           std::span<const std::uint8_t>(cx.data(), cx.size())));
    assert(cxr.any_matched);
    assert(cxr.merged.find("color_x") != nullptr);

    // genOnOff state report still decodes (on/off axis intact).
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c3, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    const Value* sv = sr.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && sv->b);
}

// ── HGZB-20-DE: plain on/off plug — no phantom colour / metering. ───────────
void test_hgzb_20_de_plug() {
    using namespace zhc::devices::smart_home_pty;
    const auto& def = kDef_HGZB_20_DE;

    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));
    assert(!def_exposes(def, "energy"));
    assert(!def_exposes(def, "power"));
    assert(def_binds(def, 1, 0x0006));

    // genOnOff state report decodes.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> off = {0x00};
    auto sr = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(off.data(), off.size())));
    assert(sr.any_matched);
    const Value* sv = sr.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && !sv->b);
}

}  // namespace

int main() {
    test_hgzb_07a_light();
    test_hgzb_20_de_plug();
    return 0;
}
