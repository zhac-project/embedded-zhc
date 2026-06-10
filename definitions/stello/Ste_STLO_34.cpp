// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared stelpro converters. Graduated from generated/ to
// wire keypad_lockout decode+write (z2m fz.hvac_user_interface +
// tz.thermostat_keypad_lockout); the expose was declared StateSet but
// neither converter was wired, so it was a dead attribute.
// Stello STLO-34 — Hilo thermostat.
// z2m-source: stello.ts #STLO-34. fromZigbee = stelpro_thermostat +
// fz.hvac_user_interface + power + energy. Hilo edition: power/energy
// live on hvacThermostat 0x4008/0x4009 (NOT seMetering); peak-demand-icon
// and outdoor-temp manuSpec writes at mfgcode 0x1185; configure binds EP25.
#include "definitions/_generic/_shared.hpp"
#include "definitions/stelpro/_shared.hpp"

namespace zhc::devices::stello {
namespace {
const FzConverter* const kFz_STLO_34[] = {
    &::zhc::stelpro::kFzStelproThermostat,
    &::zhc::stelpro::kFzStelproKeypadLockout,  // z2m fz.hvac_user_interface
    &::zhc::stelpro::kFzStelproPower,
    &::zhc::stelpro::kFzStelproEnergy,
};
const TzConverter* const kTz_STLO_34[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::stelpro::kTzStelproKeypadLockout,  // z2m tz.thermostat_keypad_lockout
    &::zhc::stelpro::kTzStelproOutdoorTemp,
    &::zhc::stelpro::kTzStelproPeakDemandIcon,
};
constexpr const char* kModels_STLO_34[] = { "STLO-34" };

}  // namespace


// --- hand-authored exposes block (z2m-parity for STLO-34). ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature",         ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"keypad_lockout",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power",                     ExposeType::Numeric, Access::State,    "W",   nullptr, nullptr, 0},
    {"energy",                    ExposeType::Numeric, Access::State,    "kWh", nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"current_heating_setpoint",  ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"system_mode",               ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",             ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"peak_demand_icon",          ExposeType::Numeric, Access::Set,      "hours",
        "Set peak demand event icon for the specified number of hours", nullptr, 0,
        ExposeCategory::Config},
    {"outdoor_temperature_display", ExposeType::Numeric, Access::Set,    "\xC2\xB0""C",
        "Outdoor temperature displayed on the thermostat", nullptr, 0,
        ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {25, 0x0000},  // genBasic
    {25, 0x0003},  // genIdentify
    {25, 0x0004},  // genGroups
    {25, 0x0201},  // hvacThermostat
    {25, 0x0204},  // hvacUserInterfaceCfg
    {25, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_STLO_34{
    .zigbee_models=kModels_STLO_34, .zigbee_models_count=sizeof(kModels_STLO_34)/sizeof(kModels_STLO_34[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STLO-34", .vendor="Stello",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STLO_34, .from_zigbee_count=sizeof(kFz_STLO_34)/sizeof(kFz_STLO_34[0]),
    .to_zigbee=kTz_STLO_34, .to_zigbee_count=sizeof(kTz_STLO_34)/sizeof(kTz_STLO_34[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::stello
