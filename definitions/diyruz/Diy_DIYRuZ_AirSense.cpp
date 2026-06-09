// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Diyruz DIYRuZ_AirSense — hand-rewritten 2026-04-28; co2 wired
// 2026-06-09.
// Air quality sensor (CO2, temp, humidity, pressure on EP1).
// z2m-source: diyruz.ts #DIYRuZ_AirSense
//   (fromZigbee: [temperature, humidity, co2, pressure, ...config]).
// co2 (z2m fz.co2, msCO2 0x040D measuredValue * 1e6 → ppm) is the headline
// channel; it was exposed but had no decoder while kFzCO2 did not exist.
// The generic now exists, so wire it alongside the standard cluster fz
// (temperature, humidity, pressure). manuSpec config attrs on msCO2/msTemp/
// etc (threshold1/2, *_offset, enable_abc) are diyruz-custom write-only
// settings and remain BLOCKED (would need a diyruz/_shared tz bundle).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {

const FzConverter* const kFz_DIYRuZ_AirSense[] = {
    &::zhc::generic::kFzCO2,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
};

constexpr const char* kModels_DIYRuZ_AirSense[] = { "DIYRuZ_AirSense" };

constexpr Expose kExp_DIYRuZ_AirSense[] = {
    { "co2",         ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "°C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0 },
    { "pressure",    ExposeType::Numeric, Access::State, "hPa", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_DIYRuZ_AirSense[] = {
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0403 },  // msPressureMeasurement
    { 1, 0x040D },  // msCO2
};

}  // namespace

extern const PreparedDefinition kDef_DIYRuZ_AirSense{
    .zigbee_models=kModels_DIYRuZ_AirSense, .zigbee_models_count=sizeof(kModels_DIYRuZ_AirSense)/sizeof(kModels_DIYRuZ_AirSense[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_AirSense", .vendor="Diyruz",
    .meta=nullptr, .exposes=kExp_DIYRuZ_AirSense, .exposes_count=sizeof(kExp_DIYRuZ_AirSense)/sizeof(kExp_DIYRuZ_AirSense[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_AirSense, .from_zigbee_count=sizeof(kFz_DIYRuZ_AirSense)/sizeof(kFz_DIYRuZ_AirSense[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_DIYRuZ_AirSense,
    .bindings_count           = sizeof(kBind_DIYRuZ_AirSense)/sizeof(kBind_DIYRuZ_AirSense[0]),
};

}  // namespace zhc::devices::diyruz
