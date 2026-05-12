// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZS230002 — hand-rewritten 2026-04-28q.
// 5-key smart bulb dimmer switch light remote control (battery, sender).
// z2m bundle: fz.command_on + fz.command_off + fz.command_step + fz.command_move
//             + fz.command_stop + fz.command_move_to_color_temp
//             + fz.command_move_to_color + fz.command_move_to_level
//             + fz.command_move_color_temperature + fz.battery.
// Mapped:
//   kFzCommandOn / Off / Step / Move / Stop                — basic on/off + dim ramp.
//   kFzCommandMoveToLevel                                  — 0x04 moveToLevelWithOnOff.
//   kFzCommandMoveToColorTemp                              — 0x0A moveToColorTemp.
//   kFzCommandMoveToHueAndSaturation                       — 0x06 (covers move_to_color hs path).
//   kFzCommandStepColorTemp                                — color_temperature_move_up/_down.
//   kFzBattery                                             — battery + voltage.
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. ZS230002 is
//   a battery-powered remote (sender), not a sink — removed.
// PARTIAL: z2m's `fz.command_move_color_temperature` (cluster 0x0300 cmd 0x4B,
//   continuous color-temp move) maps to action "color_temperature_move" with no
//   matching kFz today; kFzCommandStepColorTemp covers the discrete step variant
//   (cmd 0x4C) which is what most remotes report. See linkind parity doc.
// NOTE: `fz.command_move_to_color` action key is "color_move"; we don't have a
//   dedicated kFzCommandMoveToColor — kFzCommandMoveToHueAndSaturation handles
//   the HSV form and is the closest match in our generic set.
// z2m-source: linkind.ts #ZS230002.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZS230002[] = { "ZBT-RGBWSwitch-D0801" };

constexpr const char* kActions_ZS230002[] = {
    "on", "off",
    "brightness_step_up", "brightness_step_down",
    "brightness_move_up", "brightness_move_down", "brightness_stop",
    "brightness_move_to_level",
    "color_temperature_move",
    "color_temperature_move_up", "color_temperature_move_down",
    "color_move",
    "move_to_hue_and_saturation",
};

const FzConverter* const kFz_ZS230002[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzBattery,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_ZS230002,
      sizeof(kActions_ZS230002)/sizeof(kActions_ZS230002[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl
};

extern const PreparedDefinition kDef_ZS230002{
    .zigbee_models=kModels_ZS230002, .zigbee_models_count=sizeof(kModels_ZS230002)/sizeof(kModels_ZS230002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS230002", .vendor="Linkind",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS230002, .from_zigbee_count=sizeof(kFz_ZS230002)/sizeof(kFz_ZS230002[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::linkind
