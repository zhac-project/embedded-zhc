// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings F-MLT-US-2 — parity-extended 2026-04-28.
// Multipurpose sensor (2016 model, manuf SmartThings INC).
// z2m-source: smartthings.ts #F-MLT-US-2.
//
// z2m bundle: temperature + IAS contact (alarm_1, battery_low, tamper) +
//   battery + acceleration (manuSpecificSamsungAccelerometer 0xFC02 — moving,
//   x_axis, y_axis, z_axis).
//
// Runtime gap (Samsung accelerometer cluster not yet implemented in ZHC):
//   manuSpecificSamsungAccelerometer (0xFC02) attrs:
//     0x0000 acceleration_x, 0x0001 acceleration_y, 0x0002 acceleration_z,
//     0x0010 motion → moving (Bool).
//   Three SmartThings ports share this cluster (3321-S, F-MLT-US-2,
//   IM6001-MPP01) — under the 5-port threshold for `_shared.{hpp,cpp}`,
//   so accelerometer surfaces (`moving`, `x_axis`, `y_axis`, `z_axis`)
//   are documented as a runtime gap. Hardware testing required before
//   claiming full parity.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_F_MLT_US_2[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_F_MLT_US_2[] = { "multiv4" };

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

extern const PreparedDefinition kDef_F_MLT_US_2{
    .zigbee_models=kModels_F_MLT_US_2, .zigbee_models_count=sizeof(kModels_F_MLT_US_2)/sizeof(kModels_F_MLT_US_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="F-MLT-US-2", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_F_MLT_US_2, .from_zigbee_count=sizeof(kFz_F_MLT_US_2)/sizeof(kFz_F_MLT_US_2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
