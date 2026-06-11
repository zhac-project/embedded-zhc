// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Alchemy LED-luminaire family. Two real z2m<->ez
// gaps caught here — both the dropped optical-control axis on m.light():
//
//   * AL8TC13W-AP — "Downlight with tuneable white". z2m drives it with
//     m.light({colorTemp: {range: [153, 370]}}), i.e. CCT-only colour
//     temperature. The auto-port wired only kFzOnOff + kFzBrightness and
//     dropped the lightingColorCtrl (0x0300) colorTemp channel. The fix
//     restores kFzColorTemperature (+ kTzColorTemp) and the color_temp
//     expose. NO xy/hs colour axis (tuneable-white only).
//
//   * AL8RGB13W-AP — "Downlight RGBW". z2m drives it with
//     m.light({colorTemp: {range: [153, 370]}, color: true}), i.e. the
//     full colour + colour-temperature axis. The auto-port again wired
//     only on/off + brightness, dropping the whole 0x0300 channel. The
//     fix restores kFzColor + kFzColorTemperature (+ kTzColor +
//     kTzColorTemp) and the color_temp / color_xy / hue / saturation /
//     color_x / color_y surface.
//
// z2m-source: zigbee-herdsman-converters/src/devices/alchemy.ts.
//
// Both z2m defs are plain modernExtend m.light(); neither carries a
// fromZigbee[] list, IAS, action, thermostat or any other gap class.

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

namespace zhc::devices::alchemy {
extern const PreparedDefinition kDef_AL8TC13W_AP;
extern const PreparedDefinition kDef_AL8RGB13W_AP;
}  // namespace zhc::devices::alchemy

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

constexpr std::uint16_t GEN_ON_OFF = 0x0006;
constexpr std::uint16_t COLOR_CTRL = 0x0300;

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

// ── AL8TC13W-AP: tuneable white — restored CCT axis, NO colour ───────
void check_tc() {
    const auto& def = devices::alchemy::kDef_AL8TC13W_AP;

    // Light base stays.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // colorTemp axis restored.
    assert(def_exposes(def, "color_temp"));
    // Tuneable-white only: no xy/hs colour surface.
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "hue"));
    assert(!def_exposes(def, "saturation"));
    // lightingColorCtrl binding present.
    assert(def_binds(def, COLOR_CTRL));

    // genOnOff On/Off still decodes to state.
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, attr_report(0x0000, 0x10, std::vector<std::uint8_t>{0x01}));
    assert(ron.any_matched);
    const Value* st = ron.merged.find("state");
    assert(st);

    // lightingColorCtrl colorTemperature (attr 0x0007 u16 mireds) → color_temp.
    auto rct = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0007, 0x21, u16(300)));
    assert(rct.any_matched);
    const Value* ct = rct.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 300);
}

// ── AL8RGB13W-AP: RGBW — restored full colour + colour-temp axis ─────
void check_rgb() {
    const auto& def = devices::alchemy::kDef_AL8RGB13W_AP;

    // Light base stays.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // Both axes restored.
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_xy"));
    assert(def_binds(def, COLOR_CTRL));

    // lightingColorCtrl currentX (attr 0x0003 u16) → color_x = raw/65535.
    auto rx = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0003, 0x21, u16(32768)));
    assert(rx.any_matched);
    const Value* cx = rx.merged.find("color_x");
    assert(cx && cx->type == ValueType::Float && cx->f > 0.49f && cx->f < 0.51f);

    // lightingColorCtrl currentY (attr 0x0004 u16) → color_y.
    auto ry = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0004, 0x21, u16(16384)));
    assert(ry.any_matched);
    const Value* cy = ry.merged.find("color_y");
    assert(cy && cy->type == ValueType::Float && cy->f > 0.24f && cy->f < 0.26f);

    // lightingColorCtrl currentSaturation (attr 0x0001 u8) → saturation.
    const std::uint8_t sat[] = {0xFE};
    auto rs = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0001, 0x20, sat));
    assert(rs.any_matched);
    const Value* sv = rs.merged.find("saturation");
    assert(sv && sv->type == ValueType::Uint && sv->u == 254);

    // lightingColorCtrl colorTemperature (attr 0x0007 u16 mireds) → color_temp.
    auto rct = dispatch_zcl(def, COLOR_CTRL, 1, attr_report(0x0007, 0x21, u16(250)));
    assert(rct.any_matched);
    const Value* ct = rct.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 250);
}

}  // namespace

int main() {
    check_tc();
    check_rgb();
    return 0;
}
