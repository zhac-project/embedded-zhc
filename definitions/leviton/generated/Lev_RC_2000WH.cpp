// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Leviton RC-2000WH — Omnistat2 thermostat lives on EP9.
// Omnistat2 wireless thermostat
// z2m-source: leviton.ts #RC-2000WH.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leviton {
namespace {
const FzConverter* const kFz_RC_2000WH[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_RC_2000WH[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_RC_2000WH[] = { "65A01-1" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"occupied_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"pi_heating_demand", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {9, 0x0201},
    {9, 0x0202},
};

extern const PreparedDefinition kDef_RC_2000WH{
    .zigbee_models=kModels_RC_2000WH, .zigbee_models_count=sizeof(kModels_RC_2000WH)/sizeof(kModels_RC_2000WH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RC-2000WH", .vendor="Leviton",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RC_2000WH, .from_zigbee_count=sizeof(kFz_RC_2000WH)/sizeof(kFz_RC_2000WH[0]),
    .to_zigbee=kTz_RC_2000WH, .to_zigbee_count=sizeof(kTz_RC_2000WH)/sizeof(kTz_RC_2000WH[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
.default_endpoint=9,
};

}  // namespace zhc::devices::leviton
