// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova TAFFETAS2/PERCALE2 — graduated from generated/ 2026-06-10.
// Taffetas 2 / Percale 2 heater (Zigbee thermostat).
//
// z2m-source: acova.ts #TAFFETAS2/PERCALE2.
//
// z2m wires fz.thermostat + fz.hvac_user_interface + fz.occupancy;
// tz adds tz.acova_thermostat_system_mode (vendor-specific systemMode
// lookup) + tz.thermostat_local_temperature_calibration. configure()
// binds hvacThermostat on EP1 and msOccupancySensing on EP2.
//
// An earlier generator pass mistakenly attached kFzIasZone + IAS
// exposes + a 0x0500 binding — the source has no IAS at all. The
// 2026-04-29g sweep replaced that with kFzOccupancy + EP2 0x0406 and
// added the thermostat exposes (unoccupied_heating_setpoint,
// running_state, local_temperature_calibration, occupancy).
//
// Parity fix over that sweep: the def still used the generic
// kFzThermostat, which only decodes 0x0000 / 0x0012 / 0x001C and emits
// 0x0012 as "current_heating_setpoint". So occupied_heating_setpoint,
// unoccupied_heating_setpoint, local_temperature_calibration and
// running_state were ALL dead keys. Now routed through
// kFzAcovaThermostatExtras (see _shared): 0x0012 →
// occupied_heating_setpoint, 0x0014 → unoccupied_heating_setpoint,
// 0x0010 → local_temperature_calibration, 0x0029 → running_state.
//
// PARTIAL: tz.acova_thermostat_system_mode (vendor systemMode enum)
// and fz.hvac_user_interface have no dedicated shared helpers; the
// generic kTzThermostat will write standard systemMode codes which
// the device may not honour for the Acova-specific values.
#include "definitions/acova/_shared.hpp"

namespace zhc::devices::acova {
namespace {
const FzConverter* const kFz_TAFFETAS2_PERCALE2[] = {
    &kFzAcovaThermostatExtras,
    &::zhc::generic::kFzOccupancy,
};
const TzConverter* const kTz_TAFFETAS2_PERCALE2[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_TAFFETAS2_PERCALE2[] = { "PERCALE2 D1.00P1.01Z1.00", "PERCALE2 D1.00P1.02Z1.00", "PERCALE2 D1.00P1.03Z1.00", "TAFFETAS2 D1.00P1.01Z1.00", "TAFFETAS2 D1.00P1.02Z1.00", "TAFFETAS2 D1.00P1.03Z1.00", "TAFFETAS2 D1.00P1.63Z1.19" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"local_temperature",              ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint",      ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"unoccupied_heating_setpoint",    ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"local_temperature_calibration",  ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",                  ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"occupancy",                      ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},  // hvacThermostat on EP1
    {2, 0x0406},  // msOccupancySensing on EP2 (z2m configure binds endpoint2)
};

extern const PreparedDefinition kDef_TAFFETAS2_PERCALE2{
    .zigbee_models=kModels_TAFFETAS2_PERCALE2, .zigbee_models_count=sizeof(kModels_TAFFETAS2_PERCALE2)/sizeof(kModels_TAFFETAS2_PERCALE2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TAFFETAS2/PERCALE2", .vendor="Acova",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TAFFETAS2_PERCALE2, .from_zigbee_count=sizeof(kFz_TAFFETAS2_PERCALE2)/sizeof(kFz_TAFFETAS2_PERCALE2[0]),
    .to_zigbee=kTz_TAFFETAS2_PERCALE2, .to_zigbee_count=sizeof(kTz_TAFFETAS2_PERCALE2)/sizeof(kTz_TAFFETAS2_PERCALE2[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::acova
