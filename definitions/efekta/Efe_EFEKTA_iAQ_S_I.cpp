// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Efekta EFEKTA_iAQ_S_I — CO2 / temp / humidity / illuminance
// air-quality monitor (z2m v26.76.0 parity).
//
// z2m-source: efekta.ts #EFEKTA_iAQ_S_I. Readouts on standard clusters:
// msTemperatureMeasurement 0x0402 (/100), msRelativeHumidity 0x0405 (/100),
// msCO2 0x040D (ppm), msIlluminanceMeasurement 0x0400. Efekta custom-cluster
// config knobs (m.numeric/enumLookup/binary SET exposes) are DEFERRED.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr const char* kModels_EFEKTA_iAQ_S_I[] = { "EFEKTA_iAQ_S_I" };
constexpr Expose kExposes_EFEKTA_iAQ_S_I[] = {
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",           nullptr, nullptr, 0 },
    { "co2",         ExposeType::Numeric, Access::State, "ppm",         nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, Access::State, "lx",          nullptr, nullptr, 0 },
};
const FzConverter* const kFz_EFEKTA_iAQ_S_I[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzCO2,
    &::zhc::generic::kFzIlluminance,
};
constexpr BindingSpec kBindings_EFEKTA_iAQ_S_I[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x040D },   // msCO2
    { 1, 0x0400 },   // msIlluminanceMeasurement
};
}  // namespace
extern const PreparedDefinition kDef_EFEKTA_iAQ_S_I{
    .zigbee_models=kModels_EFEKTA_iAQ_S_I, .zigbee_models_count=sizeof(kModels_EFEKTA_iAQ_S_I)/sizeof(kModels_EFEKTA_iAQ_S_I[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_iAQ_S_I", .vendor="Efekta",
    .meta=nullptr, .exposes=kExposes_EFEKTA_iAQ_S_I, .exposes_count=sizeof(kExposes_EFEKTA_iAQ_S_I)/sizeof(kExposes_EFEKTA_iAQ_S_I[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EFEKTA_iAQ_S_I, .from_zigbee_count=sizeof(kFz_EFEKTA_iAQ_S_I)/sizeof(kFz_EFEKTA_iAQ_S_I[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_EFEKTA_iAQ_S_I, .bindings_count=sizeof(kBindings_EFEKTA_iAQ_S_I)/sizeof(kBindings_EFEKTA_iAQ_S_I[0]),
};
}  // namespace zhc::devices::efekta
