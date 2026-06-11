// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Hilux DZ8 — hand-rewritten ColorTempLight bundle.
// Spot 7W
// z2m extend: m.light({colorTemp: {range: [153, 370]}, powerOnBehavior: false})
//   → on/off + level + colorTemp (mireds, CCT-only, no xy/hs color).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the colorTemp axis — this file restores it.
// z2m-source: hilux.ts #DZ8.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hilux {
namespace {
const FzConverter* const kFz_DZ8[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_DZ8[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_DZ8[] = { "Hilux DZ8" };

}  // namespace


// Hand-curated ColorTempLight surface (CCT-only, range [153, 370] mireds).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (colorTemp)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_DZ8{
    .zigbee_models=kModels_DZ8, .zigbee_models_count=sizeof(kModels_DZ8)/sizeof(kModels_DZ8[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DZ8", .vendor="Hilux",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DZ8, .from_zigbee_count=sizeof(kFz_DZ8)/sizeof(kFz_DZ8[0]),
    .to_zigbee=kTz_DZ8, .to_zigbee_count=sizeof(kTz_DZ8)/sizeof(kTz_DZ8[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hilux
