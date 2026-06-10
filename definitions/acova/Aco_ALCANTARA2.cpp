// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova ALCANTARA2 — graduated from generated/ 2026-06-10.
// Alcantara 2 heater (Zigbee thermostat).
//
// z2m-source: acova.ts #ALCANTARA2.
//
// z2m maps fz.thermostat + fz.hvac_user_interface; tz wires
// occupied/unoccupied heating setpoint, system_mode, running_state
// and local_temperature.
//
// Parity fix over the earlier hand-rewrite: the def used the generic
// kFzThermostat, which only decodes 0x0000 / 0x0012 / 0x001C AND emits
// 0x0012 under "current_heating_setpoint". This def exposes
// "occupied_heating_setpoint", "unoccupied_heating_setpoint" and
// "running_state" — ALL of which were dead keys under the generic
// decoder. Now routed through kFzAcovaThermostatExtras (see _shared),
// which decodes 0x0012 → occupied_heating_setpoint, 0x0014 →
// unoccupied_heating_setpoint and 0x0029 → running_state.
//
// Outbound: kTzThermostat handles occupied_heating_setpoint +
// system_mode; other writes come through the generic write path.
// fz.hvac_user_interface (keypad lockout / display mode) has no
// shared helper — tracked as PARTIAL.
#include "definitions/acova/_shared.hpp"

namespace zhc::devices::acova {
namespace {
const FzConverter* const kFz_ALCANTARA2[] = {
    &kFzAcovaThermostatExtras,
};
const TzConverter* const kTz_ALCANTARA2[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_ALCANTARA2[] = { "ALCANTARA2 D1.00P1.01Z1.00", "ALCANTARA2 D1.00P1.02Z1.00" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"local_temperature",            ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint",    ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"unoccupied_heating_setpoint",  ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",                  ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",                ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},
};

extern const PreparedDefinition kDef_ALCANTARA2{
    .zigbee_models=kModels_ALCANTARA2, .zigbee_models_count=sizeof(kModels_ALCANTARA2)/sizeof(kModels_ALCANTARA2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ALCANTARA2", .vendor="Acova",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ALCANTARA2, .from_zigbee_count=sizeof(kFz_ALCANTARA2)/sizeof(kFz_ALCANTARA2[0]),
    .to_zigbee=kTz_ALCANTARA2, .to_zigbee_count=sizeof(kTz_ALCANTARA2)/sizeof(kTz_ALCANTARA2[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::acova
