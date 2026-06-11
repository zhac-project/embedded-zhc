// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Alchemy AL8TC13W-AP — hand-rewritten ColorTempLight bundle.
// Downlight with tuneable white.
// z2m extend: m.light({colorTemp: {range: [153, 370]}})
//   → on/off + level + colorTemp (mireds, CCT-only, no xy/hs color).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the colorTemp axis — this file restores it.
// z2m-source: alchemy.ts #AL8TC13W-AP.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::alchemy {
namespace {
const FzConverter* const kFz_AL8TC13W_AP[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_AL8TC13W_AP[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_AL8TC13W_AP[] = { "AL8TC13W-AP" };

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

extern const PreparedDefinition kDef_AL8TC13W_AP{
    .zigbee_models=kModels_AL8TC13W_AP, .zigbee_models_count=sizeof(kModels_AL8TC13W_AP)/sizeof(kModels_AL8TC13W_AP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AL8TC13W-AP", .vendor="Alchemy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AL8TC13W_AP, .from_zigbee_count=sizeof(kFz_AL8TC13W_AP)/sizeof(kFz_AL8TC13W_AP[0]),
    .to_zigbee=kTz_AL8TC13W_AP, .to_zigbee_count=sizeof(kTz_AL8TC13W_AP)/sizeof(kTz_AL8TC13W_AP[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::alchemy
