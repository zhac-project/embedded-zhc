// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Viessmann 7963223 — hand-rewired 2026-04-28q.
// ViCare climate sensor: temperature + humidity + battery.
// z2m-source: viessmann.ts #7963223.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::viessmann {
namespace {
const FzConverter* const kFz_D7963223[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_D7963223[] = { "7963223" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_D7963223{
    .zigbee_models=kModels_D7963223, .zigbee_models_count=sizeof(kModels_D7963223)/sizeof(kModels_D7963223[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="7963223", .vendor="Viessmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D7963223, .from_zigbee_count=sizeof(kFz_D7963223)/sizeof(kFz_D7963223[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::viessmann
