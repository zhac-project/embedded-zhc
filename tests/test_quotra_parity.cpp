// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Quotra LED family.
// z2m-source: quotra.ts.
//
// Bug fixed: B07JHL6DRV (QV-RGBCCT "RGB WW LED strip") is
//   m.light({colorTemp:{range:[150,500]}, color:true, powerOnBehavior:false})
//   in z2m — a full RGB + colour-temperature strip. The auto-generated
//   def dropped both axes, reducing it to a plain on/off + brightness
//   dimmer (exposes state/brightness only). Graduated to a Tier-2 parent
//   override that restores the colour surface: kFzColorTemperature +
//   kFzColor fz converters, kTzColorTemp + kTzColor tz converters, the
//   color_temp/color_xy exposes, and the lightingColorCtrl (0x0300) bind.
//
// The B07CVL9SZF "Dimmer" is plain m.light() — pinned here as a
// regression guard that it stays a colourless on/off + brightness dimmer
// (no phantom colour surface introduced by the parity sweep).

#include <cassert>
#include <cstddef>
#include <cstring>

#include "definitions/_generic/_shared.hpp"
#include "zhc/devices/quotra_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::quotra {
extern const PreparedDefinition kDef_B07CVL9SZF;  // plain dimmer
extern const PreparedDefinition kDef_B07JHL6DRV;  // RGB + CCT strip (fix)
}  // namespace zhc::devices::quotra

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

bool fz_has(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

bool tz_has(const PreparedDefinition& def, const TzConverter* conv) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i)
        if (def.to_zigbee[i] == conv) return true;
    return false;
}

// The whole registry must contain a def that claims `model`.
bool registry_resolves_model(const char* model) {
    for (std::size_t i = 0; i < devices::quotra::kQuotraRegistryCount; ++i) {
        const auto* d = devices::quotra::kQuotraRegistry[i];
        if (d && d->model && std::strcmp(d->model, model) == 0) return true;
    }
    return false;
}

}  // namespace

// ── B07JHL6DRV RGB+CCT strip — full colour surface restored ──────────
static void test_rgbcct_colour_surface() {
    const auto& def = devices::quotra::kDef_B07JHL6DRV;
    // Base light surface still present.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // The restored axes — this is the fix.
    assert(def_exposes(def, "color_temp") && "RGB+CCT strip must expose color_temp");
    assert(def_exposes(def, "color_xy")   && "RGB+CCT strip must expose colour xy");
    // Colour cluster bound + decoded both ways.
    assert(def_binds(def, 0x0300) && "colour strip binds lightingColorCtrl");
    assert(fz_has(def, &::zhc::generic::kFzColorTemperature));
    assert(fz_has(def, &::zhc::generic::kFzColor));
    assert(tz_has(def, &::zhc::generic::kTzColorTemp));
    assert(tz_has(def, &::zhc::generic::kTzColor));
    // On/off + brightness still wired (not clobbered by the fix).
    assert(fz_has(def, &::zhc::generic::kFzOnOff));
    assert(fz_has(def, &::zhc::generic::kFzBrightness));
    assert(registry_resolves_model("B07JHL6DRV"));
}

// ── B07CVL9SZF Dimmer — plain colourless light (no phantom colour) ────
static void test_dimmer_is_colourless() {
    const auto& def = devices::quotra::kDef_B07CVL9SZF;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // z2m m.light() with no colour args — must NOT carry a colour surface.
    assert(!def_exposes(def, "color_temp") && "plain dimmer must not expose color_temp");
    assert(!def_exposes(def, "color_xy")   && "plain dimmer must not expose colour xy");
    assert(!def_binds(def, 0x0300) && "plain dimmer must not bind lightingColorCtrl");
    assert(!fz_has(def, &::zhc::generic::kFzColor));
    assert(!fz_has(def, &::zhc::generic::kFzColorTemperature));
    assert(registry_resolves_model("B07CVL9SZF"));
}

int main() {
    test_rgbcct_colour_surface();
    test_dimmer_is_colourless();
    return 0;
}
