// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova TAFFETAS2/PERCALE2 — hand-rewritten 2026-04-29g sweep.
// Taffetas 2 / Percale 2 heater (Zigbee thermostat).
//
// z2m-source: acova.ts #TAFFETAS2/PERCALE2.
//
// z2m wires fz.thermostat + fz.hvac_user_interface + fz.occupancy;
// tz adds tz.acova_thermostat_system_mode (vendor-specific systemMode
// lookup) + tz.thermostat_local_temperature_calibration. configure()
// binds hvacThermostat on EP1 and msOccupancySensing on EP2.
//
// The previous generator pass mistakenly attached kFzIasZone and
// emitted IAS exposes (alarm/tamper/battery_low) plus binding
// 0x0500 — the source has no IAS at all. This sweep replaces IAS
// with the correct kFzOccupancy + EP2 0x0406 binding and adds the
// missing thermostat exposes (unoccupied_heating_setpoint,
// running_state, local_temperature_calibration, occupancy).
//
// PARTIAL: tz.acova_thermostat_system_mode (vendor systemMode enum)
// and fz.hvac_user_interface have no dedicated shared helpers; the
// generic kTzThermostat will write standard systemMode codes which
// the device may not honour for the Acova-specific values. Tracked
// in ACOVA_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::acova {
namespace {
const FzConverter* const kFz_TAFFETAS2_PERCALE2[] = {
    &::zhc::generic::kFzThermostat,
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
