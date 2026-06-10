// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the SLV (VALETO / Play) family. The real z2m<->ez gaps
// caught here:
//
//   * The four LED luminaires were auto-ported as plain dimmable lights
//     (on/off + brightness only). z2m wires m.light({colorTemp[, color]}),
//     so the auto-port dropped the colour axes:
//       - 1001248 (CCT driver)  + 1005314 (QPAR111) : m.light({colorTemp})
//         -> must expose color_temp and bind lightingColorCtrl 0x0300.
//       - 1001923 (GU10 RGBW)   + 1005318 (E27 RGBW) : {colorTemp, color}
//         -> must expose color_temp + color_xy and bind 0x0300.
//     Fixed by wiring the generic kFzColorTemperature/kTzColorTemp (+ for
//     RGBW kFzColor/kTzColor) converters and the colour surface.
//
//   * 1002994 (VALETO remote) is a pure binder: z2m declares
//     fromZigbee:[], toZigbee:[], exposes:[]. The auto-port invented a
//     controllable on/off `state` plus battery + voltage exposes and
//     decoders (class-e phantom bundle). Fixed by stripping it to nothing.
//
// z2m-source: zigbee-herdsman-converters/src/devices/slv.ts.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::slv {
extern const PreparedDefinition kDef_D1001248;  // CCT LED driver
extern const PreparedDefinition kDef_D1005314;  // QPAR111 CCT tunable
extern const PreparedDefinition kDef_D1001923;  // GU10 RGBW
extern const PreparedDefinition kDef_D1005318;  // E27 RGBW
extern const PreparedDefinition kDef_D1002994;  // VALETO remote (pure binder)
}  // namespace zhc::devices::slv

using namespace zhc;

namespace {

constexpr std::uint16_t GEN_ON_OFF   = 0x0006;
constexpr std::uint16_t GEN_LEVEL    = 0x0008;
constexpr std::uint16_t COLOR_CTRL   = 0x0300;

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
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
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

// Common light surface shared by every SLV luminaire.
void check_light_common(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));        // the dropped axis
    assert(def_binds(def, GEN_ON_OFF));
    assert(def_binds(def, GEN_LEVEL));
    assert(def_binds(def, COLOR_CTRL) && "colour light must bind lightingColorCtrl 0x0300");
    assert(def.to_zigbee != nullptr && def.to_zigbee_count >= 3);

    // Runtime decode: lightingColorCtrl Report Attributes, attr 0x0007
    // colorTemperatureMireds (u16) = 250 mired -> key "color_temp" present.
    const std::uint8_t ct_report[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x07, 0x00,              // attr 0x0007 (colorTemperatureMireds)
        0x21,                    // type u16
        0xFA, 0x00,              // 250
    };
    auto ct = dispatch_zcl(def, COLOR_CTRL, ct_report);
    assert(ct.any_matched);
    assert(ct.merged.find("color_temp") != nullptr &&
           "color_temp must decode now that kFzColorTemperature is wired");
}

// CCT-only luminaires: color_temp axis, but NO color surface.
void check_cct(const PreparedDefinition& def) {
    check_light_common(def);
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "color"));
}

// RGBW luminaires: color_temp + color (color_xy) surface.
void check_rgbw(const PreparedDefinition& def) {
    check_light_common(def);
    assert(def_exposes(def, "color_xy"));

    // Runtime decode: currentX (attr 0x0003, u16) -> key "color_x".
    const std::uint8_t cx_report[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x03, 0x00,              // attr 0x0003 (currentX)
        0x21,                    // type u16
        0x34, 0x52,              // 0x5234
    };
    auto cx = dispatch_zcl(def, COLOR_CTRL, cx_report);
    assert(cx.any_matched);
    assert(cx.merged.find("color_x") != nullptr &&
           "color_x must decode now that kFzColor is wired");
}

void test_cct_1001248() { check_cct(devices::slv::kDef_D1001248); }
void test_cct_1005314() { check_cct(devices::slv::kDef_D1005314); }
void test_rgbw_1001923() { check_rgbw(devices::slv::kDef_D1001923); }
void test_rgbw_1005318() { check_rgbw(devices::slv::kDef_D1005318); }

// ── VALETO remote: pure binder, exposes/decodes NOTHING ──────────────
void test_remote_is_pure_binder() {
    const auto& def = devices::slv::kDef_D1002994;
    // z2m: fromZigbee:[], toZigbee:[], exposes:[] — the phantom bundle is gone.
    assert(def.exposes_count == 0 && def.exposes == nullptr);
    assert(def.from_zigbee_count == 0 && def.from_zigbee == nullptr);
    assert(def.to_zigbee_count == 0 && def.to_zigbee == nullptr);
    assert(def.bindings_count == 0);
    assert(!def_exposes(def, "state"));    // no phantom controllable on/off
    assert(!def_exposes(def, "battery"));  // no phantom battery
    assert(!def_exposes(def, "voltage"));  // no phantom voltage

    // A genOnOff command frame must NOT produce any shadow state.
    const std::uint8_t on_cmd[] = {0x01, 0x42, 0x01};  // fc, tsn, cmd=On
    auto r = dispatch_zcl(def, GEN_ON_OFF, on_cmd);
    assert(!r.any_matched && "remote def has no converters; nothing should match");
    assert(r.merged.find("state") == nullptr);
}

}  // namespace

int main() {
    test_cct_1001248();
    test_cct_1005314();
    test_rgbw_1001923();
    test_rgbw_1005318();
    test_remote_is_pure_binder();
    return 0;
}
