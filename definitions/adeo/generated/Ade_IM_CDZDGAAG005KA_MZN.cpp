// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adeo IM-CDZDGAAG005KA-MZN — hand-rewritten for parity.
// ENKI LEXMAN GU-10 LED white
// z2m-source: adeo.ts #IM-CDZDGAAG005KA-MZN.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_IM_CDZDGAAG005KA_MZN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_IM_CDZDGAAG005KA_MZN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_IM_CDZDGAAG005KA_MZN[] = { "ZBEK-11" };

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

extern const PreparedDefinition kDef_IM_CDZDGAAG005KA_MZN{
    .zigbee_models=kModels_IM_CDZDGAAG005KA_MZN, .zigbee_models_count=sizeof(kModels_IM_CDZDGAAG005KA_MZN)/sizeof(kModels_IM_CDZDGAAG005KA_MZN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IM-CDZDGAAG005KA-MZN", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IM_CDZDGAAG005KA_MZN, .from_zigbee_count=sizeof(kFz_IM_CDZDGAAG005KA_MZN)/sizeof(kFz_IM_CDZDGAAG005KA_MZN[0]),
    .to_zigbee=kTz_IM_CDZDGAAG005KA_MZN, .to_zigbee_count=sizeof(kTz_IM_CDZDGAAG005KA_MZN)/sizeof(kTz_IM_CDZDGAAG005KA_MZN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
