// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HSE2919E — hand-upgraded for full z2m parity.
// Eva water leak sensor — battery + temperature (ep2) + IAS water-leak.
// `endpoint: (device) => ({default: 1})` is informational in z2m; the
// foundation parser already routes attribute reports per endpoint, so
// no `endpoint_map` is needed here.
// z2m-source: datek.ts #HSE2919E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HSE2919E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_HSE2919E[] = { "Water Sensor" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {2, 0x0402},  // msTemperatureMeasurement (endpoint 2 per z2m configure)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HSE2919E{
    .zigbee_models=kModels_HSE2919E, .zigbee_models_count=sizeof(kModels_HSE2919E)/sizeof(kModels_HSE2919E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HSE2919E", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HSE2919E, .from_zigbee_count=sizeof(kFz_HSE2919E)/sizeof(kFz_HSE2919E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
