// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nobø SWT-IZ — graduated from generated/ 2026-06-10.
// Smart thermostat for floor heating control.
//
// z2m-source: nobo.ts #SWT-IZ (m.identify + m.thermostat).
//
// Parity fix (WRONG CLASS). The auto-generated stub mis-ported this
// m.thermostat() device as a bare on/off + battery class
// (kFzBattery + kFzOnOff, exposing state/battery/voltage) — every key
// was phantom and the entire thermostat surface (temperature,
// setpoints, modes) was dead. z2m wires a single fromZigbee
// (fz.thermostat) and a climate expose with local_temperature,
// occupied_heating_setpoint, unoccupied_heating_setpoint,
// system_mode([off,heat]) and running_mode([off,heat]). Per project
// rule, thermostats ship FLAT exposes (no ExposeType::Climate).
//
// Now decoded by kFzNoboThermostat (see _shared): 0x0000 →
// local_temperature, 0x0012 → occupied_heating_setpoint (NOT the
// generic dead key current_heating_setpoint), 0x0014 →
// unoccupied_heating_setpoint, 0x001C → system_mode, 0x001E →
// running_mode. Setpoints + system_mode are writable via the generic
// kTzThermostat; running_mode is z2m STATE-only.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nobo/_shared.hpp"

namespace zhc::devices::nobo {
namespace {


constexpr const char* kModels_SWT_IZ[] = { "SWT-IZ" };

const FzConverter* const kFz_SWT_IZ[] = {
    &kFzNoboThermostat,
};
const TzConverter* const kTz_SWT_IZ[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr Expose kExp_SWT_IZ[] = {
    {"local_temperature",           ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint",   ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"unoccupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",                 ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_mode",                ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_SWT_IZ[] = {
    { 1, 0x0201 },  // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_SWT_IZ{
    .zigbee_models=kModels_SWT_IZ, .zigbee_models_count=sizeof(kModels_SWT_IZ)/sizeof(kModels_SWT_IZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SWT-IZ", .vendor="Nobo",
    .meta=nullptr, .exposes=kExp_SWT_IZ, .exposes_count=sizeof(kExp_SWT_IZ)/sizeof(kExp_SWT_IZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SWT_IZ, .from_zigbee_count=sizeof(kFz_SWT_IZ)/sizeof(kFz_SWT_IZ[0]),
    .to_zigbee=kTz_SWT_IZ, .to_zigbee_count=sizeof(kTz_SWT_IZ)/sizeof(kTz_SWT_IZ[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_SWT_IZ,
    .bindings_count           = sizeof(kBind_SWT_IZ)/sizeof(kBind_SWT_IZ[0]),
};

}  // namespace zhc::devices::nobo
