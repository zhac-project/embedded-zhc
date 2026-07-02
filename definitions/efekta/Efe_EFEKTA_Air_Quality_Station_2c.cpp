// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Efekta EFEKTA_Air_Quality_Station_2c — CO2 / PM2.5 / temp / humidity /
// illuminance air-quality station (CH2O/formaldehyde variant, z2m v26.76.0 parity).
//
// z2m-source: efekta.ts #EFEKTA_Air_Quality_Station_2c. Core readouts on standard
// clusters: msTemperatureMeasurement 0x0402 (/100), msRelativeHumidity 0x0405
// (/100), msCO2 0x040D (ppm), msIlluminanceMeasurement 0x0400, pm25Measurement
// 0x042A (ug/m3, raw). Extra PM channels, voc_index, nox_index and the variant's
// formaldehyde (msFormaldehyde) lack a generic converter; Efekta custom-cluster
// config knobs (m.numeric/binary STATE_SET) are DEFERRED.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr const char* kModels_EFEKTA_Air_Quality_Station_2c[] = { "EFEKTA_Air_Quality_Station_2c" };
constexpr Expose kExposes_EFEKTA_Air_Quality_Station_2c[] = {
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",           nullptr, nullptr, 0 },
    { "co2",         ExposeType::Numeric, Access::State, "ppm",         nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, Access::State, "lx",          nullptr, nullptr, 0 },
    { "pm25",        ExposeType::Numeric, Access::State, "ug/m3",       nullptr, nullptr, 0 },
};
const FzConverter* const kFz_EFEKTA_Air_Quality_Station_2c[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzCO2,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzPm25,
};
constexpr BindingSpec kBindings_EFEKTA_Air_Quality_Station_2c[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x040D },   // msCO2
    { 1, 0x0400 },   // msIlluminanceMeasurement
    { 1, 0x042A },   // pm25Measurement
};
}  // namespace
extern const PreparedDefinition kDef_EFEKTA_Air_Quality_Station_2c{
    .zigbee_models=kModels_EFEKTA_Air_Quality_Station_2c, .zigbee_models_count=sizeof(kModels_EFEKTA_Air_Quality_Station_2c)/sizeof(kModels_EFEKTA_Air_Quality_Station_2c[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_Air_Quality_Station_2c", .vendor="Efekta",
    .meta=nullptr, .exposes=kExposes_EFEKTA_Air_Quality_Station_2c, .exposes_count=sizeof(kExposes_EFEKTA_Air_Quality_Station_2c)/sizeof(kExposes_EFEKTA_Air_Quality_Station_2c[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EFEKTA_Air_Quality_Station_2c, .from_zigbee_count=sizeof(kFz_EFEKTA_Air_Quality_Station_2c)/sizeof(kFz_EFEKTA_Air_Quality_Station_2c[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_EFEKTA_Air_Quality_Station_2c, .bindings_count=sizeof(kBindings_EFEKTA_Air_Quality_Station_2c)/sizeof(kBindings_EFEKTA_Air_Quality_Station_2c[0]),
};
}  // namespace zhc::devices::efekta
