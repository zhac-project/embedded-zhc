// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/onokom/_shared.hpp"

namespace zhc::devices::onokom {
namespace {

constexpr std::uint16_t kOnokomManu = 0x4703;

constexpr ::zhc::generic::ZclWriteLookup kNoYes[] = { {"No", 0}, {"Yes", 1} };

constexpr ::zhc::generic::ZclAttrRow kRows[] = {
    { 0x4750, "allow_the_use_of_an_external_temperature_sensor", 1, kNoYes, 2 },
    { 0x4751, "external_air_temperature", 100 },
    { 0x4752, "external_temperature_sensor_validity_period" },
};
constexpr ::zhc::generic::ZclAttrMap kMap{ kRows, sizeof(kRows)/sizeof(kRows[0]), kOnokomManu };

constexpr ::zhc::generic::ZclWriteSpec kAllowSpec{
    "allow_the_use_of_an_external_temperature_sensor", 0x4750, 0x30, kOnokomManu, kNoYes, 2 };
constexpr ::zhc::generic::ZclWriteSpec kAirTempSpec{
    "external_air_temperature", 0x4751, 0x29, kOnokomManu, nullptr, 0, 0, 100 };
constexpr ::zhc::generic::ZclWriteSpec kValiditySpec{
    "external_temperature_sensor_validity_period", 0x4752, 0x21, kOnokomManu, nullptr, 0 };

}  // namespace

extern const FzConverter kFzOnokomExternalSensor = ::zhc::generic::zcl_attr_fz("hvacThermostat", &kMap);
extern const TzConverter kTzOnokomExternalSensorAllow =
    ::zhc::generic::zcl_write_tz("hvacThermostat", 0x0201, &kAllowSpec);
extern const TzConverter kTzOnokomExternalAirTemperature =
    ::zhc::generic::zcl_write_tz("hvacThermostat", 0x0201, &kAirTempSpec);
extern const TzConverter kTzOnokomExternalSensorValidity =
    ::zhc::generic::zcl_write_tz("hvacThermostat", 0x0201, &kValiditySpec);

}  // namespace zhc::devices::onokom
