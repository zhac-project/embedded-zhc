// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ynoa LA-5KEY-RGBW — hand-curated (was wrong-bundled).
// 5-key control for RGBW light — battery-powered (single endpoint), NOT a
// settable on/off device. The auto-generator emitted kFzOnOff + kTzOnOff + a
// writable `state`, so every key press was dead and it falsely advertised a
// relay. The remote actually drives a bound light by emitting genOnOff /
// genLevelCtrl / lightingColorCtrl cluster commands, surfaced on `action`.
//
// z2m fromZigbee: command_on, command_off, command_move_to_color_temp,
//   command_move_to_color, command_move_to_level, battery.
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: e.battery(), e.battery_low(),
//   e.action(["on","off","brightness_move_to_level",
//             "color_temperature_move","color_move"]).
//
// Action label mapping (generic decoders):
//   kFzCommandOn               → "on"
//   kFzCommandOff              → "off"
//   kFzCommandMoveToLevel      → "brightness_move_to_level"
//   kFzCommandMoveToColorTemp  → "color_temperature_move"
//   kFzCommandMoveToColor      → "color_move"
// battery_low rides genPowerCfg attr 0x0035 (batteryAlarmState) via the
// extended kFzBattery. Single endpoint → bare `action` (no suffix).
//
// z2m-source: ynoa.ts #LA-5KEY-RGBW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ynoa {
namespace {
const FzConverter* const kFz_LA_5KEY_RGBW[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_LA_5KEY_RGBW[] = { "ZBT-RGBWSwitch-D0800" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action",      ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg (z2m configure binds genPowerCfg only)
};

extern const PreparedDefinition kDef_LA_5KEY_RGBW{
    .zigbee_models=kModels_LA_5KEY_RGBW, .zigbee_models_count=sizeof(kModels_LA_5KEY_RGBW)/sizeof(kModels_LA_5KEY_RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LA-5KEY-RGBW", .vendor="Ynoa",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LA_5KEY_RGBW, .from_zigbee_count=sizeof(kFz_LA_5KEY_RGBW)/sizeof(kFz_LA_5KEY_RGBW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ynoa
