// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Hilux LED family. The auto-generator collapses every
// m.light() down to on/off + brightness, dropping the colorTemp/color axes.
//
//   Dropped colorTemp axis — DZ8 ("Spot 7W") is z2m
//     m.light({colorTemp: {range: [153, 370]}, powerOnBehavior: false})
//   i.e. a CCT-only bulb (no xy/hs color). The port wired only kFzOnOff +
//   kFzBrightness with bindings {0x0006, 0x0008}, so color_temp was both
//   unexposed and undecoded. This fixture pins the restored axis: the
//   color_temp expose, the lightingColorCtrl (0x0300) binding, the
//   kFzColorTemperature decoder (attr 0x0007 -> color_temp) and the
//   kTzColorTemp write path.
//
//   D160-ZG ("Zigbee LED dimmer smart switch") is z2m plain m.light() with no
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

namespace zhc::devices::hilux {
extern const PreparedDefinition kDef_DZ8;       // CCT-only spot light
extern const PreparedDefinition kDef_D160_ZG;   // plain dimmer (negative control)
}  // namespace zhc::devices::hilux

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

// ── DZ8: CCT axis restored. ────────────────────────────────────────────────
void test_dz8_colortemp() {
    using namespace zhc::devices::hilux;
    const auto& def = kDef_DZ8;

    // The light surface: on/off + brightness + color_temp, and lightingColorCtrl
    // is bound so reports/commands route. No xy/hs color (z2m has no color:{}).
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));
    assert(!def_exposes(def, "hue"));
    assert(def_binds(def, 1, 0x0006));   // genOnOff
    assert(def_binds(def, 1, 0x0008));   // genLevelCtrl
    assert(def_binds(def, 1, 0x0300));   // lightingColorCtrl (was dropped)

    // Read path: colorTemperature (attr 0x0007, u16 mireds) on
    // lightingColorCtrl -> "color_temp". 370 mireds (warm end) = 0x0172.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> ct = {0x72, 0x01};   // 370 LE
    auto cr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0007, 0x21, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(cr.any_matched);
    const Value* cv = cr.merged.find("color_temp");
    assert(cv != nullptr);   // the dropped channel now decodes

    // Write path: color_temp is settable (kTzColorTemp wired) alongside
    // state + brightness.
    assert(has_tz_for(def, "color_temp"));
    assert(has_tz_for(def, "state"));
    assert(has_tz_for(def, "brightness"));

    // Brightness (genLevelCtrl currentLevel 0x0000, u8) still decodes — the
    // restoration must not regress the kept axes.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> lv = {0x80};
    auto br = dispatch_zcl_ep(c2, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20, std::span<const std::uint8_t>(lv.data(), lv.size())));
    assert(br.any_matched);
    assert(br.merged.find("brightness") != nullptr);
}

// ── D160-ZG: plain dimmer, NO phantom color axis (negative control). ───────
void test_d160_zg_plain() {
    using namespace zhc::devices::hilux;
    const auto& def = kDef_D160_ZG;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));   // z2m m.light() has no colorTemp
    assert(!def_exposes(def, "color_x"));
    assert(!def_binds(def, 1, 0x0300));        // no lightingColorCtrl binding
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
}

}  // namespace

int main() {
    test_dz8_colortemp();
    test_d160_zg_plain();
    return 0;
}
