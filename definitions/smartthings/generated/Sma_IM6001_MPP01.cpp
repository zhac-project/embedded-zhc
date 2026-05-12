// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings IM6001-MPP01 — parity-extended 2026-04-28.
// Multipurpose sensor (2018 model, manuf Samjin Co. Ltd.).
// z2m-source: smartthings.ts #IM6001-MPP01.
//
// z2m bundle: temperature + IAS contact (alarm_1, battery_low, tamper) +
//   battery + acceleration (manuSpecificSamsungAccelerometer).
//
// Runtime gap: manuSpecificSamsungAccelerometer (0xFC02) — see Sma_F_MLT_US_2.cpp
// for the shared comment. `moving`, `x_axis`, `y_axis`, `z_axis` are not
// emitted until the cluster is wired into the static cluster table.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_IM6001_MPP01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_IM6001_MPP01[] = { "multi" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_IM6001_MPP01{
    .zigbee_models=kModels_IM6001_MPP01, .zigbee_models_count=sizeof(kModels_IM6001_MPP01)/sizeof(kModels_IM6001_MPP01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IM6001-MPP01", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IM6001_MPP01, .from_zigbee_count=sizeof(kFz_IM6001_MPP01)/sizeof(kFz_IM6001_MPP01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
