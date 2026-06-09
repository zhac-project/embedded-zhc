// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to add the tank-level + temperature decoders.
// Tank level monitor (propane/oil gauge).
//
// z2m-source: sinope.ts #LM4110ZB — fromZigbee
//   [fz.battery, fz.temperature, fzLocal.tank_level], exposes
//   [battery_low, battery, temperature, tank_level (%)].
//
// The generated def only wired kFzBattery, so the device's whole reason
// for existing — the tank fill level (genAnalogInput presentValue → %)
// — plus the temperature sensor were missing. Add kFzTemperature and the
// Sinopé kFzSinopeTankLevel decoder, and bind genAnalogInput (0x000C) +
// msTemperatureMeasurement (0x0402).
#include "definitions/_generic/_shared.hpp"
#include "definitions/sinope/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_LM4110ZB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::sinope::kFzSinopeTankLevel,  // genAnalogInput presentValue → tank_level %
};

constexpr const char* kModels_LM4110ZB[] = { "LM4110-ZB" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"tank_level", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x000C},  // genAnalogInput (tank level)
};

extern const PreparedDefinition kDef_LM4110ZB{
    .zigbee_models=kModels_LM4110ZB, .zigbee_models_count=sizeof(kModels_LM4110ZB)/sizeof(kModels_LM4110ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LM4110ZB", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LM4110ZB, .from_zigbee_count=sizeof(kFz_LM4110ZB)/sizeof(kFz_LM4110ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sinope
