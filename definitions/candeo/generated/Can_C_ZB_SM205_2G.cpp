// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Candeo C-ZB-SM205-2G — 2-gang + metering + switch_type (PARTIAL multi-ep).
// Smart 2 gang switch module
// z2m-source: candeo.ts #C-ZB-SM205-2G.
#include "definitions/_generic/_shared.hpp"
#include "definitions/candeo/_shared.hpp"

namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_SM205_2G[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::candeo::kFzExternalSwitchType
};
const TzConverter* const kTz_C_ZB_SM205_2G[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::candeo::kTzExternalSwitchType
};
constexpr const char* kModels_C_ZB_SM205_2G[] = { "C-ZB-SM205-2G" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",            ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0},
    {"current",           ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"external_switch_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0x0702},
    {1, 0x0B04}
};

constexpr ::zhc::EndpointLabel kEndpoints_C_ZB_SM205_2G[] = { {"l1", 1}, {"l2", 2}, {"e11", 11} };

}  // namespace

extern const PreparedDefinition kDef_C_ZB_SM205_2G{
    .zigbee_models=kModels_C_ZB_SM205_2G, .zigbee_models_count=sizeof(kModels_C_ZB_SM205_2G)/sizeof(kModels_C_ZB_SM205_2G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-SM205-2G", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_SM205_2G, .from_zigbee_count=sizeof(kFz_C_ZB_SM205_2G)/sizeof(kFz_C_ZB_SM205_2G[0]),
    .to_zigbee=kTz_C_ZB_SM205_2G, .to_zigbee_count=sizeof(kTz_C_ZB_SM205_2G)/sizeof(kTz_C_ZB_SM205_2G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_C_ZB_SM205_2G,
    .endpoint_map_count = sizeof(kEndpoints_C_ZB_SM205_2G)/sizeof(kEndpoints_C_ZB_SM205_2G[0]),
};

}  // namespace zhc::devices::candeo
