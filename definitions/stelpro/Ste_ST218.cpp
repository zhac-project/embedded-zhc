// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared stelpro converters. Graduated from generated/ to
// wire keypad_lockout decode+write (z2m fz.hvac_user_interface +
// tz.thermostat_keypad_lockout); the expose was declared StateSet but
// neither converter was wired, so it was a dead attribute.
// Stelpro ST218 — Ki convector, line-voltage thermostat.
// White-label: SonomaStyle "Style Fan Heater".
// z2m-source: stelpro.ts #ST218 (with SonomaStyle whiteLabel +
// fzLocal.stelpro_thermostat + fz.hvac_user_interface + outdoor-temp
// manuSpec write at mfgcode 0x1185).
#include "definitions/_generic/_shared.hpp"
#include "definitions/stelpro/_shared.hpp"

namespace zhc::devices::stelpro {
namespace {
const FzConverter* const kFz_ST218[] = {
    &::zhc::stelpro::kFzStelproThermostat,
    &::zhc::stelpro::kFzStelproKeypadLockout,
};
const TzConverter* const kTz_ST218[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::stelpro::kTzStelproKeypadLockout,
    &::zhc::stelpro::kTzStelproOutdoorTemp,
};
constexpr const char* kModels_ST218[] = { "ST218", "SonomaStyle" };

constexpr WhiteLabel kWhiteLabels_ST218[] = {
    {"Stelpro", "SonomaStyle"},
};

}  // namespace


// --- hand-authored exposes block. ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature",         ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"keypad_lockout",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"current_heating_setpoint",  ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"system_mode",               ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",             ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
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

extern const PreparedDefinition kDef_ST218{
    .zigbee_models=kModels_ST218, .zigbee_models_count=sizeof(kModels_ST218)/sizeof(kModels_ST218[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ST218", .vendor="Stelpro",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_ST218,
    .white_labels_count=sizeof(kWhiteLabels_ST218)/sizeof(kWhiteLabels_ST218[0]),
    .from_zigbee=kFz_ST218, .from_zigbee_count=sizeof(kFz_ST218)/sizeof(kFz_ST218[0]),
    .to_zigbee=kTz_ST218, .to_zigbee_count=sizeof(kTz_ST218)/sizeof(kTz_ST218[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::stelpro
