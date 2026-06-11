// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Gmy B07KG5KF5R — hand-rewritten ColorTempLight bundle.
// GMY Smart bulb, 470lm, vintage dimmable, 2700-6500k, E27
// z2m extend: m.light({colorTemp: {range: undefined}})
//   → on/off + level + colorTemp (mireds, CCT-only, no xy/hs color).
//   range: undefined = device-default colorTemp range (2700-6500K per descr).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the colorTemp axis — this file restores it. Model "CCT box"
// (correlated colour temperature) confirms a tunable-white bulb.
// z2m-source: gmy.ts #B07KG5KF5R.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gmy {
namespace {
const FzConverter* const kFz_B07KG5KF5R[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_B07KG5KF5R[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_B07KG5KF5R[] = { "CCT box" };

}  // namespace


// Hand-curated ColorTempLight surface (CCT-only, device-default mired range).
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

extern const PreparedDefinition kDef_B07KG5KF5R{
    .zigbee_models=kModels_B07KG5KF5R, .zigbee_models_count=sizeof(kModels_B07KG5KF5R)/sizeof(kModels_B07KG5KF5R[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="B07KG5KF5R", .vendor="Gmy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_B07KG5KF5R, .from_zigbee_count=sizeof(kFz_B07KG5KF5R)/sizeof(kFz_B07KG5KF5R[0]),
    .to_zigbee=kTz_B07KG5KF5R, .to_zigbee_count=sizeof(kTz_B07KG5KF5R)/sizeof(kTz_B07KG5KF5R[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::gmy
