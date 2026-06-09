// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: CustomDevicesDiy ZeeFlora — graduated from generated/ to wire the
// soil-moisture + illuminance decoders that the generator left as TODOs.
// Flower sensor with rechargeable battery.
// z2m-source: custom_devices_diy.ts #ZeeFlora —
//   fromZigbee=[fz.temperature, fz.soil_moisture, fz.battery], extend=[m.illuminance()].
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// Reports temperature (0x0402), soil moisture (msSoilMoisture 0x0408) and
// illuminance (m.illuminance() → 0x0400) on EP1. kFzSoilMoisture +
// kFzIlluminance now exist in _generic/_shared, so all four decode.
const FzConverter* const kFz_ZeeFlora[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzSoilMoisture,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_ZeeFlora[] = { "ZeeFlora" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0400},
    {1, 0x0402},
    {1, 0x0408},
};

extern const PreparedDefinition kDef_ZeeFlora{
    .zigbee_models=kModels_ZeeFlora, .zigbee_models_count=sizeof(kModels_ZeeFlora)/sizeof(kModels_ZeeFlora[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZeeFlora", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZeeFlora, .from_zigbee_count=sizeof(kFz_ZeeFlora)/sizeof(kFz_ZeeFlora[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
