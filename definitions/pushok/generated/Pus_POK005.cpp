// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK005 — temperature + humidity sensor.
// z2m-source: pushok.ts #POK005.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK005[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_POK005[] = { "POK005" };

}  // namespace


// --- hand-authored exposes block (z2m: temperature, humidity, battery) ---
constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK005{
    .zigbee_models=kModels_POK005, .zigbee_models_count=sizeof(kModels_POK005)/sizeof(kModels_POK005[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK005", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK005, .from_zigbee_count=sizeof(kFz_POK005)/sizeof(kFz_POK005[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
