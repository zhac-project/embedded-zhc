// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-SEMO — IAS occupancy + battery; Tuya DPs PARTIAL.
// Motion sensor
// z2m-source: candeo.ts #C-ZB-SEMO.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_SEMO[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzIlluminance
};
constexpr const char* kModels_C_ZB_SEMO[] = { "C-ZB-SEMO" };

constexpr Expose kAutoExposes[] = {
    {"battery",           ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0},
    {"occupancy",         ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_low",       ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"tamper",            ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"illuminance",       ExposeType::Numeric, Access::State,    "lx",    nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0400}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_SEMO{
    .zigbee_models=kModels_C_ZB_SEMO, .zigbee_models_count=sizeof(kModels_C_ZB_SEMO)/sizeof(kModels_C_ZB_SEMO[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-SEMO", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_SEMO, .from_zigbee_count=sizeof(kFz_C_ZB_SEMO)/sizeof(kFz_C_ZB_SEMO[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
