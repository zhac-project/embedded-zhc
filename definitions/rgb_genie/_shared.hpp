// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared RGB Genie plumbing — modelled on iluminize/_shared.
//
// RGB Genie's z2m surface (rgb_genie.ts) is one m.light() dimmer plus
// seven battery-powered or mains-powered scene/dimmer/colour remotes.
// All remotes are senders (clients of On/Off, LevelCtrl, ColorCtrl,
// Scenes) and emit `action` events — they are NOT light sinks. The
// auto-generator collapses every device to "state + brightness +
// kFzOnOff/kTzOnOff", which is the wrong shape for a sender.
//
// Three bundles cover the remotes seen in z2m's rgb_genie.ts:
//
//   Tier             | feature set                                 | bindings
//   -----------------+---------------------------------------------+---------------
//   ActionBattery    | recall + on/off + move + stop + battery     | 0x0006, 0x0008, 0x0001
//   ActionBatteryDim | + step (brightness_step_up/_down)           | + (no extra)
//   ActionBatteryRGB | + ColorCtrl actions (move_to_color_temp,    | + 0x0300
//                    |   move_to_hue_and_saturation, step_ct)      |
//
// PARTIAL note: z2m's `command_move_hue`, `command_move_to_color`,
// `command_move_color_temperature` are not yet wired in
// `_generic/_shared.{hpp,cpp}`. The corresponding action labels
// ("hue_move", "color_move", "color_temperature_move") therefore do
// not fire today. Tracked in docs/parity/RGB_GENIE_PARITY.md.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::rgb_genie {

// ── Action + battery: recall, on/off, move, stop ────────────────────
// Used by ZB-5001 (custom on/off in z2m, but kFzCommandOn/Off produces
// the same output for non-multi-EP devices), ZB-5004.
extern const ::zhc::FzConverter* const kFzActionBattery[];
extern const std::uint8_t              kFzActionBatteryCount;

extern const ::zhc::BindingSpec        kBindingsActionBattery[];
extern const std::uint8_t              kBindingsActionBatteryCount;

// ── + Step (brightness_step_up/_down) ────────────────────────────────
// Used by ZB-5121.
extern const ::zhc::FzConverter* const kFzActionBatteryDim[];
extern const std::uint8_t              kFzActionBatteryDimCount;

extern const ::zhc::BindingSpec        kBindingsActionBatteryDim[];
extern const std::uint8_t              kBindingsActionBatteryDimCount;

// ── + Color/CT actions (step CT, move-to-CT, move-to-hue-and-sat) ───
// Used by ZB-5122 (battery, single-EP), ZB-3008 (mains, multi-EP),
// ZB-3009 (mains, single-EP), ZB-5028 (battery, multi-EP).
//
// Battery-less variants override `kFzActionBatteryRGB` by dropping the
// kFzBattery converter at the device level (or use the same list and
// rely on the device just not reporting genPowerCfg).
extern const ::zhc::FzConverter* const kFzActionBatteryRGB[];
extern const std::uint8_t              kFzActionBatteryRGBCount;

extern const ::zhc::FzConverter* const kFzActionRGBNoBattery[];
extern const std::uint8_t              kFzActionRGBNoBatteryCount;

extern const ::zhc::BindingSpec        kBindingsActionBatteryRGB[];
extern const std::uint8_t              kBindingsActionBatteryRGBCount;

extern const ::zhc::BindingSpec        kBindingsActionRGBNoBattery[];
extern const std::uint8_t              kBindingsActionRGBNoBatteryCount;

}  // namespace zhc::devices::rgb_genie
