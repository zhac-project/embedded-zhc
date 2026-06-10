// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZS230002 — hand-rewritten 2026-04-28q; converters corrected 2026-06-10.
// 5-key smart bulb dimmer switch light remote control (battery, sender).
// z2m bundle: fz.command_on + fz.command_off + fz.command_step + fz.command_move
//             + fz.command_stop + fz.command_move_to_color_temp
//             + fz.command_move_to_color + fz.command_move_to_level
//             + fz.command_move_color_temperature + fz.battery.
// Mapped (each generic kFz matches the z2m fz it ports, 1:1):
//   kFzCommandOn / Off / Step / Move / Stop                — basic on/off + dim ramp.
//   kFzCommandMoveToLevel                                  — genLevelCtrl 0x00 → brightness_move_to_level.
//   kFzCommandMoveToColorTemp        (lightingColorCtrl 0x0A) — fz.command_move_to_color_temp → "color_temperature_move".
//   kFzCommandMoveToColor            (lightingColorCtrl 0x07) — fz.command_move_to_color → "color_move".
//   kFzCommandMoveColorTemperature   (lightingColorCtrl 0x4B) — fz.command_move_color_temperature → "color_temperature_move_up/_down/_stop".
//   kFzBattery                                             — battery + voltage.
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. ZS230002 is
//   a battery-powered remote (sender), not a sink — removed.
// FIX 2026-06-10: prior port wired kFzCommandStepColorTemp (cmd 0x4C →
//   "color_temperature_step_*", a DIFFERENT z2m converter not in this bundle)
//   and kFzCommandMoveToHueAndSaturation (cmd 0x06 → phantom
//   "move_to_hue_and_saturation"). z2m's actual fz are command_move_color_temperature
//   (cmd 0x4B) and command_move_to_color (cmd 0x07); both generic converters
//   already exist, so the discrete color_temperature_move_up/_down + color_move
//   actions now decode and the phantom action is gone.
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
    "color_temperature_move_up", "color_temperature_move_down", "color_temperature_move_stop",
    "color_move",
};

const FzConverter* const kFz_ZS230002[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveColorTemperature,
    &::zhc::generic::kFzCommandMoveToColor,
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
