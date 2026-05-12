// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adeo IC-CDZFB2AC004HA-MZN — hand-rewritten for parity.
// ENKI LEXMAN E14 LED white
// z2m-source: adeo.ts #IC-CDZFB2AC004HA-MZN.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_IC_CDZFB2AC004HA_MZN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_IC_CDZFB2AC004HA_MZN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_IC_CDZFB2AC004HA_MZN[] = { "ZBEK-10" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_IC_CDZFB2AC004HA_MZN{
    .zigbee_models=kModels_IC_CDZFB2AC004HA_MZN, .zigbee_models_count=sizeof(kModels_IC_CDZFB2AC004HA_MZN)/sizeof(kModels_IC_CDZFB2AC004HA_MZN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IC-CDZFB2AC004HA-MZN", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IC_CDZFB2AC004HA_MZN, .from_zigbee_count=sizeof(kFz_IC_CDZFB2AC004HA_MZN)/sizeof(kFz_IC_CDZFB2AC004HA_MZN[0]),
    .to_zigbee=kTz_IC_CDZFB2AC004HA_MZN, .to_zigbee_count=sizeof(kTz_IC_CDZFB2AC004HA_MZN)/sizeof(kTz_IC_CDZFB2AC004HA_MZN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
