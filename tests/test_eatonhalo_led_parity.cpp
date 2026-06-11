// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Eaton/Halo LED family. The auto-generator collapses
// m.light() down to on/off + brightness, dropping the colorTemp axis.
//
//   Dropped colorTemp axis — RL460WHZHA69 ("Wireless Controlled LED retrofit
//     downlight", zigbeeModel "Halo_RL5601") is z2m
//       m.light({colorTemp: {range: [200, 370]}})
//   i.e. a CCT-only bulb (no xy/hs color). The port wired only kFzOnOff +
//   kFzBrightness with bindings {0x0006, 0x0008}, so color_temp was both
//   unexposed and undecoded. This fixture pins the restored axis: the
//   color_temp expose, the lightingColorCtrl (0x0300) binding, the
//   kFzColorTemperature decoder (attr 0x0007 -> color_temp) and the
//   kTzColorTemp write path. It also pins the absence of any phantom xy/hs
//   color axis (z2m has no color:{}).

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

namespace zhc::devices::eatonhalo_led {
extern const PreparedDefinition kDef_RL460WHZHA69;   // CCT-only downlight
}  // namespace zhc::devices::eatonhalo_led

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

// ── RL460WHZHA69: CCT axis restored. ───────────────────────────────────────
void test_rl460_colortemp() {
    using namespace zhc::devices::eatonhalo_led;
    const auto& def = kDef_RL460WHZHA69;

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

}  // namespace

int main() {
    test_rl460_colortemp();
    return 0;
}
