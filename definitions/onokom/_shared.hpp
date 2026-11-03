// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Onokom AC adapters — external temperature sensor controls shared by the
// adapters z2m v26.125.0 gives them (all but GR-1 / TCL-3). hvacThermostat
// manufacturer attributes (mfg 0x4703):
//   0x4750 enum8  allow_the_use_of_an_external_temperature_sensor  No / Yes
//   0x4751 int16  external_air_temperature  /100 °C, −50…50
//   0x4752 uint16 external_temperature_sensor_validity_period
// List kTzOnokomExternalSensor* ahead of the wildcard kTzThermostat.
// z2m-source: onokom.ts (externalTemperatureSensor extend).
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::onokom {

extern const FzConverter kFzOnokomExternalSensor;
extern const TzConverter kTzOnokomExternalSensorAllow;
extern const TzConverter kTzOnokomExternalAirTemperature;
extern const TzConverter kTzOnokomExternalSensorValidity;

inline constexpr const char* kOnokomNoYes[] = { "No", "Yes" };

#define ZHC_ONOKOM_EXTERNAL_SENSOR_EXPOSES \
    {"allow_the_use_of_an_external_temperature_sensor", ExposeType::Enum, Access::StateSet, nullptr, \
     "Allow the use of an external temperature sensor", ::zhc::devices::onokom::kOnokomNoYes, 2, \
     ExposeCategory::Config}, \
    {"external_air_temperature", ExposeType::Numeric, Access::StateSet, "°C", \
     "External air temperature", nullptr, 0, ExposeCategory::Config, -50, 50, 1}, \
    {"external_temperature_sensor_validity_period", ExposeType::Numeric, Access::StateSet, nullptr, \
     "External temperature sensor validity period", nullptr, 0, ExposeCategory::Config, 0, 100, 1}

}  // namespace zhc::devices::onokom
