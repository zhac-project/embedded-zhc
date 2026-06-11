// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Commercial Electric. The real z2m<->ez gap caught here:
//
//   * 53170161 "Zigbee CCT Downlight" is a colour-temperature tunable
//     light: z2m wires `m.light({colorTemp: {range: undefined}})`, which
//     exposes state + brightness + color_temp, binds lightingColorCtrl
//     (0x0300), and decodes/sets the colour temperature axis. The
//     auto-generated port dropped the CCT axis entirely — it only carried
//     on/off + brightness (kFzOnOff/kFzBrightness, exposes state+brightness,
//     binds 0x0006+0x0008). So color_temp reports were never decoded and
//     the SPA could never set CCT. Fixed by wiring the generic
//     kFzColorTemperature / kTzColorTemp converters, publishing the
//     `color_temp` expose, and binding lightingColorCtrl (0x0300).
//
// z2m-source: zigbee-herdsman-converters/src/devices/commercial_electric.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::commercial_electric {
extern const PreparedDefinition kDef_D53170161;
}  // namespace zhc::devices::commercial_electric

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

constexpr std::uint16_t GEN_ON_OFF   = 0x0006;
constexpr std::uint16_t GEN_LEVEL    = 0x0008;
constexpr std::uint16_t LIGHT_COLOR  = 0x0300;

// genOnOff AttributeReport: attr 0x0000 onOff type 0x10 bool = 1.
constexpr std::uint8_t kOnOffFrame[] = {
    0x18, 0x10, 0x0A,
    0x00, 0x00, 0x10, 0x01,
};

// genLevelCtrl AttributeReport: attr 0x0000 currentLevel type 0x20 u8 = 254.
constexpr std::uint8_t kBrightnessFrame[] = {
    0x18, 0x11, 0x0A,
    0x00, 0x00, 0x20, 0xFE,
};

// lightingColorCtrl AttributeReport: attr 0x0007 colorTemperature u16 = 370.
constexpr std::uint8_t kColorTempFrame[] = {
    0x18, 0x12, 0x0A,
    0x07, 0x00, 0x21, 0x72, 0x01,  // 0x0007 u16 = 0x0172 = 370 mired
};

void test_exposes_and_bindings() {
    const auto& def = devices::commercial_electric::kDef_D53170161;

    // CCT light shape: state + brightness + the restored color_temp axis.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));

    // CCT-only — no full-colour (xy/hue/sat) axis on this downlight.
    assert(!def_exposes(def, "color"));
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "hue"));
    assert(!def_exposes(def, "saturation"));

    // lightingColorCtrl must be bound for the CCT axis to report/configure.
    assert(def_binds(def, GEN_ON_OFF));
    assert(def_binds(def, GEN_LEVEL));
    assert(def_binds(def, LIGHT_COLOR));

    // Settable light → must carry a write path (on/off + brightness + CCT).
    assert(def.to_zigbee != nullptr && def.to_zigbee_count >= 3);
}

void test_decodes_color_temp() {
    const auto& def = devices::commercial_electric::kDef_D53170161;

    // The dropped axis: a colorTemperature report must now decode to color_temp.
    auto rct = dispatch_zcl(def, LIGHT_COLOR, 1, kColorTempFrame);
    assert(rct.any_matched);
    const Value* ct = rct.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 370);
}

void test_on_off_and_brightness_still_decode() {
    const auto& def = devices::commercial_electric::kDef_D53170161;

    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, kOnOffFrame);
    assert(ron.any_matched);
    const Value* st = ron.merged.find("state");
    assert(st != nullptr);

    auto rbr = dispatch_zcl(def, GEN_LEVEL, 1, kBrightnessFrame);
    assert(rbr.any_matched);
    const Value* br = rbr.merged.find("brightness");
    assert(br && br->type == ValueType::Uint && br->u == 254);
}

}  // namespace

int main() {
    test_exposes_and_bindings();
    test_decodes_color_temp();
    test_on_off_and_brightness_still_decode();
    return 0;
}
