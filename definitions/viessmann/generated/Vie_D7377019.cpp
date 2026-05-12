// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Viessmann 7377019 — uses shared viessmann converters.
// ViCare CO2, temperature and humidity sensor.
// z2m-source: viessmann.ts #7377019.
//
// fz: co2 + temperature + humidity + battery (z2m line 143).
#include "definitions/viessmann/_shared.hpp"

namespace zhc::devices::viessmann {
namespace {
const FzConverter* const kFz_D7377019[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &kFzCo2,
};

constexpr const char* kModels_D7377019[] = { "7377019" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x040D},
};

extern const PreparedDefinition kDef_D7377019{
    .zigbee_models=kModels_D7377019, .zigbee_models_count=sizeof(kModels_D7377019)/sizeof(kModels_D7377019[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="7377019", .vendor="Viessmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D7377019, .from_zigbee_count=sizeof(kFz_D7377019)/sizeof(kFz_D7377019[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::viessmann
