// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy b-parasite — auto-generated.
// b-parasite open source soil moisture sensor
// z2m-source: custom_devices_diy.ts #b-parasite.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: extend=[m.battery, m.temperature, m.humidity, m.soilMoisture, m.illuminance].
// b-parasite: open-source soil-moisture sensor (rbaron/b-parasite).
// TODO(parity): no kFzSoilMoisture (msSoilMoisture 0x0408) or kFzIlluminance helpers
// for these endpoints? kFzIlluminance exists -- include it. soil_moisture remains TODO.
const FzConverter* const kFz_b_parasite[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_b_parasite[] = { "b-parasite" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0400},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0408},
};

extern const PreparedDefinition kDef_b_parasite{
    .zigbee_models=kModels_b_parasite, .zigbee_models_count=sizeof(kModels_b_parasite)/sizeof(kModels_b_parasite[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="b-parasite", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_b_parasite, .from_zigbee_count=sizeof(kFz_b_parasite)/sizeof(kFz_b_parasite[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
