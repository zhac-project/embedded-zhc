// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Domraem DOM-Z-105P_CCT — hand-rewritten CTLight bundle.
// LED controller 5 in 1.
// z2m extend: m.light({colorTemp: {range: [158, 495]}})
//   → on/off + level + colorTemp (tunable-white, no color).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the colorTemp axis — this file restores it.
// z2m-source: domraem.ts #DOM-Z-105P_CCT.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::domraem {
namespace {
const FzConverter* const kFz_DOM_Z_105P_CCT[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_DOM_Z_105P_CCT[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_DOM_Z_105P_CCT[] = { "CCT" };

}  // namespace


// Hand-curated CTLight surface (CT range 158–495 mired per z2m).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 158, 495, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_DOM_Z_105P_CCT{
    .zigbee_models=kModels_DOM_Z_105P_CCT, .zigbee_models_count=sizeof(kModels_DOM_Z_105P_CCT)/sizeof(kModels_DOM_Z_105P_CCT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DOM-Z-105P_CCT", .vendor="Domraem",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DOM_Z_105P_CCT, .from_zigbee_count=sizeof(kFz_DOM_Z_105P_CCT)/sizeof(kFz_DOM_Z_105P_CCT[0]),
    .to_zigbee=kTz_DOM_Z_105P_CCT, .to_zigbee_count=sizeof(kTz_DOM_Z_105P_CCT)/sizeof(kTz_DOM_Z_105P_CCT[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::domraem
