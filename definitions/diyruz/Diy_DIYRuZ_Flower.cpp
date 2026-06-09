// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Diyruz DIYRuZ_Flower — hand-rewritten 2026-04-28; soil_moisture
// wired 2026-06-09.
// Flower sensor: BME280 (EP1: temp/humidity/pressure/illuminance/battery
// + msSoilMoisture) + DS18B20 (EP2: extra temperature).
// z2m-source: diyruz.ts #DIYRuZ_Flower
//   (fromZigbee: [temperature, humidity, soil_moisture, pressure, battery]
//    + extend m.illuminance()).
// soil_moisture (z2m fz.soil_moisture, msSoilMoisture 0x0408 measuredValue
// / 100) is the headline channel; it was exposed but had no decoder while
// kFzSoilMoisture did not exist. The generic now exists, so wire it. The
// multi-EP runtime routes msTemperature reports from EP2 to the "ds"
// label, EP1 to "bme".
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {
const FzConverter* const kFz_DIYRuZ_Flower[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzSoilMoisture,
};

constexpr const char* kModels_DIYRuZ_Flower[] = { "DIYRuZ_Flower" };

constexpr ::zhc::EndpointLabel kEndpoints_DIYRuZ_Flower[] = { {"bme", 1}, {"ds", 2} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",       ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"voltage",       ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0},
    {"temperature",   ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",      ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"pressure",      ExposeType::Numeric, Access::State, "hPa", nullptr, nullptr, 0},
    {"illuminance",   ExposeType::Numeric, Access::State, "lx",  nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement (BME)
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0403},  // msPressureMeasurement
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0408},  // msSoilMoisture (BME)
    {2, 0x0402},  // msTemperatureMeasurement (DS18B20)
};

extern const PreparedDefinition kDef_DIYRuZ_Flower{
    .zigbee_models=kModels_DIYRuZ_Flower, .zigbee_models_count=sizeof(kModels_DIYRuZ_Flower)/sizeof(kModels_DIYRuZ_Flower[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_Flower", .vendor="Diyruz",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_Flower, .from_zigbee_count=sizeof(kFz_DIYRuZ_Flower)/sizeof(kFz_DIYRuZ_Flower[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_DIYRuZ_Flower,
    .endpoint_map_count = sizeof(kEndpoints_DIYRuZ_Flower)/sizeof(kEndpoints_DIYRuZ_Flower[0]),
};

}  // namespace zhc::devices::diyruz
