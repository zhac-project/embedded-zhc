// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared eurotronic converters.
// Comet Zero Zigbee wireless heater thermostat (CoZB_dha).
// z2m-source: zigbee-herdsman-converters/src/devices/eurotronic.ts #CoZB_dha.
#include "definitions/_generic/_shared.hpp"
#include "definitions/eurotronic/_shared.hpp"

namespace zhc::devices::eurotronic {
namespace {

const FzConverter* const kFz_CoZB_dha[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::eurotronic::kFzEurotronicThermostat,
};

const TzConverter* const kTz_CoZB_dha[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::eurotronic::kTzEurotronicTrvMode,
    &::zhc::eurotronic::kTzEurotronicValvePosition,
    &::zhc::eurotronic::kTzEurotronicCurrentSetpoint,
    &::zhc::eurotronic::kTzEurotronicHostFlags,
    &::zhc::eurotronic::kTzEurotronicMirrorDisplay,
    &::zhc::eurotronic::kTzEurotronicChildLock,
};

constexpr const char* kModels_CoZB_dha[] = { "CoZB_dha" };

constexpr const char* kSystemModeValues[] = { "off", "auto", "heat" };
constexpr const char* kTrvModeValues[]    = { "1", "2" };

constexpr Expose kAutoExposes[] = {
    {"battery",                ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",                ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"local_temperature",      ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint",  ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"pi_heating_demand",      ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"running_state",          ExposeType::Enum,    Access::State,    nullptr, nullptr,
     kSystemModeValues, 2},
    {"system_mode",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
     kSystemModeValues, sizeof(kSystemModeValues)/sizeof(kSystemModeValues[0])},
    {"trv_mode",               ExposeType::Enum,    Access::StateSet, nullptr,
     "1=manual valve, 2=auto setpoint.",
     kTrvModeValues, sizeof(kTrvModeValues)/sizeof(kTrvModeValues[0])},
    {"valve_position",         ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"child_lock",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"mirror_display",         ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"error_status",           ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0201},
};

}  // namespace

extern const PreparedDefinition kDef_CoZB_dha{
    .zigbee_models           = kModels_CoZB_dha,
    .zigbee_models_count     = sizeof(kModels_CoZB_dha)/sizeof(kModels_CoZB_dha[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "CoZB_dha",
    .vendor                  = "Eurotronic",
    .meta                    = nullptr,
    .exposes                 = kAutoExposes,
    .exposes_count           = sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFz_CoZB_dha,
    .from_zigbee_count       = sizeof(kFz_CoZB_dha)/sizeof(kFz_CoZB_dha[0]),
    .to_zigbee               = kTz_CoZB_dha,
    .to_zigbee_count         = sizeof(kTz_CoZB_dha)/sizeof(kTz_CoZB_dha[0]),
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kAutoBindings,
    .bindings_count          = sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::eurotronic
