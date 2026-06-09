// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sunricher HK-SENSOR-4IN1-A — graduated from generated/ to fix a real parity gap.
// 4-in-1 sensor (occupancy + temperature + humidity + illuminance).
// z2m-source: sunricher.ts #HK-SENSOR-4IN1-A — m.battery({dontDividePercentage:true}),
//   m.identify(), m.occupancy(), m.temperature(), m.humidity(), m.illuminance().
//
// PARITY FIX (missing core sensors): the auto-generated port lowered only
// kFzBattery, so the device surfaced battery + voltage and dropped all four of its
// primary measurements. A 4-in-1 sensor that reports neither occupancy, temperature,
// humidity nor illuminance is broken. Add kFzOccupancy + kFzTemperature +
// kFzHumidity + kFzIlluminance (all generic decoders match z2m's standard /100 °C,
// /100 %RH, raw lux, and msOccupancySensing bit 0 semantics) and the matching
// exposes + cluster binds. (battery is the z2m `dontDividePercentage` variant — a
// config nuance on the report path, at parity for the value itself.)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_HK_SENSOR_4IN1_A[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_HK_SENSOR_4IN1_A[] = { "HK-SENSOR-4IN1-A" };

}  // namespace

constexpr Expose kExposes_HK_SENSOR_4IN1_A[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_HK_SENSOR_4IN1_A[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0400},  // msIlluminanceMeasurement
};

extern const PreparedDefinition kDef_HK_SENSOR_4IN1_A{
    .zigbee_models=kModels_HK_SENSOR_4IN1_A, .zigbee_models_count=sizeof(kModels_HK_SENSOR_4IN1_A)/sizeof(kModels_HK_SENSOR_4IN1_A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HK-SENSOR-4IN1-A", .vendor="Sunricher",
    .meta=nullptr, .exposes=kExposes_HK_SENSOR_4IN1_A, .exposes_count=sizeof(kExposes_HK_SENSOR_4IN1_A)/sizeof(kExposes_HK_SENSOR_4IN1_A[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HK_SENSOR_4IN1_A, .from_zigbee_count=sizeof(kFz_HK_SENSOR_4IN1_A)/sizeof(kFz_HK_SENSOR_4IN1_A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_HK_SENSOR_4IN1_A,.bindings_count=sizeof(kBindings_HK_SENSOR_4IN1_A)/sizeof(kBindings_HK_SENSOR_4IN1_A[0]),
};

}  // namespace zhc::devices::sunricher
