// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings F-ADT-WTR-1 — parity-extended 2026-04-28.
// ADT water leak detector.
// z2m-source: smartthings.ts #F-ADT-WTR-1.
//
// z2m bundle: temperature + IAS water_leak (alarm_1, battery_low) + battery.
// (No tamper expose in z2m for this model.)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_F_ADT_WTR_1[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_F_ADT_WTR_1[] = { "F-ADT-WTR-1" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_F_ADT_WTR_1{
    .zigbee_models=kModels_F_ADT_WTR_1, .zigbee_models_count=sizeof(kModels_F_ADT_WTR_1)/sizeof(kModels_F_ADT_WTR_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="F-ADT-WTR-1", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_F_ADT_WTR_1, .from_zigbee_count=sizeof(kFz_F_ADT_WTR_1)/sizeof(kFz_F_ADT_WTR_1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
