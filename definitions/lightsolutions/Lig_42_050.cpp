// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Light Solutions 42-050 — LED driver CCT.
// New upstream in z2m v26.100.0.
// z2m-source: lightsolutions.ts #42-050.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lightsolutions {
namespace {
const FzConverter* const kFz_42_050[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_42_050[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};

constexpr const char* kModels_42_050[] = { "42-050" };

constexpr Expose kExposes_42_050[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 160, 450, 1},
};

constexpr BindingSpec kBindings_42_050[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};
}  // namespace

extern const PreparedDefinition kDef_42_050{
    .zigbee_models=kModels_42_050, .zigbee_models_count=sizeof(kModels_42_050)/sizeof(kModels_42_050[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="42-050", .vendor="Light Solutions",
    .meta=nullptr,
    .exposes=kExposes_42_050, .exposes_count=sizeof(kExposes_42_050)/sizeof(kExposes_42_050[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_42_050, .from_zigbee_count=sizeof(kFz_42_050)/sizeof(kFz_42_050[0]),
    .to_zigbee=kTz_42_050, .to_zigbee_count=sizeof(kTz_42_050)/sizeof(kTz_42_050[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_42_050, .bindings_count=sizeof(kBindings_42_050)/sizeof(kBindings_42_050[0]),
};

}  // namespace zhc::devices::lightsolutions
