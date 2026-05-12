// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HSE2920E — hand-upgraded for full z2m parity.
// Door/window sensor — IAS contact alarm + temperature.
// z2m-source: datek.ts #HSE2920E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HSE2920E[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_HSE2920E[] = { "Door/Window Sensor" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},  // ssIasZone
    {1, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HSE2920E{
    .zigbee_models=kModels_HSE2920E, .zigbee_models_count=sizeof(kModels_HSE2920E)/sizeof(kModels_HSE2920E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HSE2920E", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HSE2920E, .from_zigbee_count=sizeof(kFz_HSE2920E)/sizeof(kFz_HSE2920E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
