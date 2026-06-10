// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Smartenit ZBHT-1 — temperature & humidity sensor.
// z2m-source: smartenit.ts #ZBHT-1 — fz.battery, fz.temperature, fz.humidity.
//
// Bug fixed: the Tier-1 auto-port dropped fz.temperature (0x0402) and
// fz.humidity (0x0405), reducing this temp/humidity sensor to a
// battery-only device (z2m exposes battery + temperature + humidity).
// Re-wired the generic msTemperatureMeasurement / msRelativeHumidity
// decoders + restored the two missing exposes and report bindings.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartenit {
namespace {
const FzConverter* const kFz_ZBHT_1[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_ZBHT_1[] = { "ZBHT-1" };

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg  (battery)
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_ZBHT_1{
    .zigbee_models=kModels_ZBHT_1, .zigbee_models_count=sizeof(kModels_ZBHT_1)/sizeof(kModels_ZBHT_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZBHT-1", .vendor="Smartenit",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZBHT_1, .from_zigbee_count=sizeof(kFz_ZBHT_1)/sizeof(kFz_ZBHT_1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartenit
