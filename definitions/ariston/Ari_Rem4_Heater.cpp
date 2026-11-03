// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Ariston Rem4 Heater boiler gateway, room-thermostat interface (z2m
// v26.125.0 window, new vendor). m.onOff (no power-on behaviour) + m.thermostat
// (room temperature, heating setpoint 8.5-30 °C, system_mode off/heat) +
// outdoor_temperature from hvacThermostat outdoorTemp (0x0001, /100, read-only).
// Upstream's ignoreRunningMode strips attr 0x001E before fz.thermostat; our
// thermostat decoder never reads 0x001E, so nothing to strip. The heating_only
// control sequence is decode-only upstream and is not decoded here.
// Upstream tested a single zone (endpoint 2): writes, binds and reporting go
// there (m.onOff onOff 0..MAX/1, m.thermostat localTemp and setpoint
// 0..1 h/10, systemMode 0..1 h/0); converters accept any endpoint.
// z2m-source: ariston.ts #Rem4 Heater.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ariston {
namespace {

constexpr ::zhc::generic::ZclAttrRow kRowsThermostat[] = {
    {0x0001, "outdoor_temperature", 100},
};
constexpr ::zhc::generic::ZclAttrMap kMapThermostat{ kRowsThermostat, sizeof(kRowsThermostat)/sizeof(kRowsThermostat[0]) };
constexpr FzConverter kFzOutdoorTemperature = ::zhc::generic::zcl_attr_fz("hvacThermostat", &kMapThermostat);

const FzConverter* const kFz_Rem4_Heater[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzThermostat,
    &kFzOutdoorTemperature,
};
const TzConverter* const kTz_Rem4_Heater[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_Rem4_Heater[] = { "Rem4 Heater" };

constexpr const char* kSystemModes[] = { "off", "heat" };
constexpr Expose kExp_Rem4_Heater[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C",
     "Room temperature measured by the boiler room unit", nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::State, 8, 30, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModes, 2},
    {"outdoor_temperature", ExposeType::Numeric, Access::State, "°C",
     "Outdoor temperature known by the boiler", nullptr, 0},
};

constexpr BindingSpec kBindings_Rem4_Heater[] = {
    {2, 0x0006},
    {2, 0x0201},
};
constexpr ReportingSpec kReports_Rem4_Heater[] = {
    {2, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},    // onOff
    {2, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},    // localTemp
    {2, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},    // occupiedHeatingSetpoint
    {2, 0x0201, 0x001C, 0x30, 0, 3600, 0, 0},     // systemMode
};

}  // namespace

extern const PreparedDefinition kDef_Rem4_Heater{
    .zigbee_models=kModels_Rem4_Heater, .zigbee_models_count=sizeof(kModels_Rem4_Heater)/sizeof(kModels_Rem4_Heater[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Rem4 Heater", .vendor="Ariston",
    .meta=nullptr, .exposes=kExp_Rem4_Heater, .exposes_count=sizeof(kExp_Rem4_Heater)/sizeof(kExp_Rem4_Heater[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Rem4_Heater, .from_zigbee_count=sizeof(kFz_Rem4_Heater)/sizeof(kFz_Rem4_Heater[0]),
    .to_zigbee=kTz_Rem4_Heater, .to_zigbee_count=sizeof(kTz_Rem4_Heater)/sizeof(kTz_Rem4_Heater[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_Rem4_Heater,
    .bindings_count=sizeof(kBindings_Rem4_Heater)/sizeof(kBindings_Rem4_Heater[0]),
    .reports=kReports_Rem4_Heater,
    .reports_count=sizeof(kReports_Rem4_Heater)/sizeof(kReports_Rem4_Heater[0]),
    .default_endpoint=2,
};

}  // namespace zhc::devices::ariston
