// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips Hue white and colour ambiance slim downlight 4"
// (z2m v26.95.0 parity, added v26.95.0).
// z2m-source: philips.ts #929004291501
// NOTE the fingerprint: the zigbeeModel is "LCD018", not the 929004291501
// order code the definition is named after.
// Upstream colorTemp range [50, 1000] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {
const FzConverter* const kFz_D929004291501[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D929004291501[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D929004291501[] = { "LCD018" };

constexpr Expose kExposes_D929004291501[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, "Light state",        nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Brightness",         nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", "Colour temperature", nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, "Colour x",           nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, "Colour y",           nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, "Hue",                nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, "Saturation",         nullptr, 0},
};

constexpr BindingSpec kBindings_D929004291501[] = { {1, 0x0006}, {1, 0x0008}, {1, 0x0300} };
}  // namespace

extern const PreparedDefinition kDef_D929004291501{
    .zigbee_models=kModels_D929004291501, .zigbee_models_count=sizeof(kModels_D929004291501)/sizeof(kModels_D929004291501[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="929004291501", .vendor="Philips",
    .meta=nullptr, .exposes=kExposes_D929004291501, .exposes_count=sizeof(kExposes_D929004291501)/sizeof(kExposes_D929004291501[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D929004291501, .from_zigbee_count=sizeof(kFz_D929004291501)/sizeof(kFz_D929004291501[0]),
    .to_zigbee=kTz_D929004291501, .to_zigbee_count=sizeof(kTz_D929004291501)/sizeof(kTz_D929004291501[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D929004291501, .bindings_count=sizeof(kBindings_D929004291501)/sizeof(kBindings_D929004291501[0]),
};

}  // namespace zhc::devices::philips
