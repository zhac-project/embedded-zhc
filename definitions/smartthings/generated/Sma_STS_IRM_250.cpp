// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings STS-IRM-250 — parity-extended 2026-04-28.
// Motion sensor (2016 model, manuf SmartThings INC).
// z2m-source: smartthings.ts #STS-IRM-250.
//
// z2m bundle: temperature + IAS motion (alarm_1 → occupancy, battery_low,
// tamper) + battery.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_STS_IRM_250[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_STS_IRM_250[] = { "motionv4" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_STS_IRM_250{
    .zigbee_models=kModels_STS_IRM_250, .zigbee_models_count=sizeof(kModels_STS_IRM_250)/sizeof(kModels_STS_IRM_250[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STS-IRM-250", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STS_IRM_250, .from_zigbee_count=sizeof(kFz_STS_IRM_250)/sizeof(kFz_STS_IRM_250[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
