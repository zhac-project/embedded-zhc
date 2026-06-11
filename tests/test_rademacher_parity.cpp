// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Rademacher addZ colour-LED family. All three z2m defs
// are full RGB+CCT bulbs:
//   * 35104001    m.light({colorTemp:{range:[153,555]}, color:true})
//   * 35144001    m.light({colorTemp:{range:[153,555]}, color:true})
//   * RDM-35274001 m.light({colorTemp:{range:[153,555]},
//                           color:{modes:["xy","hs"], enhancedHue:true}})
//
// The auto-generator collapses every m.light() to on/off + brightness only,
// dropping BOTH the colour-temperature and colour axes. The fix re-wires the
// generic kFzColorTemperature (lightingColorCtrl attr 0x0007 -> color_temp) and
// kFzColor (attr 0x0000 hue / 0x0001 saturation / 0x0003 color_x / 0x0004
// color_y) decoders, restores the matching exposes, and adds the
// lightingColorCtrl (0x0300) binding. This fixture pins all three.

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

namespace zhc::devices::rademacher {
extern const PreparedDefinition kDef_D35104001;
extern const PreparedDefinition kDef_D35144001;
extern const PreparedDefinition kDef_RDM_35274001;
}  // namespace zhc::devices::rademacher

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

// Keep `ctx` alive while the result is read: result pointers alias ctx scratch.
DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
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

// Every Rademacher bulb is a full RGB+CCT ColorCTLight. Assert the static
// surface (exposes + bindings + write paths) and that each colour axis actually
// decodes from lightingColorCtrl reports.
void check_color_ct_light(const PreparedDefinition& def) {
    // Static surface: all five colour-related exposes restored, plus base.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));

    // Bindings: on/off + level + colour cluster.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl (was dropped)

    // Write paths exist for all four light axes (state/brightness/CT/colour).
    assert(def.to_zigbee_count >= 4);

    // colorTemperature (attr 0x0007, u16 mireds) -> color_temp.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> ct = {0x9A, 0x01};   // 410 LE
    auto ctr = dispatch_zcl(c1, def, 0x0300, "lightingColorCtrl",
                            attr_report(0x0007, 0x21, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(ctr.any_matched);
    assert(ctr.merged.find("color_temp") != nullptr);

    // currentX (attr 0x0003, u16) -> color_x.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> cx = {0x34, 0x52};   // 0x5234 LE
    auto cxr = dispatch_zcl(c2, def, 0x0300, "lightingColorCtrl",
                            attr_report(0x0003, 0x21, std::span<const std::uint8_t>(cx.data(), cx.size())));
    assert(cxr.any_matched);
    assert(cxr.merged.find("color_x") != nullptr);

    // currentHue (attr 0x0000, u8) -> hue.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> hv = {0x7F};
    auto hr = dispatch_zcl(c3, def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0000, 0x20, std::span<const std::uint8_t>(hv.data(), hv.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("hue") != nullptr);

    // currentSaturation (attr 0x0001, u8) -> saturation.
    RuntimeContext c4{};
    const std::array<std::uint8_t, 1> sv = {0xC8};
    auto sr = dispatch_zcl(c4, def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0001, 0x20, std::span<const std::uint8_t>(sv.data(), sv.size())));
    assert(sr.any_matched);
    assert(sr.merged.find("saturation") != nullptr);

    // Base on/off relay still decodes (genOnOff attr 0x0000).
    RuntimeContext c5{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto onr = dispatch_zcl(c5, def, 0x0006, "genOnOff",
                            attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(onr.any_matched);
    assert(onr.merged.find("state") != nullptr);
}

void test_all_bulbs() {
    using namespace zhc::devices::rademacher;
    check_color_ct_light(kDef_D35104001);
    check_color_ct_light(kDef_D35144001);
    check_color_ct_light(kDef_RDM_35274001);
}

}  // namespace

int main() {
    test_all_bulbs();
    return 0;
}
