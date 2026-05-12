// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Plugwise 106-03 — uses shared plugwise converters.
// Tom thermostatic radiator valve.
// z2m-source: plugwise.ts #106-03.
//
// Notes (vs z2m):
// * `system_mode` and `occupied_heating_setpoint` are NOT supported by
//   the device firmware (Z2M PR koenkk/zigbee2mqtt.io#1666).
// * Decoder coverage:
//     - kFzBattery               -> battery / voltage
//     - kFzTemperature           -> local_temperature (msTemperatureMeasurement)
//     - kFzThermostat            -> pIHeatingDemand -> "pi_heating_demand"
//     - kFzPlugwiseThermostat    -> manuSpec attrs (valve_position, force,
//                                   radio_strength, error_status,
//                                   plugwise_t_diff, current_heating_setpoint)
// * Writer coverage:
//     - kTzPlugwiseValvePosition  -> "valve_position"
//     - kTzPlugwisePushForce      -> "force" (string lookup)
//     - kTzPlugwiseRadioStrength  -> "radio_strength" (string lookup)
//     - kTzPlugwiseCalibrateValve -> "calibrate_valve" (cluster command 0xA0)
#include "definitions/_generic/_shared.hpp"
#include "definitions/plugwise/_shared.hpp"

namespace zhc::devices::plugwise {
namespace {
const FzConverter* const kFz_D106_03[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzThermostat,
    &kFzPlugwiseThermostat,
};
const TzConverter* const kTz_D106_03[] = {
    &kTzPlugwiseValvePosition,
    &kTzPlugwisePushForce,
    &kTzPlugwiseRadioStrength,
    &kTzPlugwiseCalibrateValve,
};
constexpr const char* kModels_D106_03[] = { "106-03" };

}  // namespace


// --- exposes ---
constexpr Expose kAutoExposes[] = {
    {"battery",            ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",            ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"local_temperature",  ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"pi_heating_demand",  ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"valve_position",     ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"force",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"radio_strength",     ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"calibrate_valve",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0000},   // genBasic
    {1, 0x0001},   // genPowerCfg
    {1, 0x0201},   // hvacThermostat
};
// --- end ---

extern const PreparedDefinition kDef_D106_03{
    .zigbee_models=kModels_D106_03, .zigbee_models_count=sizeof(kModels_D106_03)/sizeof(kModels_D106_03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="106-03", .vendor="Plugwise",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D106_03, .from_zigbee_count=sizeof(kFz_D106_03)/sizeof(kFz_D106_03[0]),
    .to_zigbee=kTz_D106_03, .to_zigbee_count=sizeof(kTz_D106_03)/sizeof(kTz_D106_03[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::plugwise
