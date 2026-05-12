// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings 3310-S — parity-extended 2026-04-28.
// Temperature and humidity sensor.
// z2m-source: smartthings.ts #3310-S.
//
// z2m bundle: temperature + fzCentralite.d3310_humidity (manuSpecific
//   Centralite Humidity cluster 0xFC45 manu 0x104E attr 0x0000 -> humidity)
//   + battery.
//
// Runtime gap: manuSpecificCentraliteHumidity (cluster 0xFC45, manu 0x104E,
// attr 0x0000 = humidity %  *100) is not yet wired into ZHC. The standard
// `kFzHumidity` reads cluster 0x0405 (msRelativeHumidity) which the 3310-S
// does NOT report — so until the manuSpecific cluster is added, only
// temperature/battery will be emitted.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_D3310_S[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_D3310_S[] = { "3310-S" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    // humidity — pending manuSpecificCentraliteHumidity cluster wiring.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
};
}  // namespace

extern const PreparedDefinition kDef_D3310_S{
    .zigbee_models=kModels_D3310_S, .zigbee_models_count=sizeof(kModels_D3310_S)/sizeof(kModels_D3310_S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3310-S", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3310_S, .from_zigbee_count=sizeof(kFz_D3310_S)/sizeof(kFz_D3310_S[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
