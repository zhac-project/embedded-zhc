// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E2112 — VINDSTYRKA air quality & humidity sensor.
//
// z2m-source: ikea.ts #E2112 — m.temperature() + m.humidity() +
// m.pm25() + ikeaVoc() (Sensirion VOC index).
//
// The auto-generated def wired the full kFzIkeaLight bundle — a phantom
// light on a 4-channel environment sensor. Replaced with the generic
// temperature (msTemperatureMeasurement 0x0402) + humidity
// (msRelativeHumidity 0x0405) decoders, which match z2m's m.temperature
// + m.humidity exactly.
//
// NOT yet wired (no generic decoder exists — INFRA, deferred):
//   - pm25: IKEA exposes PM2.5 on a custom pm25Measurement cluster
//     (0x042a, attr 0x0000 SINGLE_PREC), not the standard ZCL
//     pm25Measurement layout — needs a dedicated converter.
//   - voc_index: manuSpecificIkeaVocIndexMeasurement manufacturer
//     cluster — needs a manuSpecific decoder.
// These remain TODO and are tracked as separate infra tickets.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {
const FzConverter* const kFz_E2112[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_E2112[] = { "VINDSTYRKA" };

constexpr Expose kExposes_E2112[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E2112[] = {
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_E2112{
    .zigbee_models=kModels_E2112, .zigbee_models_count=sizeof(kModels_E2112)/sizeof(kModels_E2112[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2112", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposes_E2112, .exposes_count=sizeof(kExposes_E2112)/sizeof(kExposes_E2112[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E2112, .from_zigbee_count=sizeof(kFz_E2112)/sizeof(kFz_E2112[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E2112, .bindings_count=sizeof(kBindings_E2112)/sizeof(kBindings_E2112[0]),
};

}  // namespace zhc::devices::ikea
