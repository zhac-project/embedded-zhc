// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Schneider 550B1024 — Temperature & humidity sensor, zigbeeModel
// "CCT593011_AS". z2m-source: schneider.ts #550B1024.
//
// GRADUATED from generated/ (hand-edited — do NOT let the generator clobber).
// The auto-emit produced a battery-only port; the device's core decode was
// missing. z2m fromZigbee is [fz.humidity, fz.temperature, fz.battery], so the
// sensor never surfaced temperature/humidity. Wired the generic temperature
// (msTemperatureMeasurement 0x0402) + humidity (msRelativeHumidity 0x0405)
// converters + bindings to restore parity.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
const FzConverter* const kFz_D550B1024[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_D550B1024[] = { "CCT593011_AS" };

}  // namespace


// exposes + bindings (originally emit_expose_bindings.py; hand-extended to
// match the wired temperature/humidity converters above).
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
};

extern const PreparedDefinition kDef_D550B1024{
    .zigbee_models=kModels_D550B1024, .zigbee_models_count=sizeof(kModels_D550B1024)/sizeof(kModels_D550B1024[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="550B1024", .vendor="Schneider",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D550B1024, .from_zigbee_count=sizeof(kFz_D550B1024)/sizeof(kFz_D550B1024[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::schneider
