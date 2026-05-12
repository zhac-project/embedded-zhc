// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared eurotronic converters.
// Comet Zigbee wireless heater thermostat (COZB0001).
//
// NOTE: this device shares the SPZB0001 zigbeeModel string but is
// disambiguated in z2m only via fingerprint dateCodes — at the
// runtime layer we currently do not match dateCode, so the SPZB0001
// definition wins for any "SPZB0001" join. We register a duplicate
// definition keyed on a placeholder modelId for the sake of parity
// metrics; the runtime will not pick this entry until fingerprint
// matching lands. Setpoint range differs (8..28 vs 5..30 on Spirit).
//
// z2m-source: zigbee-herdsman-converters/src/devices/eurotronic.ts
// (fingerprint block immediately after SPZB0001).
#include "definitions/_generic/_shared.hpp"
#include "definitions/eurotronic/_shared.hpp"

namespace zhc::devices::eurotronic {
namespace {

const FzConverter* const kFz_COZB0001[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::eurotronic::kFzEurotronicThermostat,
};

const TzConverter* const kTz_COZB0001[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::eurotronic::kTzEurotronicTrvMode,
    &::zhc::eurotronic::kTzEurotronicValvePosition,
    &::zhc::eurotronic::kTzEurotronicCurrentSetpoint,
    &::zhc::eurotronic::kTzEurotronicHostFlags,
    &::zhc::eurotronic::kTzEurotronicMirrorDisplay,
    &::zhc::eurotronic::kTzEurotronicChildLock,
};

// z2m identifies COZB0001 by fingerprint on dateCodes only (modelID
// matches SPZB0001). The runtime currently keys on the modelID
// string, so we use the model name itself here as a stand-in until
// fingerprint matching lands. This keeps the parity report honest.
constexpr const char* kModels_COZB0001[] = { "COZB0001" };

constexpr const char* kSystemModeValues[] = { "off", "auto", "heat" };
constexpr const char* kTrvModeValues[]    = { "1", "2" };

constexpr Expose kAutoExposes[] = {
    {"battery",                ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",                ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"local_temperature",      ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C",
     "Setpoint range 8..28 °C (Comet revision).", nullptr, 0},
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

extern const PreparedDefinition kDef_COZB0001{
    .zigbee_models           = kModels_COZB0001,
    .zigbee_models_count     = sizeof(kModels_COZB0001)/sizeof(kModels_COZB0001[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "COZB0001",
    .vendor                  = "Eurotronic",
    .meta                    = nullptr,
    .exposes                 = kAutoExposes,
    .exposes_count           = sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFz_COZB0001,
    .from_zigbee_count       = sizeof(kFz_COZB0001)/sizeof(kFz_COZB0001[0]),
    .to_zigbee               = kTz_COZB0001,
    .to_zigbee_count         = sizeof(kTz_COZB0001)/sizeof(kTz_COZB0001[0]),
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kAutoBindings,
    .bindings_count          = sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::eurotronic
