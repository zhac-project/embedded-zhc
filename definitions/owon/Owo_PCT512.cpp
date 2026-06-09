// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Owon PCT512 — uses shared owon converters.
// Thermostat
// Parity fix: z2m's climate surface exposes running_state (heat/idle), decoded
// by fz.thermostat from hvacThermostat attr 0x0029, and m.occupancy() decoded
// by fz.occupancy from msOccupancySensing. The generic kFzThermostat covers
// only 0x0000/0x0012/0x001C and the def wired no occupancy decoder, so
// running_state and occupancy were dead exposes. Wired kFzOwonThermostatExtras
// + kFzOccupancy.
// z2m-source: owon.ts #PCT512 + fromZigbee.ts fz.thermostat / fz.occupancy.
#include "definitions/_generic/_shared.hpp"
#include "definitions/owon/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_PCT512[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::owon::kFzOwonThermostatExtras,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzOccupancy,
};
const TzConverter* const kTz_PCT512[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_PCT512[] = { "PCT512" };

}  // namespace


// z2m: climate (off/heat) + running_state (heat/idle) + occupied_heating_setpoint(5..30,0.5)
// + humidity + occupancy (extend). running_state decodes via kFzOwonThermostatExtras
// (attr 0x0029); occupancy decodes via kFzOccupancy (msOccupancySensing).
constexpr Expose kAutoExposes[] = {
    {"local_temperature",        ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",            ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"humidity",                 ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
    {"occupancy",                ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},  // hvacThermostat
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0406},  // msOccupancySensing
};

extern const PreparedDefinition kDef_PCT512{
    .zigbee_models=kModels_PCT512, .zigbee_models_count=sizeof(kModels_PCT512)/sizeof(kModels_PCT512[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PCT512", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PCT512, .from_zigbee_count=sizeof(kFz_PCT512)/sizeof(kFz_PCT512[0]),
    .to_zigbee=kTz_PCT512, .to_zigbee_count=sizeof(kTz_PCT512)/sizeof(kTz_PCT512[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
