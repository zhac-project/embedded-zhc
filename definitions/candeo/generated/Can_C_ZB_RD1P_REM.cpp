// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Candeo C-ZB-RD1P-REM — knob remote on EP2, metering on EP1.
// Zigbee rotary dimmer pro (remote mode)
// z2m-source: candeo.ts #C-ZB-RD1P-REM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/candeo/_shared.hpp"

namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_RD1P_REM[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::candeo::kFzRd1pKnobPress,
    &::zhc::candeo::kFzRd1pKnobRotation
};
constexpr const char* kModels_C_ZB_RD1P_REM[] = { "C-ZB-RD1P-REM" };

constexpr Expose kAutoExposes[] = {
    {"energy",            ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0},
    {"current",           ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"action",            ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},
    {1, 0x0B04},
    {2, 0x0006},
    {2, 0x0008}
};

constexpr ::zhc::EndpointLabel kEndpoints_C_ZB_RD1P_REM[] = { {"l1", 1}, {"l2", 2} };

}  // namespace

extern const PreparedDefinition kDef_C_ZB_RD1P_REM{
    .zigbee_models=kModels_C_ZB_RD1P_REM, .zigbee_models_count=sizeof(kModels_C_ZB_RD1P_REM)/sizeof(kModels_C_ZB_RD1P_REM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-RD1P-REM", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_RD1P_REM, .from_zigbee_count=sizeof(kFz_C_ZB_RD1P_REM)/sizeof(kFz_C_ZB_RD1P_REM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_C_ZB_RD1P_REM,
    .endpoint_map_count = sizeof(kEndpoints_C_ZB_RD1P_REM)/sizeof(kEndpoints_C_ZB_RD1P_REM[0]),
};

}  // namespace zhc::devices::candeo
