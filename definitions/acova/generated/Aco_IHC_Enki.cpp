// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova IHC-Enki — hand-rewritten 2026-04-29g sweep.
// Acova Madras IHC towel radiator (Zigbee thermostat).
//
// z2m-source: acova.ts #IHC-Enki.
//
// z2m uses modernExtend `m.thermostat({ setpoints: { values:
// {occupiedHeatingSetpoint: {min:7, max:30, step:0.5}}},
// localTemperatureCalibration: {values:{min:-5, max:5, step:0.1}},
// systemMode: {values: ["off", "heat", "auto"]}})`. There is **no**
// on/off, battery, or voltage in the source — the previous generator
// pass produced a completely wrong device class (kFzOnOff +
// kFzBattery, exposes state/battery/voltage, bindings genBasic +
// genOnOff). This sweep rewrites it as a pure heating thermostat
// using the standard kFzThermostat / kTzThermostat helpers.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::acova {
namespace {

constexpr const char* kModels_IHC_Enki[] = { "IHC-Enki" };

const FzConverter* const kFz_IHC_Enki[] = {
    &::zhc::generic::kFzThermostat,
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
