// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schneider CCTFR6400 — "Temperature/Humidity measurement with
// thermostat interface" (modelID "Thermostat"). Uses shared schneider converters.
// z2m-source: schneider.ts #CCTFR6400.
//
// GRADUATED from generated/ (hand-edited — do NOT let the generator clobber).
// The thermostat half was wired, but the device's named "Humidity measurement"
// was missing: z2m fromZigbee carries fz.humidity + e.humidity() and binds
// msRelativeHumidity, yet the port had no kFzHumidity. Added the generic
// humidity converter (cluster 0x0405) + expose + binding to restore parity.
// (Temperature surfaces as local_temperature via the hvacThermostat path.)
#include "definitions/_generic/_shared.hpp"
#include "definitions/schneider/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
const FzConverter* const kFz_CCTFR6400[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzHumidity,
    &::zhc::schneider::kFzSchneiderThermostat,
};
const TzConverter* const kTz_CCTFR6400[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
    &::zhc::schneider::kTzSchneiderWiserZoneMode,
    &::zhc::schneider::kTzSchneiderControlType,
    &::zhc::schneider::kTzSchneiderHeatingFuel,
};
constexpr const char* kModels_CCTFR6400[] = { "Thermostat" };

}  // namespace


// exposes + bindings (originally emit_expose_bindings.py; hand-extended with
// the humidity row/binding to match the wired kFzHumidity converter above).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0001},
    {1, 0x0201},
    {1, 0x0405},
};

extern const PreparedDefinition kDef_CCTFR6400{
    .zigbee_models=kModels_CCTFR6400, .zigbee_models_count=sizeof(kModels_CCTFR6400)/sizeof(kModels_CCTFR6400[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CCTFR6400", .vendor="Schneider",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CCTFR6400, .from_zigbee_count=sizeof(kFz_CCTFR6400)/sizeof(kFz_CCTFR6400[0]),
    .to_zigbee=kTz_CCTFR6400, .to_zigbee_count=sizeof(kTz_CCTFR6400)/sizeof(kTz_CCTFR6400[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::schneider
