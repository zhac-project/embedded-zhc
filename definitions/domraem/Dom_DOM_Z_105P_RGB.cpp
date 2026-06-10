// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Domraem DOM-Z-105P_RGB — hand-rewritten ColorLight (hs) bundle.
// LED controller 5 in 1.
// z2m extend: m.light({color: {modes: ["hs"], enhancedHue: false}})
//   → on/off + level + color (hs only — no colorTemp, no xy).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the color axis — this file restores hue + saturation.
// z2m-source: domraem.ts #DOM-Z-105P_RGB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::domraem {
namespace {
const FzConverter* const kFz_DOM_Z_105P_RGB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_DOM_Z_105P_RGB[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_DOM_Z_105P_RGB[] = { "RGB" };

}  // namespace


// Hand-curated ColorLight (hs) surface.
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (hs color)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_DOM_Z_105P_RGB{
    .zigbee_models=kModels_DOM_Z_105P_RGB, .zigbee_models_count=sizeof(kModels_DOM_Z_105P_RGB)/sizeof(kModels_DOM_Z_105P_RGB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DOM-Z-105P_RGB", .vendor="Domraem",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DOM_Z_105P_RGB, .from_zigbee_count=sizeof(kFz_DOM_Z_105P_RGB)/sizeof(kFz_DOM_Z_105P_RGB[0]),
    .to_zigbee=kTz_DOM_Z_105P_RGB, .to_zigbee_count=sizeof(kTz_DOM_Z_105P_RGB)/sizeof(kTz_DOM_Z_105P_RGB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::domraem
