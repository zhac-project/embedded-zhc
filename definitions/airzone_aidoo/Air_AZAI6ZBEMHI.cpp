// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AirzoneAidoo AZAI6ZBEMHI — uses shared airzone_aidoo converters.
// Gateway for two-way control and integration of AirCon Units.AZAI6ZBEMHI for Mitsubishi Heavy
// Parity fix: z2m exposes a cooling setpoint (.withSetpoint("occupied_cooling_
// setpoint", ...)) decoded by fz.thermostat from hvacThermostat attr 0x0011.
// The generic kFzThermostat decodes only 0x0000/0x0012/0x001C, so the cooling
// setpoint was a dead/absent expose. Wired kFzAirzoneThermostatExtras and added
// the current_cooling_setpoint expose. Also corrected system_mode / fan_mode
// from Binary to Enum (z2m withSystemMode / withFanMode), which the prior
// auto-port mistyped.
// z2m-source: airzone_aidoo.ts #AZAI6ZBEMHI + fromZigbee.ts fz.thermostat / fz.fan / fz.on_off.
#include "definitions/_generic/_shared.hpp"
#include "definitions/airzone_aidoo/_shared.hpp"

namespace zhc::devices::airzone_aidoo {
namespace {
const FzConverter* const kFz_AZAI6ZBEMHI[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzThermostat,
    &::zhc::airzone_aidoo::kFzAirzoneThermostatExtras,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_AZAI6ZBEMHI[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_AZAI6ZBEMHI[] = { "Aidoo Zigbee" };

}  // namespace


// z2m exposes: climate(localTemperature, systemMode[off/auto/cool/heat/fan_only/
// dry], fanMode[off/low/medium/high/on/auto], occupied_heating_setpoint,
// occupied_cooling_setpoint) + switch(). The generic thermostat decoder emits
// current_heating_setpoint for the heating setpoint; the cooling setpoint comes
// from the airzone extras converter as current_cooling_setpoint.
constexpr Expose kAutoExposes[] = {
    {"state",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature",        ExposeType::Numeric, Access::State,    "C",     nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"current_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode",                 ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0201},  // hvacThermostat
    {1, 0x0202},  // hvacFanCtrl
};

extern const PreparedDefinition kDef_AZAI6ZBEMHI{
    .zigbee_models=kModels_AZAI6ZBEMHI, .zigbee_models_count=sizeof(kModels_AZAI6ZBEMHI)/sizeof(kModels_AZAI6ZBEMHI[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AZAI6ZBEMHI", .vendor="AirzoneAidoo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AZAI6ZBEMHI, .from_zigbee_count=sizeof(kFz_AZAI6ZBEMHI)/sizeof(kFz_AZAI6ZBEMHI[0]),
    .to_zigbee=kTz_AZAI6ZBEMHI, .to_zigbee_count=sizeof(kTz_AZAI6ZBEMHI)/sizeof(kTz_AZAI6ZBEMHI[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::airzone_aidoo
