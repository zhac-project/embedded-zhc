// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: VilleroyBoch C0040000 — hand-rewritten ColorTempLight (CCT).
// Zigbee home automation kit for mirror
// z2m extend: m.light({colorTemp: {range: [160, 450]}})
//   → on/off + brightness + colorTemp (mireds). No color (xy/hs) axis.
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the CT axis — this file restores it (lightingColorCtrl 0x0300).
// z2m-source: villeroy_boch.ts #C0040000.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::villeroy_boch {
namespace {
const FzConverter* const kFz_C0040000[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_C0040000[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_C0040000[] = { "EC1300" };

}  // namespace


// Hand-curated ColorTempLight surface — CCT range [160, 450] mireds.
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
                   ExposeCategory::State, 160, 450, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (colorTemp)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_C0040000{
    .zigbee_models=kModels_C0040000, .zigbee_models_count=sizeof(kModels_C0040000)/sizeof(kModels_C0040000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C0040000", .vendor="VilleroyBoch",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C0040000, .from_zigbee_count=sizeof(kFz_C0040000)/sizeof(kFz_C0040000[0]),
    .to_zigbee=kTz_C0040000, .to_zigbee_count=sizeof(kTz_C0040000)/sizeof(kTz_C0040000[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::villeroy_boch
