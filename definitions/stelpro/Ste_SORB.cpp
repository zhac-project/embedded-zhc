// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared stelpro converters. Graduated from generated/ to
// wire keypad_lockout decode+write (z2m fz.hvac_user_interface +
// tz.thermostat_keypad_lockout); the expose was declared StateSet but
// neither converter was wired, so it was a dead attribute. (z2m exposes
// no fan channel for this "fan heater" — just climate + keypad_lockout.)
// Stelpro SORB — ORLÉANS fan heater. z2m-source: stelpro.ts #SORB
// (fzLocal.stelpro_thermostat + fz.hvac_user_interface; NO outdoor-
// temperature write tz, NO humidity).
#include "definitions/_generic/_shared.hpp"
#include "definitions/stelpro/_shared.hpp"

namespace zhc::devices::stelpro {
namespace {
const FzConverter* const kFz_SORB[] = {
    &::zhc::stelpro::kFzStelproThermostat,
    &::zhc::stelpro::kFzStelproKeypadLockout,
};
const TzConverter* const kTz_SORB[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::stelpro::kTzStelproKeypadLockout,
};
constexpr const char* kModels_SORB[] = { "SORB" };

}  // namespace


// --- hand-authored exposes block. ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature",         ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"keypad_lockout",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"current_heating_setpoint",  ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"system_mode",               ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",             ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
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

extern const PreparedDefinition kDef_SORB{
    .zigbee_models=kModels_SORB, .zigbee_models_count=sizeof(kModels_SORB)/sizeof(kModels_SORB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SORB", .vendor="Stelpro",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SORB, .from_zigbee_count=sizeof(kFz_SORB)/sizeof(kFz_SORB[0]),
    .to_zigbee=kTz_SORB, .to_zigbee_count=sizeof(kTz_SORB)/sizeof(kTz_SORB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::stelpro
