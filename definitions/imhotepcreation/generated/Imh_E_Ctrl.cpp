// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Imhotepcreation E-Ctrl — hand-rewritten 2026-04-28p sweep,
// upgraded with cross-cutting library converters 2026-04-28p
// post-sweep. Heater thermostat PH25 and compliant. Covers
// whiteLabels "RSS E-Ctrl" (Towel heater THIE) and "RPH E-Ctrl"
// (Panel radiant MPHIE) — same fingerprint, same converters.
// z2m-source: imhotepcreation.ts #E-Ctrl.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::imhotepcreation {
namespace {
const FzConverter* const kFz_E_Ctrl[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzOccupancy,
};
const TzConverter* const kTz_E_Ctrl[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::generic::kTzMinHeatSetpointLimit,
    &::zhc::generic::kTzMaxHeatSetpointLimit,
    &::zhc::generic::kTzSetpointRaiseLower,
};
constexpr const char* kModels_E_Ctrl[] = { "E-Ctrl", "RPH E-Ctrl", "RSS E-Ctrl" };

}  // namespace


// --- exposes match z2m climate + occupancy + min/max setpoint limits ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"min_heat_setpoint_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"max_heat_setpoint_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"setpoint_raise_lower", ExposeType::Numeric, Access::Set, nullptr, nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},   // hvacThermostat
    {1, 0x0406},   // msOccupancySensing — z2m configure binds this
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_E_Ctrl{
    .zigbee_models=kModels_E_Ctrl, .zigbee_models_count=sizeof(kModels_E_Ctrl)/sizeof(kModels_E_Ctrl[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E-Ctrl", .vendor="Imhotepcreation",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E_Ctrl, .from_zigbee_count=sizeof(kFz_E_Ctrl)/sizeof(kFz_E_Ctrl[0]),
    .to_zigbee=kTz_E_Ctrl, .to_zigbee_count=sizeof(kTz_E_Ctrl)/sizeof(kTz_E_Ctrl[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::imhotepcreation
