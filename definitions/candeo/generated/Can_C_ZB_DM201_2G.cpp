// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-DM201-2G — multi-endpoint partial (see CANDEO_PARITY.md).
// Zigbee 2 gang dimmer module
// z2m-source: candeo.ts #C-ZB-DM201-2G.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_DM201_2G[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzPowerOnBehavior1
};
const TzConverter* const kTz_C_ZB_DM201_2G[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1
};
constexpr const char* kModels_C_ZB_DM201_2G[] = { "C-ZB-DM201-2G" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {2, 0x0006},
    {2, 0x0008}
};

constexpr ::zhc::EndpointLabel kEndpoints_C_ZB_DM201_2G[] = { {"l1", 1}, {"l2", 2} };

}  // namespace

extern const PreparedDefinition kDef_C_ZB_DM201_2G{
    .zigbee_models=kModels_C_ZB_DM201_2G, .zigbee_models_count=sizeof(kModels_C_ZB_DM201_2G)/sizeof(kModels_C_ZB_DM201_2G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-DM201-2G", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_DM201_2G, .from_zigbee_count=sizeof(kFz_C_ZB_DM201_2G)/sizeof(kFz_C_ZB_DM201_2G[0]),
    .to_zigbee=kTz_C_ZB_DM201_2G, .to_zigbee_count=sizeof(kTz_C_ZB_DM201_2G)/sizeof(kTz_C_ZB_DM201_2G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_C_ZB_DM201_2G,
    .endpoint_map_count = sizeof(kEndpoints_C_ZB_DM201_2G)/sizeof(kEndpoints_C_ZB_DM201_2G[0]),
};

}  // namespace zhc::devices::candeo
