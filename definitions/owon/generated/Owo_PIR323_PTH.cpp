// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon PIR323-PTH — auto-generated.
// Multi-sensor
// z2m-source: owon.ts #PIR323-PTH.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_PIR323_PTH[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_PIR323_PTH[] = { "PIR323-PTH" };

}  // namespace


// z2m: occupancy + battery_low + temperature + humidity (no tamper expose, but kFzIasMotionAlarm still emits the bit if present).
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {2, 0x0402},  // msTemperatureMeasurement
    {2, 0x0405},  // msRelativeHumidity
};

extern const PreparedDefinition kDef_PIR323_PTH{
    .zigbee_models=kModels_PIR323_PTH, .zigbee_models_count=sizeof(kModels_PIR323_PTH)/sizeof(kModels_PIR323_PTH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PIR323-PTH", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PIR323_PTH, .from_zigbee_count=sizeof(kFz_PIR323_PTH)/sizeof(kFz_PIR323_PTH[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
