// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Giderwel (m.light) device family.
// z2m-source: giderwel.ts.
//
// Bug fixed: GD-ZCRGB012 (Smart Zigbee RGB LED strip controller) is
// defined in z2m as m.light({color: {modes: ["xy", "hs"]}}) — an RGB
// controller exposing on/off + brightness + colour (xy/hs) on the
// lightingColorCtrl cluster 0x0300. The auto-port dropped the entire
// colour axis: it carried only kFzOnOff/kFzBrightness, exposed only
// state + brightness, and bound only 0x0006/0x0008 — no kFzColor /
// kTzColor, no color_xy expose, no 0x0300 binding. Restored here.
// (No colorTemp axis: z2m declares only xy/hs modes.)

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
#include "zhc/devices/giderwel_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"   // complete RuntimeContext (fz_color ctx arg)
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::giderwel {
extern const PreparedDefinition kDef_GD_ZCRGB012;
}  // namespace zhc::devices::giderwel

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool def_has_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

bool def_has_tz(const PreparedDefinition& def, const TzConverter* conv) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i)
        if (def.to_zigbee[i] == conv) return true;
    return false;
}

InboundApsFrame build_frame(std::uint16_t cluster, std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool nearly_equal(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
}

// lightingColorCtrl AttributeReport carrying the XY axis (z2m "xy" mode):
//   attr 0x0003 currentX type 0x21 u16 = 0x4000
//   attr 0x0004 currentY type 0x21 u16 = 0x8000
//   attr 0x0008 colorMode type 0x30 enum8 = 0x01 (xy)
constexpr std::uint8_t kColorXyFrame[] = {
    0x18, 0x42, 0x0A,
    0x03, 0x00, 0x21, 0x00, 0x40,
    0x04, 0x00, 0x21, 0x00, 0x80,
    0x08, 0x00, 0x30, 0x01,
};

// lightingColorCtrl AttributeReport carrying the HS axis (z2m "hs" mode):
//   attr 0x0000 currentHue type 0x20 u8 = 120
//   attr 0x0001 currentSaturation type 0x20 u8 = 200
constexpr std::uint8_t kColorHsFrame[] = {
    0x18, 0x43, 0x0A,
    0x00, 0x00, 0x20, 0x78,
    0x01, 0x00, 0x20, 0xC8,
};

}  // namespace

// ── static surface: colour axis restored on the RGB controller ───────
static void test_rgb_controller_colour_surface() {
    const auto& def = devices::giderwel::kDef_GD_ZCRGB012;

    // on/off + brightness surface intact.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_binds(def, 0x0006) && "binds genOnOff");
    assert(def_binds(def, 0x0008) && "binds genLevelCtrl");

    // The restored colour axis (the fix).
    assert(def_exposes(def, "color_xy") && "RGB controller must expose colour");
    assert(def_binds(def, 0x0300) && "RGB controller must bind lightingColorCtrl");
    assert(def_has_fz(def, &generic::kFzColor) && "must wire the colour fz decoder");
    assert(def_has_tz(def, &generic::kTzColor) && "must wire the colour tz encoder");

    // z2m declares only xy/hs — no colorTemp axis on this device.
    assert(!def_exposes(def, "color_temp") && "GD-ZCRGB012 has no colorTemp axis");

    // Resolves through the registry, not just the symbol.
    bool found = false;
    for (std::size_t i = 0; i < devices::giderwel::kGiderwelRegistryCount; ++i) {
        const auto* d = devices::giderwel::kGiderwelRegistry[i];
        if (d && d->model && std::strcmp(d->model, "GD-ZCRGB012") == 0) found = true;
    }
    assert(found && "GD-ZCRGB012 present in registry");
}

// ── live decode: kFzColor emits the xy axis from a real frame ────────
static void test_colour_decodes_xy() {
    auto raw = build_frame(0x0300, kColorXyFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));

    const Value* x = out.find("color_x");
    const Value* y = out.find("color_y");
    assert(x && x->type == ValueType::Float && nearly_equal(x->f, 0x4000 / 65535.0f));
    assert(y && y->type == ValueType::Float && nearly_equal(y->f, 0x8000 / 65535.0f));
}

// ── live decode: kFzColor emits the hs axis from a real frame ────────
static void test_colour_decodes_hs() {
    auto raw = build_frame(0x0300, kColorHsFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));

    const Value* hue = out.find("hue");
    const Value* sat = out.find("saturation");
    assert(hue && hue->type == ValueType::Uint && hue->u == 120);
    assert(sat && sat->type == ValueType::Uint && sat->u == 200);
}

int main() {
    test_rgb_controller_colour_surface();
    test_colour_decodes_xy();
    test_colour_decodes_hs();
    return 0;
}
