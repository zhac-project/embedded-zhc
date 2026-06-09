// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Owon PCT504 — uses shared owon converters.
// HVAC fan coil
// Parity fix: z2m's fz.thermostat decodes running_state (attr 0x0029) and the
// cooling setpoint (attr 0x0011), and fz.occupancy decodes msOccupancySensing
// on ep2. The generic kFzThermostat covers only 0x0000/0x0012/0x001C and the
// def wired no occupancy decoder, so running_state, current_cooling_setpoint
// and occupancy were dead exposes. Wired kFzOwonThermostatExtras + kFzOccupancy.
// z2m-source: owon.ts #PCT504 + fromZigbee.ts fz.thermostat / fz.occupancy.
#include "definitions/_generic/_shared.hpp"
#include "definitions/owon/_shared.hpp"

namespace zhc::devices::owon {
namespace {
// HVAC fan coil unit. Endpoint 1 = thermostat + fan + ui; endpoint 2 = msOccupancy.
// z2m fz: fan, thermostat, humidity, occupancy, hvac_user_interface — IAS is *not* in z2m's bundle.
const FzConverter* const kFz_PCT504[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::owon::kFzOwonThermostatExtras,
    &::zhc::generic::kFzFanMode,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzOccupancy,
};
const TzConverter* const kTz_PCT504[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_PCT504[] = { "PCT504", "PCT504-E" };

}  // namespace


// z2m: full climate (heat/cool/fan_only/sleep), humidity, occupancy, keypad_lockout,
// programming_operation_mode. We expose the subset the runtime can carry today.
// occupancy from msOccupancySensing on ep2 now decodes via kFzOccupancy;
// running_state + current_cooling_setpoint decode via kFzOwonThermostatExtras.
// keypad_lockout (hvacUserInterface 0x0204) has no generic decoder yet — its
// binding is registered but the value will not decode (tracked as PARTIAL).
constexpr Expose kAutoExposes[] = {
    {"local_temperature",          ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"current_heating_setpoint",   ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"current_cooling_setpoint",   ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",                ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",              ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"fan_mode",                   ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity",                   ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
    {"occupancy",                  ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"keypad_lockout",             ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},  // hvacThermostat
    {1, 0x0202},  // hvacFanCtrl
    {1, 0x0204},  // hvacUserInterface (keypad_lockout)
    {1, 0x0405},  // msRelativeHumidity
    {2, 0x0406},  // msOccupancySensing on ep2
};

extern const PreparedDefinition kDef_PCT504{
    .zigbee_models=kModels_PCT504, .zigbee_models_count=sizeof(kModels_PCT504)/sizeof(kModels_PCT504[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PCT504", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PCT504, .from_zigbee_count=sizeof(kFz_PCT504)/sizeof(kFz_PCT504[0]),
    .to_zigbee=kTz_PCT504, .to_zigbee_count=sizeof(kTz_PCT504)/sizeof(kTz_PCT504[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
