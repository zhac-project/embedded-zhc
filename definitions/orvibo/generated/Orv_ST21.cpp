// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo ST21.
// Temperature & humidity Sensor
// z2m-source: orvibo.ts #ST21.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_ST21[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
constexpr const char* kModels_ST21[] = { "888a434f3cfc47f29ec4a3a03e9fc442" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {2, 0x0405},
};

extern const PreparedDefinition kDef_ST21{
    .zigbee_models=kModels_ST21, .zigbee_models_count=sizeof(kModels_ST21)/sizeof(kModels_ST21[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ST21", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ST21, .from_zigbee_count=sizeof(kFz_ST21)/sizeof(kFz_ST21[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
