// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm mTouch_Air_Sensor — auto-generated.
// Air Sensor, temperature & humidity sensor
// z2m-source: ctm.ts #mTouch_Air_Sensor.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
const FzConverter* const kFz_mTouch_Air_Sensor[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_mTouch_Air_Sensor[] = { "Air Sensor" };

}  // namespace


// Manual: temperature/humidity from msTemperatureMeasurement (0x0402) /
// msRelativeHumidity (0x0405) added on top of the auto-generated set.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
};

extern const PreparedDefinition kDef_mTouch_Air_Sensor{
    .zigbee_models=kModels_mTouch_Air_Sensor, .zigbee_models_count=sizeof(kModels_mTouch_Air_Sensor)/sizeof(kModels_mTouch_Air_Sensor[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mTouch_Air_Sensor", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mTouch_Air_Sensor, .from_zigbee_count=sizeof(kFz_mTouch_Air_Sensor)/sizeof(kFz_mTouch_Air_Sensor[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
