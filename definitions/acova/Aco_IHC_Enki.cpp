// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova IHC-Enki — graduated from generated/ 2026-06-10.
// Acova Madras IHC towel radiator (Zigbee thermostat).
//
// z2m-source: acova.ts #IHC-Enki.
//
// z2m uses modernExtend `m.thermostat({ setpoints: { values:
// {occupiedHeatingSetpoint: {min:7, max:30, step:0.5}}},
// localTemperatureCalibration: {values:{min:-5, max:5, step:0.1}},
// systemMode: {values: ["off", "heat", "auto"]}})`. m.thermostat
// decodes inbound via fz.thermostat and exposes
// "occupied_heating_setpoint" + "local_temperature_calibration".
// There is **no** on/off, battery, or voltage in the source — an
// earlier generator pass produced a wrong device class (kFzOnOff +
// kFzBattery); the 2026-04-29g sweep rewrote it as a pure thermostat.
//
// Parity fix over that sweep: the def used the generic kFzThermostat,
// which only decodes 0x0000 / 0x0012 / 0x001C and emits 0x0012 as
// "current_heating_setpoint". So occupied_heating_setpoint and
// local_temperature_calibration were dead keys. Now routed through
// kFzAcovaThermostatExtras (see _shared): 0x0012 →
// occupied_heating_setpoint, 0x0010 → local_temperature_calibration.
#include "definitions/acova/_shared.hpp"

namespace zhc::devices::acova {
namespace {

constexpr const char* kModels_IHC_Enki[] = { "IHC-Enki" };

const FzConverter* const kFz_IHC_Enki[] = {
    &kFzAcovaThermostatExtras,
};
const TzConverter* const kTz_IHC_Enki[] = {
    &::zhc::generic::kTzThermostat,
};

constexpr Expose kExp_IHC_Enki[] = {
    {"local_temperature",              ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint",      ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"local_temperature_calibration",  ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_IHC_Enki[] = {
    {1, 0x0201},  // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_IHC_Enki{
    .zigbee_models=kModels_IHC_Enki, .zigbee_models_count=sizeof(kModels_IHC_Enki)/sizeof(kModels_IHC_Enki[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IHC-Enki", .vendor="Acova",
    .meta=nullptr, .exposes=kExp_IHC_Enki, .exposes_count=sizeof(kExp_IHC_Enki)/sizeof(kExp_IHC_Enki[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IHC_Enki, .from_zigbee_count=sizeof(kFz_IHC_Enki)/sizeof(kFz_IHC_Enki[0]),
    .to_zigbee=kTz_IHC_Enki, .to_zigbee_count=sizeof(kTz_IHC_Enki)/sizeof(kTz_IHC_Enki[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_IHC_Enki, .bindings_count=sizeof(kBind_IHC_Enki)/sizeof(kBind_IHC_Enki[0]),
};

}  // namespace zhc::devices::acova
