// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adeo 9CZA-P470T-A1A — hand-rewritten for parity.
// ENKI LEXMAN E14 LED RGBW
// z2m-source: adeo.ts #9CZA-P470T-A1A.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_D9CZA_P470T_A1A[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D9CZA_P470T_A1A[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D9CZA_P470T_A1A[] = { "LXEK-3" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_D9CZA_P470T_A1A{
    .zigbee_models=kModels_D9CZA_P470T_A1A, .zigbee_models_count=sizeof(kModels_D9CZA_P470T_A1A)/sizeof(kModels_D9CZA_P470T_A1A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="9CZA-P470T-A1A", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D9CZA_P470T_A1A, .from_zigbee_count=sizeof(kFz_D9CZA_P470T_A1A)/sizeof(kFz_D9CZA_P470T_A1A[0]),
    .to_zigbee=kTz_D9CZA_P470T_A1A, .to_zigbee_count=sizeof(kTz_D9CZA_P470T_A1A)/sizeof(kTz_D9CZA_P470T_A1A[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
