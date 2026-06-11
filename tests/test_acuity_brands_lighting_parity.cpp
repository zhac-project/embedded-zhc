// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Acuity Brands Lighting (Juno / nLight) downlights.
//
// All three are m.light(...) bundles whose auto-port dropped the entire
// lightingColorCtrl (0x0300) axis — wiring only kFzOnOff + kFzBrightness and
// exposing state + brightness:
//
//   * WF4C_WF6C (ABL-LIGHT-Z-001) — m.light({colorTemp:{range:[200,370]}}).
//       Tunable-white: needs color_temp (CCT). Restored kFzColorTemperature /
//       kTzColorTemp + color_temp expose (mireds 200-370) + 0x0300 bind.
//   * RB56SC   (ABL-LIGHT-Z-201) — m.light({colorTemp:..., color:true}).
//   * RB56AC   (ABL-LIGHT-Z-202) — m.light({colorTemp:..., color:true}).
//       Full colour: needs color_temp + color (xy). Restored
//       kFzColorTemperature + kFzColor / kTzColorTemp + kTzColor +
//       color_temp / color_x / color_y exposes + 0x0300 bind.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * every def exposes color_temp + binds lightingColorCtrl (0x0300),
//   * the two colour bulbs additionally expose color_x / color_y,
//   * a lightingColorCtrl colorTemperature (0x0007) report decodes to
//     color_temp (raw mireds),
//   * a currentX / currentY (0x0003 / 0x0004) report decodes to
//     color_x / color_y (float = raw/65535) on the colour bulbs,
//   * the tunable-white bulb does NOT carry phantom colour exposes.
//
// z2m-source: zigbee-herdsman-converters/src/devices/acuity_brands_lighting.ts
//             #WF4C_WF6C / #RB56SC / #RB56AC + lib/modernExtend.ts light().

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

namespace zhc::devices::acuity_brands_lighting {
extern const PreparedDefinition kDef_WF4C_WF6C;
extern const PreparedDefinition kDef_RB56SC;
extern const PreparedDefinition kDef_RB56AC;
}  // namespace zhc::devices::acuity_brands_lighting

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

DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// lightingColorCtrl colorTemperature (attr 0x0007, u16) report.
void check_color_temp_decode(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    const std::array<std::uint8_t, 2> ct = {0x2C, 0x01};  // 300 mireds
    auto r = dispatch_zcl(ctx, def, 0x0300, "lightingColorCtrl",
                          attr_report(0x0007, 0x21,
                                      std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(r.any_matched);
    const Value* v = r.merged.find("color_temp");
    assert(v && v->type == ValueType::Uint && v->u == 300);
}

// lightingColorCtrl currentX / currentY (attr 0x0003 / 0x0004, u16) reports.
void check_color_xy_decode(const PreparedDefinition& def) {
    // currentX = 0x8000 (32768) → 32768/65535 ≈ 0.5.
    RuntimeContext ctx1{};
    const std::array<std::uint8_t, 2> cx = {0x00, 0x80};
    auto rx = dispatch_zcl(ctx1, def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0003, 0x21,
                                       std::span<const std::uint8_t>(cx.data(), cx.size())));
    assert(rx.any_matched);
    const Value* vx = rx.merged.find("color_x");
    assert(vx && vx->type == ValueType::Float);
    assert(vx->f > 0.49f && vx->f < 0.51f);

    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 2> cy = {0x00, 0x40};  // 0x4000 → ~0.25
    auto ry = dispatch_zcl(ctx2, def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0004, 0x21,
                                       std::span<const std::uint8_t>(cy.data(), cy.size())));
    assert(ry.any_matched);
    const Value* vy = ry.merged.find("color_y");
    assert(vy && vy->type == ValueType::Float);
    assert(vy->f > 0.24f && vy->f < 0.26f);
}

// ── WF4C_WF6C: tunable white — color_temp only, NO phantom colour. ────────
void check_wf4c() {
    using namespace zhc::devices::acuity_brands_lighting;
    const auto& def = kDef_WF4C_WF6C;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_x"));   // colorTemp-only, no colour axis
    assert(!def_exposes(def, "color_y"));

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));

    check_color_temp_decode(def);
}

// ── RB56SC / RB56AC: full colour — color_temp + color_x/color_y. ──────────
void check_colour_bulb(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));

    check_color_temp_decode(def);
    check_color_xy_decode(def);
}

}  // namespace

int main() {
    using namespace zhc::devices::acuity_brands_lighting;
    check_wf4c();
    check_colour_bulb(kDef_RB56SC);
    check_colour_bulb(kDef_RB56AC);
    return 0;
}
