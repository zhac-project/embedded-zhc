// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bacchus Duck Pool Thermometer — generic converters only.
// Battery-powered pool thermometer with low-temp alarm.
// z2m-source: bacchus.ts #Duck Pool Thermometer.
//
// Wire shape from z2m source:
//   - msTemperatureMeasurement (0x0402):
//       measuredValue          → temperature (°C)
//       manu 0xf002 u16        → measurment_period (sec) [write]
//       manu 0xf001 u16        → threshold (°C) [write]
//   - genPowerCfg (0x0001) → battery + voltage.
//
// Coverage in this Tier-1 port:
//   - kFzTemperature → temperature.
//   - kFzBattery     → battery + voltage.
//
// Gap: the two manu writes (`measurment_period`, `threshold`) need a
// vendor `bacchus_shared` ZHC_TZ helper with manu fc=0x14 and the
// Bacchus mfgcode (not yet encoded in z2m — z2m emits with the default
// fc=0x10 here). Listed in exposes for parity.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {
const FzConverter* const kFz_Duck_Pool_Thermometer[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Duck_Pool_Thermometer[] = { "Duck Pool Thermometer" };

constexpr Expose kExp_Duck_Pool_Thermometer[] = {
    { "temperature",       ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "measurment_period", ExposeType::Numeric, ::zhc::Access::StateSet, "sec", "Temperature measurement period", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "threshold",         ExposeType::Numeric, ::zhc::Access::StateSet, "\xC2\xB0""C", "Min threshold for alarm binding", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "battery",           ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",           ExposeType::Numeric, ::zhc::Access::State,    "mV",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBind_Duck_Pool_Thermometer[] = {
    { 1, 0x0001 }, // genPowerCfg
    { 1, 0x0402 }, // msTemperatureMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_Duck_Pool_Thermometer{
    .zigbee_models=kModels_Duck_Pool_Thermometer, .zigbee_models_count=sizeof(kModels_Duck_Pool_Thermometer)/sizeof(kModels_Duck_Pool_Thermometer[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Duck Pool Thermometer", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Duck_Pool_Thermometer, .exposes_count=sizeof(kExp_Duck_Pool_Thermometer)/sizeof(kExp_Duck_Pool_Thermometer[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Duck_Pool_Thermometer, .from_zigbee_count=sizeof(kFz_Duck_Pool_Thermometer)/sizeof(kFz_Duck_Pool_Thermometer[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Duck_Pool_Thermometer, .bindings_count=sizeof(kBind_Duck_Pool_Thermometer)/sizeof(kBind_Duck_Pool_Thermometer[0]),
};

}  // namespace zhc::devices::bacchus
