// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Selena SC0002 desktop environmental monitoring station.
// New vendor upstream in z2m v26.101.0.
//
// Five stock measurement clusters, no vendor extensions — temperature,
// pressure, humidity, illuminance and CO2 all report on their standard
// clusters, so the generic converters cover it outright.
//
// z2m-source: selena.ts #SC0002.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::selena {
namespace {

const FzConverter* const kFz_SC0002[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzCO2,
};

constexpr const char* kModels_SC0002[] = { "SC0002" };

constexpr Expose kExposes_SC0002[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",   nullptr, nullptr, 0},
    {"pressure",    ExposeType::Numeric, Access::State, "hPa", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",  nullptr, nullptr, 0},
    {"co2",         ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SC0002[] = {
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0403},   // msPressureMeasurement
    {1, 0x0405},   // msRelativeHumidity
    {1, 0x0400},   // msIlluminanceMeasurement
    {1, 0x040D},   // msCO2
};

}  // namespace

extern const PreparedDefinition kDef_SC0002{
    .zigbee_models=kModels_SC0002, .zigbee_models_count=sizeof(kModels_SC0002)/sizeof(kModels_SC0002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SC0002", .vendor="Selena",
    .meta=nullptr,
    .exposes=kExposes_SC0002, .exposes_count=sizeof(kExposes_SC0002)/sizeof(kExposes_SC0002[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SC0002, .from_zigbee_count=sizeof(kFz_SC0002)/sizeof(kFz_SC0002[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SC0002, .bindings_count=sizeof(kBindings_SC0002)/sizeof(kBindings_SC0002[0]),
};

}  // namespace zhc::devices::selena
