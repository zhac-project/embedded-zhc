// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared converter / expose / binding bundles for Philips Hue.  Mirrors
// the same shape used by `definitions/ikea/_shared.{hpp,cpp}`.  The
// vast majority of Hue ports fall into one of four light tiers; sensor
// devices use bespoke arrays declared at the bottom.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {

// ── philips.m.light()  — OnOff + LevelCtrl, no colour ───────────────
extern const ::zhc::FzConverter* const kFzPhilipsLight[];
inline constexpr std::uint8_t          kFzPhilipsLightCount = 2;
extern const ::zhc::TzConverter* const kTzPhilipsLight[];
inline constexpr std::uint8_t          kTzPhilipsLightCount = 2;
extern const ::zhc::Expose             kExposesPhilipsLight[];
inline constexpr std::uint8_t          kExposesPhilipsLightCount = 2;
extern const ::zhc::BindingSpec        kBindingsPhilipsLight[];
inline constexpr std::uint8_t          kBindingsPhilipsLightCount = 2;

// ── philips.m.light({colorTemp: …})  — adds CT cluster 0x0300 ───────
extern const ::zhc::FzConverter* const kFzPhilipsCTLight[];
inline constexpr std::uint8_t          kFzPhilipsCTLightCount = 3;
extern const ::zhc::TzConverter* const kTzPhilipsCTLight[];
inline constexpr std::uint8_t          kTzPhilipsCTLightCount = 3;
extern const ::zhc::Expose             kExposesPhilipsCTLight[];
inline constexpr std::uint8_t          kExposesPhilipsCTLightCount = 3;
extern const ::zhc::BindingSpec        kBindingsPhilipsCTLight[];
inline constexpr std::uint8_t          kBindingsPhilipsCTLightCount = 3;

// ── philips.m.light({color: …}) — colour-only (no CT) ───────────────
extern const ::zhc::FzConverter* const kFzPhilipsColorLight[];
inline constexpr std::uint8_t          kFzPhilipsColorLightCount = 3;
extern const ::zhc::TzConverter* const kTzPhilipsColorLight[];
inline constexpr std::uint8_t          kTzPhilipsColorLightCount = 3;
extern const ::zhc::Expose             kExposesPhilipsColorLight[];
inline constexpr std::uint8_t          kExposesPhilipsColorLightCount = 6;

// ── philips.m.light({colorTemp:…, color:…})  — full Hue colour bulb ─
extern const ::zhc::FzConverter* const kFzPhilipsColorCTLight[];
inline constexpr std::uint8_t          kFzPhilipsColorCTLightCount = 4;
extern const ::zhc::TzConverter* const kTzPhilipsColorCTLight[];
inline constexpr std::uint8_t          kTzPhilipsColorCTLightCount = 4;
extern const ::zhc::Expose             kExposesPhilipsColorCTLight[];
inline constexpr std::uint8_t          kExposesPhilipsColorCTLightCount = 7;

// ── Hue motion sensor (SML00x) — occupancy + temperature + lux ──────
//
// z2m wires `fz.battery, fz.occupancy, fz.temperature` plus
// `m.illuminance()`.  All four channels are ported: occupancy via the
// generic `kFzOccupancy` (msOccupancySensing 0x0406) decoder.  The
// Hue-specific motion_sensitivity / led_indication / occupancy_timeout
// writes (philips.tz.hue_motion_*) have no generic converter and remain
// unported — see `docs/PHILIPS_PARITY.md`.
extern const ::zhc::FzConverter* const kFzPhilipsMotionSensor[];
inline constexpr std::uint8_t          kFzPhilipsMotionSensorCount = 4;
extern const ::zhc::Expose             kExposesPhilipsMotionSensor[];
inline constexpr std::uint8_t          kExposesPhilipsMotionSensorCount = 4;
extern const ::zhc::BindingSpec        kBindingsPhilipsMotionSensor[];
inline constexpr std::uint8_t          kBindingsPhilipsMotionSensorCount = 4;

// ── Hue dimmer switch (RWL020/021/022) — manuSpecificPhilips 0xFC00 ─
extern const ::zhc::FzConverter* const kFzPhilipsDimmerSwitch[];
inline constexpr std::uint8_t          kFzPhilipsDimmerSwitchCount = 3;
extern const ::zhc::Expose             kExposesPhilipsDimmerSwitch[];
inline constexpr std::uint8_t          kExposesPhilipsDimmerSwitchCount = 2;

}  // namespace zhc::devices::philips
