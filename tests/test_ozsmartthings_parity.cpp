// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Oz Smart Things family (one device). Real z2m<->ez gap
// pinned here after the fix:
//
//   Wrong class / phantom bundle — DL41-03-10-R-ZB is z2m
//   tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]}, color:true}) →
//   modernExtend.light() with both the colorTemp and color axes → on/off +
//   brightness + colorTemp (mireds) + color (xy + hs). The auto-port collapsed
//   it to a battery + on/off SENSOR stub (phantom battery/voltage exposes,
//   genPowerCfg 0x0001 bind, no brightness/color), dropping the whole light.
//   Restored kFzOnOff + kFzBrightness + kFzColorTemperature + kFzColor
//   (+ kTz* write path), state/brightness/color_temp/color_x/color_y/hue/
//   saturation exposes, bind 0x0006/0x0008/0x0300, no battery.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ozsmartthings.ts

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

namespace zhc::devices::ozsmartthings {
extern const PreparedDefinition kDef_DL41_03_10_R_ZB;  // RGBW: on/off+bri+CT+color
}  // namespace zhc::devices::ozsmartthings

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

bool b_true(const Value* v) { return v && v->type == ValueType::Bool && v->b; }

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

// ── DL41-03-10-R-ZB: full RGBW light, sensor stub eliminated. ───────────────
void test_rgbw_downlight() {
    using namespace zhc::devices::ozsmartthings;
    const auto& def = kDef_DL41_03_10_R_ZB;

    // Real light class restored.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));    // was dropped
    assert(def_exposes(def, "color_temp"));    // was dropped
    assert(def_exposes(def, "color_x"));       // was dropped
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));
    // Phantom sensor exposes gone.
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));

    // Bindings: light clusters present, phantom genPowerCfg gone.
    assert(def_binds(def, 1, 0x0006));         // genOnOff
    assert(def_binds(def, 1, 0x0008));         // genLevelCtrl
    assert(def_binds(def, 1, 0x0300));         // lightingColorCtrl
    assert(!def_binds(def, 1, 0x0001));        // genPowerCfg phantom removed
    assert(def.to_zigbee_count >= 4);          // on/off + bri + CT + color

    // on/off (genOnOff onOff 0x0000) -> state true.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    assert(b_true(sr.merged.find("state")));

    // brightness (genLevelCtrl currentLevel 0x0000) -> brightness.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto br = dispatch_zcl_ep(c2, def, 0x0008, 1, "genLevelCtrl",
                             attr_report(0x0000, 0x20, std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    assert(br.any_matched);
    assert(br.merged.find("brightness") != nullptr);

    // colorTemperature (0x0007, u16 mireds=370) -> color_temp.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 2> ct = {0x72, 0x01};   // 370 LE
    auto cr = dispatch_zcl_ep(c3, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0007, 0x21, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(cr.any_matched);
    const Value* cv = cr.merged.find("color_temp");
    assert(cv && cv->type == ValueType::Uint && cv->u == 370);

    // currentX (0x0003, u16) -> color_x.
    RuntimeContext c4{};
    const std::array<std::uint8_t, 2> x = {0x00, 0x80};    // 0x8000
    auto xr = dispatch_zcl_ep(c4, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0003, 0x21, std::span<const std::uint8_t>(x.data(), x.size())));
    assert(xr.any_matched);
    assert(xr.merged.find("color_x") != nullptr);
}

}  // namespace

int main() {
    test_rgbw_downlight();
    return 0;
}
