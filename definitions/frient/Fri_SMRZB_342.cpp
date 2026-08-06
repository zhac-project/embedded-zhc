// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Frient SMRZB-342 Smart DIN Relay 2 (z2m v26.92.0 parity, added v26.80.0).
// z2m-source: frient.ts #SMRZB-342
//
// genOnOff relay on endpoint 2 (upstream `endpoint: () => ({default: 2})`)
// plus metering and electrical measurement, plus device temperature.
// Upstream divisors: voltage /100, current /1000, power /1, energy /1000 --
// these live in the cluster's own divisor attributes, which kFzMetering and
// kFzElectricalMeasurement read, so no per-device scaling is baked here.
// Upstream sets powerOnBehavior:false, so no such expose.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::frient {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzDeviceTemperature,
};
const TzConverter* const kTz[] = { &::zhc::generic::kTzOnOff };
constexpr const char* kModels[] = { "SMRZB-342" };

constexpr Expose kExposes[] = {
    {"state",              ExposeType::Binary,  Access::StateSet, nullptr, "Relay state",        nullptr, 0},
    {"power",              ExposeType::Numeric, Access::State,    "W",     "Active power",       nullptr, 0},
    {"energy",             ExposeType::Numeric, Access::State,    "kWh",   "Cumulative energy",  nullptr, 0},
    {"voltage",            ExposeType::Numeric, Access::State,    "V",     "Voltage",            nullptr, 0},
    {"current",            ExposeType::Numeric, Access::State,    "A",     "Current",            nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State,    "C",     "Device temperature", nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {2, 0x0006},
    {2, 0x0702},
    {2, 0x0B04},
    {2, 0x0002},
};
}  // namespace

extern const PreparedDefinition kDef_SMRZB_342{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SMRZB-342", .vendor="Frient",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};

}  // namespace zhc::devices::frient
