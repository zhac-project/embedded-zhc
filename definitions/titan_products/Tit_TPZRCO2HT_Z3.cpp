// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TitanProducts TPZRCO2HT-Z3 — Room CO2, humidity & temperature sensor.
//
// Bug fixed (auto-port channel drop): the generated port wired ONLY kFzBattery
// and exposed only battery/voltage — the device had been reduced to battery-only.
// z2m wires fromZigbee:[fz.battery, fz.humidity, fz.temperature, fz.co2] and
// exposes battery_voltage/battery_low/humidity/temperature/co2. The temperature
// (msTemperatureMeasurement 0x0402), humidity (msRelativeHumidity 0x0405, on
// endpoint 2) and CO2 (msCO2 0x040D) channels were all dropped. Restored the
// three generic decoders + the matching exposes + bindings.
//
// Single-channel device (no _l1/_l2) → bare keys, NO endpoint_map. The humidity
// cluster lives on endpoint 2 per z2m configure; kFzHumidity matches on
// WILDCARD_ENDPOINT so it decodes regardless.
//
// z2m-source: titan_products.ts #TPZRCO2HT-Z3.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::titan_products {
namespace {
const FzConverter* const kFz_TPZRCO2HT_Z3[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzCO2,
};

constexpr const char* kModels_TPZRCO2HT_Z3[] = { "TPZRCO2HT-Z3", "TPZRCO2HT-Z3/L" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x040D},  // msCO2
    {2, 0x0405},  // msRelativeHumidity (endpoint 2 per z2m configure)
};

extern const PreparedDefinition kDef_TPZRCO2HT_Z3{
    .zigbee_models=kModels_TPZRCO2HT_Z3, .zigbee_models_count=sizeof(kModels_TPZRCO2HT_Z3)/sizeof(kModels_TPZRCO2HT_Z3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TPZRCO2HT-Z3", .vendor="TitanProducts",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TPZRCO2HT_Z3, .from_zigbee_count=sizeof(kFz_TPZRCO2HT_Z3)/sizeof(kFz_TPZRCO2HT_Z3[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::titan_products
