// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Viessmann ZK03840 — uses shared viessmann converters.
// ViCare radiator thermostat valve.
// z2m-source: viessmann.ts #ZK03840.
//
// Wires the standard hvacThermostat decoder + Viessmann-specific
// manuSpec (mfg=0x1221) read of window_open / window_open_force /
// assembly_mode, plus the keypad_lockout read on hvacUserInterfaceCfg.
// Tz adds window_open_force write (manuSpec) and keypad_lockout
// write (non-manu).
#include "definitions/viessmann/_shared.hpp"

namespace zhc::devices::viessmann {
namespace {
const FzConverter* const kFz_ZK03840[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
    &kFzViessmannThermostat,
    &kFzKeypadLockout,
};
const TzConverter* const kTz_ZK03840[] = {
    &::zhc::generic::kTzThermostat,
    &kTzViessmannWindowOpenForce,
    &kTzKeypadLockout,
};
constexpr const char* kModels_ZK03840[] = { "7637434" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_open_force", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"keypad_lockout", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0201},
};

extern const PreparedDefinition kDef_ZK03840{
    .zigbee_models=kModels_ZK03840, .zigbee_models_count=sizeof(kModels_ZK03840)/sizeof(kModels_ZK03840[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZK03840", .vendor="Viessmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZK03840, .from_zigbee_count=sizeof(kFz_ZK03840)/sizeof(kFz_ZK03840[0]),
    .to_zigbee=kTz_ZK03840, .to_zigbee_count=sizeof(kTz_ZK03840)/sizeof(kTz_ZK03840[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::viessmann
