// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Plugwise 170-01 — uses shared plugwise converters.
// Emma Wired Pro / Emma Wireless OpenTherm gateway thermostat.
// z2m-source: plugwise.ts #170-01.
//
// Note: 170-01 ships with no battery (Emma Wired Pro is mains-powered;
// Wireless variant uses USB-C). The original auto-port wired
// kFzBattery, which is harmless but never fires — we drop it here.
//
// Decoder coverage:
//   - kFzTemperature           -> local_temperature, humidity (msTemperatureMeasurement / msRelativeHumidity)
//   - kFzHumidity              -> humidity
//   - kFzThermostat            -> local_temperature, occupied_heating_setpoint,
//                                 occupied_cooling_setpoint, system_mode,
//                                 running_state, local_temperature_calibration,
//                                 outdoor_temperature, pi_heating_demand
//   - kFzPlugwiseThermostat    -> boiler_water_temperature, dhw_temperature,
//                                 return_water_temperature, application_fault_code,
//                                 oem_fault_code, max_dhw_setpoint,
//                                 max_boiler_setpoint, external_heat_demand,
//                                 external_heat_demand_timeout
//
// Writer coverage:
//   - kTzThermostat                  -> occupied_heating_setpoint, current_heating_setpoint, system_mode
//   - kTzPlugwiseMaxDhwSetpoint      -> max_dhw_setpoint
//   - kTzPlugwiseMaxBoilerSetpoint   -> max_boiler_setpoint
//   - kTzPlugwiseExternalHeatDemand  -> external_heat_demand
//   - kTzPlugwiseExternalHeatDemandTo-> external_heat_demand_timeout
//
// Not yet ported (see PLUGWISE_PARITY.md): occupied_cooling_setpoint
// and local_temperature_calibration writes — the generic kTzThermostat
// only handles 0x0012/0x001C, so a richer "tz_thermostat_full" or
// dedicated ZclWriteSpec instances would be needed. Listed PARTIAL.
#include "definitions/_generic/_shared.hpp"
#include "definitions/plugwise/_shared.hpp"

namespace zhc::devices::plugwise {
namespace {
const FzConverter* const kFz_D170_01[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzThermostat,
    &kFzPlugwiseThermostat,
};
const TzConverter* const kTz_D170_01[] = {
    &::zhc::generic::kTzThermostat,
    &kTzPlugwiseMaxDhwSetpoint,
    &kTzPlugwiseMaxBoilerSetpoint,
    &kTzPlugwiseExternalHeatDemand,
    &kTzPlugwiseExternalHeatDemandTo,
};

constexpr const char* kModels_D170_01[] = { "170-01" };

}  // namespace


// --- exposes ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature",          ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"humidity",                   ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"outdoor_temperature",        ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"occupied_heating_setpoint",  ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"occupied_cooling_setpoint",  ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"system_mode",                ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",              ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"pi_heating_demand",          ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"boiler_water_temperature",   ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"dhw_temperature",            ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"return_water_temperature",   ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"max_dhw_setpoint",           ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"max_boiler_setpoint",        ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"external_heat_demand",       ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"external_heat_demand_timeout", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"application_fault_code",     ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
    {"oem_fault_code",             ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},   // hvacThermostat
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};
// --- end ---

extern const PreparedDefinition kDef_D170_01{
    .zigbee_models=kModels_D170_01, .zigbee_models_count=sizeof(kModels_D170_01)/sizeof(kModels_D170_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="170-01", .vendor="Plugwise",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D170_01, .from_zigbee_count=sizeof(kFz_D170_01)/sizeof(kFz_D170_01[0]),
    .to_zigbee=kTz_D170_01, .to_zigbee_count=sizeof(kTz_D170_01)/sizeof(kTz_D170_01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::plugwise
