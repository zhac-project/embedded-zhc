// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the GMY Smart Bulb family. The real z2m<->ez gap caught here:
//
//   * B07KG5KF5R ("CCT box") is z2m m.light({colorTemp: {range: undefined}}):
//     a tunable-white (correlated colour temperature) bulb exposing
//     state + brightness + color_temp, with fz/tz on lightingColorCtrl
//     (0x0300) colorTemperature (attr 0x0007, mireds). The Tier-1 auto-port
//     collapsed m.light() to on/off + brightness only, dropping the colorTemp
//     axis (no kFzColorTemperature, no color_temp expose, no 0x0300 bind).
//     Restored via kFzColorTemperature / kTzColorTemp + bind 0x0300.
//
//   * range: undefined → CCT-only (no color:true), so there must be NO phantom
//     hue/saturation/xy colour axis. Pinned by the regression guard below.
//
// z2m-source: zigbee-herdsman-converters/src/devices/gmy.ts #B07KG5KF5R.

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

namespace zhc::devices::gmy {
extern const PreparedDefinition kDef_B07KG5KF5R;
}  // namespace zhc::devices::gmy

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
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

constexpr std::uint16_t GEN_ON_OFF  = 0x0006;
constexpr std::uint16_t GEN_LEVEL   = 0x0008;
constexpr std::uint16_t LIGHT_COLOR = 0x0300;

// ── CCT bulb: state + brightness + color_temp, no phantom colour axis ──
void test_cct_box() {
    const auto& def = devices::gmy::kDef_B07KG5KF5R;

    // The restored tunable-white surface.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));   // was dropped by the auto-port

    // CCT-only: range:undefined, no color:true → NO hue/sat/xy colour axis.
    assert(!def_exposes(def, "color"));
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "color_hs"));

    // The colorTemp write path must be present (on/off + level + color_temp).
    assert(def.to_zigbee != nullptr && def.to_zigbee_count == 3);

    // 0x0300 lightingColorCtrl must be bound (colorTemp reporting).
    assert(def_binds(def, GEN_ON_OFF));
    assert(def_binds(def, GEN_LEVEL));
    assert(def_binds(def, LIGHT_COLOR));

    // genOnOff onOff (attr 0x0000, bool) → state.
    const std::uint8_t on[] = {0x01};
    auto rs = dispatch_zcl(def, GEN_ON_OFF, 1,
                           attr_report(0x0000, 0x10,
                                       std::span<const std::uint8_t>(on, 1)));
    assert(rs.any_matched);
    assert(rs.merged.find("state") != nullptr);

    // genLevelCtrl currentLevel (attr 0x0000, u8) → brightness.
    const std::uint8_t lvl[] = {0x80};
    auto rb = dispatch_zcl(def, GEN_LEVEL, 1,
                           attr_report(0x0000, 0x20,
                                       std::span<const std::uint8_t>(lvl, 1)));
    assert(rb.any_matched);
    assert(rb.merged.find("brightness") != nullptr);

    // lightingColorCtrl colorTemperature (attr 0x0007, u16 mireds) → color_temp.
    // 0x00FA = 250 mireds (4000K), inside the 2700-6500K range.
    const std::uint8_t ct[] = {0xFA, 0x00};
    auto rct = dispatch_zcl(def, LIGHT_COLOR, 1,
                            attr_report(0x0007, 0x21,
                                        std::span<const std::uint8_t>(ct, 2)));
    assert(rct.any_matched);
    const Value* v = rct.merged.find("color_temp");
    assert(v != nullptr);   // the axis that was silently dropped
}

}  // namespace

int main() {
    test_cct_box();
    return 0;
}
