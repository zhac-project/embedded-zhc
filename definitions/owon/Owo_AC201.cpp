// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Owon AC201 — uses shared owon converters.
// HVAC controller/IR blaster
// Parity fix: z2m exposes a cooling setpoint (.withSetpoint("occupied_cooling_
// setpoint", ...)) decoded by fz.thermostat from hvacThermostat attr 0x0011.
// The generic kFzThermostat decodes only 0x0000/0x0012/0x001C, so
// current_cooling_setpoint was a dead expose. Wired kFzOwonThermostatExtras.
// z2m-source: owon.ts #AC201 + fromZigbee.ts fz.thermostat.
#include "definitions/_generic/_shared.hpp"
#include "definitions/owon/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_AC201[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::owon::kFzOwonThermostatExtras,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_AC201[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_AC201[] = { "AC201" };

}  // namespace


// z2m: climate(off/heat/cool/auto) + heating + cooling + ac_louver + fan_mode + local_temperature.
constexpr Expose kAutoExposes[] = {
    {"local_temperature",        ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"current_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode",                 ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ac_louver_position",       ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},  // hvacThermostat
    {1, 0x0202},  // hvacFanCtrl
};

extern const PreparedDefinition kDef_AC201{
    .zigbee_models=kModels_AC201, .zigbee_models_count=sizeof(kModels_AC201)/sizeof(kModels_AC201[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC201", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AC201, .from_zigbee_count=sizeof(kFz_AC201)/sizeof(kFz_AC201[0]),
    .to_zigbee=kTz_AC201, .to_zigbee_count=sizeof(kTz_AC201)/sizeof(kTz_AC201[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
