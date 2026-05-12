// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared eurotronic converters.
// Spirit Zigbee wireless heater thermostat (SPZB0001).
// z2m-source: zigbee-herdsman-converters/src/devices/eurotronic.ts #SPZB0001.
#include "definitions/_generic/_shared.hpp"
#include "definitions/eurotronic/_shared.hpp"

namespace zhc::devices::eurotronic {
namespace {

const FzConverter* const kFz_SPZB0001[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::eurotronic::kFzEurotronicThermostat,
};

const TzConverter* const kTz_SPZB0001[] = {
    // Generic ZCL writes (occupied_heating_setpoint, system_mode are
    // covered by `kTzThermostat`; the manu-specific writers below
    // claim the same key set the device emits via the SPA).
    &::zhc::generic::kTzThermostat,
    &::zhc::eurotronic::kTzEurotronicTrvMode,
    &::zhc::eurotronic::kTzEurotronicValvePosition,
    &::zhc::eurotronic::kTzEurotronicCurrentSetpoint,
    &::zhc::eurotronic::kTzEurotronicHostFlags,
    &::zhc::eurotronic::kTzEurotronicMirrorDisplay,
    &::zhc::eurotronic::kTzEurotronicChildLock,
};

constexpr const char* kModels_SPZB0001[] = { "SPZB0001" };

// SystemMode enum string set — z2m: ["off","auto","heat"].
constexpr const char* kSystemModeValues[] = { "off", "auto", "heat" };

// trv_mode enum: 1=manual valve control, 2=auto setpoint control.
constexpr const char* kTrvModeValues[]    = { "1", "2" };

constexpr Expose kAutoExposes[] = {
    {"battery",                ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",                ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"local_temperature",      ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint",  ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"pi_heating_demand",      ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"running_state",          ExposeType::Enum,    Access::State,    nullptr, nullptr,
     kSystemModeValues, 2},  // "idle"/"heat" are 2 strings; reuse storage by intent
    {"system_mode",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
     kSystemModeValues, sizeof(kSystemModeValues)/sizeof(kSystemModeValues[0])},
    {"trv_mode",               ExposeType::Enum,    Access::StateSet, nullptr,
     "Select between direct valve control (1) or automatic setpoint control (2).",
     kTrvModeValues, sizeof(kTrvModeValues)/sizeof(kTrvModeValues[0])},
    {"valve_position",         ExposeType::Numeric, Access::StateSet, nullptr,
     "Direct valve control (0=closed..255=fully open) when trv_mode=1.",
     nullptr, 0},
    {"child_lock",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"mirror_display",         ExposeType::Binary,  Access::StateSet, nullptr,
     "Mirror display (e.g. when device is mounted upside down).",
     nullptr, 0},
    {"error_status",           ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0201},  // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_SPZB0001{
    .zigbee_models           = kModels_SPZB0001,
    .zigbee_models_count     = sizeof(kModels_SPZB0001)/sizeof(kModels_SPZB0001[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "SPZB0001",
    .vendor                  = "Eurotronic",
    .meta                    = nullptr,
    .exposes                 = kAutoExposes,
    .exposes_count           = sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFz_SPZB0001,
    .from_zigbee_count       = sizeof(kFz_SPZB0001)/sizeof(kFz_SPZB0001[0]),
    .to_zigbee               = kTz_SPZB0001,
    .to_zigbee_count         = sizeof(kTz_SPZB0001)/sizeof(kTz_SPZB0001[0]),
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kAutoBindings,
    .bindings_count          = sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::eurotronic
