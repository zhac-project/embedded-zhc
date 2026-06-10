// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke 2AJZ4KPFT — temperature and humidity sensor (hand-rewritten).
// z2m-source: konke.ts #2AJZ4KPFT
//   (fromZigbee: [fz.temperature, fz.humidity, fz.battery];
//    exposes: [e.temperature(), e.humidity(), e.battery()]).
// The Tier-1 auto-port dropped both sensor channels — it wired only
// kFzBattery and exposed only battery/voltage, so temperature + humidity
// reports were silently discarded. Restored msTemperatureMeasurement (0x0402)
// and msRelativeHumidity (0x0405) decoders + the matching exposes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_D2AJZ4KPFT[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D2AJZ4KPFT[] = { "3AFE140103020000", "3AFE220103020000" };

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_D2AJZ4KPFT{
    .zigbee_models=kModels_D2AJZ4KPFT, .zigbee_models_count=sizeof(kModels_D2AJZ4KPFT)/sizeof(kModels_D2AJZ4KPFT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="2AJZ4KPFT", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D2AJZ4KPFT, .from_zigbee_count=sizeof(kFz_D2AJZ4KPFT)/sizeof(kFz_D2AJZ4KPFT[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
