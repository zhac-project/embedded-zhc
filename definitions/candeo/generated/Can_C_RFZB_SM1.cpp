// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-RFZB-SM1.
// Zigbee & RF switch module
// z2m-source: candeo.ts #C-RFZB-SM1.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_RFZB_SM1[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1
};
const TzConverter* const kTz_C_RFZB_SM1[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1
};
constexpr const char* kModels_C_RFZB_SM1[] = { "C-RFZB-SM1" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}
};

}  // namespace

extern const PreparedDefinition kDef_C_RFZB_SM1{
    .zigbee_models=kModels_C_RFZB_SM1, .zigbee_models_count=sizeof(kModels_C_RFZB_SM1)/sizeof(kModels_C_RFZB_SM1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-RFZB-SM1", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_RFZB_SM1, .from_zigbee_count=sizeof(kFz_C_RFZB_SM1)/sizeof(kFz_C_RFZB_SM1[0]),
    .to_zigbee=kTz_C_RFZB_SM1, .to_zigbee_count=sizeof(kTz_C_RFZB_SM1)/sizeof(kTz_C_RFZB_SM1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
