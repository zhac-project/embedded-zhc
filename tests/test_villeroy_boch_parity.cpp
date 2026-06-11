// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Villeroy & Boch family. Two real z2m<->ez gaps:
//
//   * C5850000 (Subway 3.0 home automation kit, zigbeeModel 5991711/5991713)
//   * C0040000 (mirror home automation kit, zigbeeModel EC1300)
//
// Both are driven in z2m by m.light({colorTemp:{range:[160,450]}}), i.e. a
// tunable-white (CCT) light: on/off + brightness + colorTemperature on
// lightingColorCtrl (0x0300). The auto-port wired only kFzOnOff +
// kFzBrightness and dropped the entire 0x0300 colour-temperature axis.
// The fix restores kFzColorTemperature (+ kTzColorTemp) and the color_temp
// expose. Neither device has a color: arg, so there is NO hue/saturation/
// color_x/color_y axis (guarded below).
//
// z2m-source: zigbee-herdsman-converters/src/devices/villeroy_boch.ts.

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

namespace zhc::devices::villeroy_boch {
extern const PreparedDefinition kDef_C0040000;
extern const PreparedDefinition kDef_C5850000;
}  // namespace zhc::devices::villeroy_boch

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

// ZCL attribute report (cluster-specific=false, Report=0x0A, one record).
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

bool has_binding(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

constexpr std::uint16_t COLOR_CTRL = 0x0300;

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

// Shared assertions: CCT light surface (on/off + brightness + color_temp),
// NO colour axis, 0x0300 binding present, and a live colorTemperature decode.
void check_cct(const PreparedDefinition& def) {
    // Light base.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // Colour-temperature axis restored.
    assert(def_exposes(def, "color_temp"));
    // No colour (xy/hs) axis — m.light() had no color: arg.
    assert(!def_exposes(def, "hue"));
    assert(!def_exposes(def, "saturation"));
    assert(!def_exposes(def, "color_x"));
    assert(!def_exposes(def, "color_y"));
    // lightingColorCtrl binding for the CT reporting setup.
    assert(has_binding(def, COLOR_CTRL));

    // lightingColorCtrl colorTemperature (attr 0x0007, u16 mireds) → color_temp.
    auto rct = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0007, 0x21, u16(300)));
    assert(rct.any_matched);
    const Value* ct = rct.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 300);
}

}  // namespace

int main() {
    check_cct(devices::villeroy_boch::kDef_C5850000);
    check_cct(devices::villeroy_boch::kDef_C0040000);
    return 0;
}
