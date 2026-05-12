// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iris iL07_1 — hand-rewritten 2026-04-28q.
// Motion + temperature + humidity sensor (zigbeeModel "1117-S").
// z2m-source: iris.ts #iL07_1.
//   fromZigbee: [fz.ias_occupancy_alarm_2, fz.temperature, fz.humidity]
//                + extend m.battery({voltage, voltageReporting, voltageToPercentage})
//   exposes:    [occupancy, battery_low, temperature, humidity, battery]
//   See 3326-L note re alarm_2/alarm_1 bit-selection.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iris {
namespace {
const FzConverter* const kFz_iL07_1[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIasMotionAlarm2,
};

constexpr const char* kModels_iL07_1[] = { "1117-S" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_iL07_1{
    .zigbee_models=kModels_iL07_1, .zigbee_models_count=sizeof(kModels_iL07_1)/sizeof(kModels_iL07_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="iL07_1", .vendor="Iris",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_iL07_1, .from_zigbee_count=sizeof(kFz_iL07_1)/sizeof(kFz_iL07_1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::iris
