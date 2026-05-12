// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iris 3460-L — hand-rewritten 2026-04-28q.
// Smart button (single button + temperature sensor).
// z2m-source: iris.ts #3460-L.
//   fromZigbee: [fz.command_on, fz.command_off, fz.battery, fz.temperature]
//   toZigbee:   []
//   exposes:    [battery, temperature, action: ["on", "off"]]
//
// Mapped:
//   kFzCommandOn   — genOnOff cmd 0x01 → action "on"
//   kFzCommandOff  — genOnOff cmd 0x00 → action "off"
//   kFzBattery     — genPowerCfg → battery + voltage
//   kFzTemperature — msTemperatureMeasurement → temperature
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iris {
namespace {

const FzConverter* const kFz_D3460_L[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_D3460_L[] = { "3460-L" };

constexpr Expose kAutoExposes[] = {
    {"action",      ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_D3460_L{
    .zigbee_models=kModels_D3460_L, .zigbee_models_count=sizeof(kModels_D3460_L)/sizeof(kModels_D3460_L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3460-L", .vendor="Iris",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3460_L, .from_zigbee_count=sizeof(kFz_D3460_L)/sizeof(kFz_D3460_L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::iris
