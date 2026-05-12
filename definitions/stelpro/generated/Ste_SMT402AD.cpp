// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared stelpro converters.
// Stelpro SMT402AD — Maestro, line-voltage thermostat (anti-dust
// variant). z2m-source: stelpro.ts #SMT402AD (fzLocal.stelpro_
// thermostat + fz.humidity + outdoor-temp manuSpec write at mfgcode
// 0x1185).
#include "definitions/_generic/_shared.hpp"
#include "definitions/stelpro/_shared.hpp"

namespace zhc::devices::stelpro {
namespace {
const FzConverter* const kFz_SMT402AD[] = {
    &::zhc::stelpro::kFzStelproThermostat,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_SMT402AD[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::stelpro::kTzStelproOutdoorTemp,
};
constexpr const char* kModels_SMT402AD[] = { "SMT402AD" };

}  // namespace


// --- hand-authored exposes block. ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature",         ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"keypad_lockout",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity",                  ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
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
    {25, 0x0405},  // msRelativeHumidity
    {25, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_SMT402AD{
    .zigbee_models=kModels_SMT402AD, .zigbee_models_count=sizeof(kModels_SMT402AD)/sizeof(kModels_SMT402AD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SMT402AD", .vendor="Stelpro",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMT402AD, .from_zigbee_count=sizeof(kFz_SMT402AD)/sizeof(kFz_SMT402AD[0]),
    .to_zigbee=kTz_SMT402AD, .to_zigbee_count=sizeof(kTz_SMT402AD)/sizeof(kTz_SMT402AD[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::stelpro
