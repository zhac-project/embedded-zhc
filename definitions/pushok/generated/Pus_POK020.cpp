// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK020 — battery powered thermostat valve.
// z2m-source: pushok.ts #POK020 (extends m.onOff, m.battery, several rod
// numerics + pushokExtend.pok020Thermostat()).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK020[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_POK020[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_POK020[] = { "POK020" };

}  // namespace


// --- hand-authored exposes block. Climate cluster handled by generic
//     thermostat FZ/TZ; rod_* + external_temperature + control_steps are
//     vendor-defined on genAnalogInput / genAnalogValue / genMultistateOutput
//     and surfaced for UI only (no specialised FZ wired yet). ---
constexpr Expose kAutoExposes[] = {
    {"state",                  ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature",      ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint",   ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"unoccupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"system_mode",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0,
        ExposeCategory::Config},
    {"sensor_source",          ExposeType::Enum,    Access::StateSet, nullptr,
        "Temperature sensor used for control", nullptr, 0, ExposeCategory::Config},
    {"alarm",                  ExposeType::Enum,    Access::State,    nullptr,
        "Calibration status alarm", nullptr, 0, ExposeCategory::Diagnostic},
    {"rod_zero_position",      ExposeType::Numeric, Access::State,    "mm",
        "Rod zero position", nullptr, 0, ExposeCategory::Diagnostic},
    {"rod_length",             ExposeType::Numeric, Access::State,    "mm",
        "Rod length", nullptr, 0, ExposeCategory::Diagnostic},
    {"rod_position",           ExposeType::Numeric, Access::State,    "mm",
        "Rod position", nullptr, 0, ExposeCategory::Diagnostic},
    {"external_temperature",   ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C",
        "External temperature", nullptr, 0},
    {"control_steps",          ExposeType::Enum,    Access::StateSet, nullptr,
        "Control steps preset", nullptr, 0, ExposeCategory::Config},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0001},  // genPowerCfg
    {1, 0x0201},  // hvacThermostat
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK020{
    .zigbee_models=kModels_POK020, .zigbee_models_count=sizeof(kModels_POK020)/sizeof(kModels_POK020[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK020", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK020, .from_zigbee_count=sizeof(kFz_POK020)/sizeof(kFz_POK020[0]),
    .to_zigbee=kTz_POK020, .to_zigbee_count=sizeof(kTz_POK020)/sizeof(kTz_POK020[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
