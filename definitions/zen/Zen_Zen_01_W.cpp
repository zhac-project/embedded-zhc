// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zen Zen-01-W — graduated from generated/ for a real decoder fix.
// Thermostat (heat + cool + fan).
//
// z2m wires `fz.thermostat` (full hvacThermostat surface) + `fz.fan` +
// `fz.battery`. The generic `kFzThermostat` decodes only 0x0000 / 0x0012 /
// 0x001C, leaving three declared exposes dead: occupied_cooling_setpoint
// (0x0011), running_state (0x0029) and local_temperature_calibration (0x0010).
// The vendor `kFzZenThermostat` (definitions/zen/_shared.cpp) decodes those
// alongside the generic decoder; fan_mode stays on the generic kFzFanMode.
// z2m-source: zen.ts #Zen-01-W.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zen/_shared.hpp"

namespace zhc::devices::zen {
namespace {
const FzConverter* const kFz_Zen_01_W[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
    &kFzZenThermostat,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_Zen_01_W[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_Zen_01_W[] = { "Zen-01" };

}  // namespace


// Exposes mirror z2m: climate(heat+cool setpoints, local_temperature,
// system_mode, running_state, local_temperature_calibration, fan_mode) flat
// per project rule (no ExposeType::Climate) + battery_voltage. system_mode /
// fan_mode are enums (off/auto/heat/cool/emergency_heating and auto/on); the
// auto-port wrongly typed them Binary and omitted the cooling setpoint,
// running_state and calibration exposes — all now restored.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"occupied_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0201},
    {1, 0x0202},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_Zen_01_W{
    .zigbee_models=kModels_Zen_01_W, .zigbee_models_count=sizeof(kModels_Zen_01_W)/sizeof(kModels_Zen_01_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Zen-01-W", .vendor="Zen",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Zen_01_W, .from_zigbee_count=sizeof(kFz_Zen_01_W)/sizeof(kFz_Zen_01_W[0]),
    .to_zigbee=kTz_Zen_01_W, .to_zigbee_count=sizeof(kTz_Zen_01_W)/sizeof(kTz_Zen_01_W[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::zen
