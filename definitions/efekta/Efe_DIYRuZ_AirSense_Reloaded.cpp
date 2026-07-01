// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Efekta DIYRuZ_AirSense_Reloaded — CO2 / temp / humidity / pressure
// air-quality monitor (z2m v26.76.0 parity).
//
// z2m-source: efekta.ts #DIYRuZ_AirSense_Reloaded. Readouts on standard
// measurement clusters: msTemperatureMeasurement 0x0402 (/100),
// msRelativeHumidity 0x0405 (/100), msCO2 0x040D (ppm), msPressureMeasurement
// 0x0403 (hPa). The Efekta custom-cluster config knobs (auto-brightness,
// forced recalibration, thresholds — z2m m.numeric/binary SET exposes) are
// manu-specific config and DEFERRED; primary readouts are covered here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr const char* kModels_DIYRuZ_AirSense_Reloaded[] = { "DIYRuZ_AirSense_Reloaded" };
constexpr Expose kExposes_DIYRuZ_AirSense_Reloaded[] = {
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",           nullptr, nullptr, 0 },
    { "co2",         ExposeType::Numeric, Access::State, "ppm",         nullptr, nullptr, 0 },
    { "pressure",    ExposeType::Numeric, Access::State, "hPa",         nullptr, nullptr, 0 },
};
const FzConverter* const kFz_DIYRuZ_AirSense_Reloaded[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzCO2,
    &::zhc::generic::kFzPressure,
};
constexpr BindingSpec kBindings_DIYRuZ_AirSense_Reloaded[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x040D },   // msCO2
    { 1, 0x0403 },   // msPressureMeasurement
};
}  // namespace
extern const PreparedDefinition kDef_DIYRuZ_AirSense_Reloaded{
    .zigbee_models=kModels_DIYRuZ_AirSense_Reloaded, .zigbee_models_count=sizeof(kModels_DIYRuZ_AirSense_Reloaded)/sizeof(kModels_DIYRuZ_AirSense_Reloaded[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_AirSense_Reloaded", .vendor="Efekta",
    .meta=nullptr, .exposes=kExposes_DIYRuZ_AirSense_Reloaded, .exposes_count=sizeof(kExposes_DIYRuZ_AirSense_Reloaded)/sizeof(kExposes_DIYRuZ_AirSense_Reloaded[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_AirSense_Reloaded, .from_zigbee_count=sizeof(kFz_DIYRuZ_AirSense_Reloaded)/sizeof(kFz_DIYRuZ_AirSense_Reloaded[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_DIYRuZ_AirSense_Reloaded, .bindings_count=sizeof(kBindings_DIYRuZ_AirSense_Reloaded)/sizeof(kBindings_DIYRuZ_AirSense_Reloaded[0]),
};
}  // namespace zhc::devices::efekta
