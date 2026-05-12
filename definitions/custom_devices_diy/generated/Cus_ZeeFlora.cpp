// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy ZeeFlora — auto-generated.
// Flower sensor with rechargeable battery
// z2m-source: custom_devices_diy.ts #ZeeFlora.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[fz.temperature, fz.soil_moisture, fz.battery]; multiEndpoint device
// that reports temperature on EP1 and soil moisture on cluster msSoilMoisture (0x0408).
// TODO(parity): no kFzSoilMoisture helper yet - msSoilMoisture (0x0408) attr 0x0000
// is not decoded. Add soil_moisture decoder to _generic/_shared.
const FzConverter* const kFz_ZeeFlora[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_ZeeFlora[] = { "ZeeFlora" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
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
