// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bacchus Mercury_3ph_Counter — generic converters only.
// Zigbee Mercury 3 Phase electricity meter.
// z2m-source: bacchus.ts #Mercury_3ph_Counter.
//
// Same converter shape as Mercury_Counter; the 3-phase variant adds per-phase
// V/I/P attributes (0xfd, 0x9d, etc. on haElectricalMeasurement). The Tier-1
// `kFzElectricalMeasurement` only decodes the canonical attrs (rmsVoltage,
// rmsCurrent, activePower); the per-phase set is listed in exposes for parity
// but won't be pushed by the generic decoder until per-phase support lands.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {
const FzConverter* const kFz_Mercury_3ph_Counter[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_Mercury_3ph_Counter[] = { "Mercury_3ph_Counter" };

constexpr Expose kExp_Mercury_3ph_Counter[] = {
    { "energy",             ExposeType::Numeric, ::zhc::Access::State,    "kWh", nullptr, nullptr, 0 },
    { "power",              ExposeType::Numeric, ::zhc::Access::State,    "W",   nullptr, nullptr, 0 },
    { "voltage",            ExposeType::Numeric, ::zhc::Access::State,    "V",   nullptr, nullptr, 0 },
    { "current",            ExposeType::Numeric, ::zhc::Access::State,    "A",   nullptr, nullptr, 0 },
    { "energy_t1",          ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Energy on tariff 1", nullptr, 0 },
    { "energy_t2",          ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Energy on tariff 2", nullptr, 0 },
    { "energy_t3",          ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Energy on tariff 3", nullptr, 0 },
    { "energy_t4",          ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Energy on tariff 4", nullptr, 0 },
    { "measurement_period", ExposeType::Numeric, ::zhc::Access::StateSet, "sec", "Measurement Period",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "device_address",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Device Address",    nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "temperature",        ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_Mercury_3ph_Counter[] = {
    { 2, 0x0702 },
    { 2, 0x0B04 },
    { 2, 0x0402 },
};

}  // namespace

extern const PreparedDefinition kDef_Mercury_3ph_Counter{
    .zigbee_models=kModels_Mercury_3ph_Counter, .zigbee_models_count=sizeof(kModels_Mercury_3ph_Counter)/sizeof(kModels_Mercury_3ph_Counter[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Mercury_3ph_Counter", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Mercury_3ph_Counter, .exposes_count=sizeof(kExp_Mercury_3ph_Counter)/sizeof(kExp_Mercury_3ph_Counter[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Mercury_3ph_Counter, .from_zigbee_count=sizeof(kFz_Mercury_3ph_Counter)/sizeof(kFz_Mercury_3ph_Counter[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Mercury_3ph_Counter, .bindings_count=sizeof(kBind_Mercury_3ph_Counter)/sizeof(kBind_Mercury_3ph_Counter[0]),
};

}  // namespace zhc::devices::bacchus
