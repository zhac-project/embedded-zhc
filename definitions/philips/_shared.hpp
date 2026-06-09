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
extern const std::uint8_t              kFzPhilipsLightCount;
extern const ::zhc::TzConverter* const kTzPhilipsLight[];
extern const std::uint8_t              kTzPhilipsLightCount;
extern const ::zhc::Expose             kExposesPhilipsLight[];
extern const std::uint8_t              kExposesPhilipsLightCount;
extern const ::zhc::BindingSpec        kBindingsPhilipsLight[];
extern const std::uint8_t              kBindingsPhilipsLightCount;

// ── philips.m.light({colorTemp: …})  — adds CT cluster 0x0300 ───────
extern const ::zhc::FzConverter* const kFzPhilipsCTLight[];
extern const std::uint8_t              kFzPhilipsCTLightCount;
extern const ::zhc::TzConverter* const kTzPhilipsCTLight[];
extern const std::uint8_t              kTzPhilipsCTLightCount;
extern const ::zhc::Expose             kExposesPhilipsCTLight[];
extern const std::uint8_t              kExposesPhilipsCTLightCount;
extern const ::zhc::BindingSpec        kBindingsPhilipsCTLight[];
extern const std::uint8_t              kBindingsPhilipsCTLightCount;

// ── philips.m.light({color: …}) — colour-only (no CT) ───────────────
extern const ::zhc::FzConverter* const kFzPhilipsColorLight[];
extern const std::uint8_t              kFzPhilipsColorLightCount;
extern const ::zhc::TzConverter* const kTzPhilipsColorLight[];
extern const std::uint8_t              kTzPhilipsColorLightCount;
extern const ::zhc::Expose             kExposesPhilipsColorLight[];
extern const std::uint8_t              kExposesPhilipsColorLightCount;

// ── philips.m.light({colorTemp:…, color:…})  — full Hue colour bulb ─
extern const ::zhc::FzConverter* const kFzPhilipsColorCTLight[];
extern const std::uint8_t              kFzPhilipsColorCTLightCount;
extern const ::zhc::TzConverter* const kTzPhilipsColorCTLight[];
extern const std::uint8_t              kTzPhilipsColorCTLightCount;
extern const ::zhc::Expose             kExposesPhilipsColorCTLight[];
extern const std::uint8_t              kExposesPhilipsColorCTLightCount;

// ── Hue motion sensor (SML00x) — occupancy + temperature + lux ──────
//
// z2m wires `fz.battery, fz.occupancy, fz.temperature` plus
// `m.illuminance()`.  All four channels are ported: occupancy via the
// generic `kFzOccupancy` (msOccupancySensing 0x0406) decoder.  The
// Hue-specific motion_sensitivity / led_indication / occupancy_timeout
// writes (philips.tz.hue_motion_*) have no generic converter and remain
// unported — see `docs/PHILIPS_PARITY.md`.
extern const ::zhc::FzConverter* const kFzPhilipsMotionSensor[];
extern const std::uint8_t              kFzPhilipsMotionSensorCount;
extern const ::zhc::Expose             kExposesPhilipsMotionSensor[];
extern const std::uint8_t              kExposesPhilipsMotionSensorCount;
extern const ::zhc::BindingSpec        kBindingsPhilipsMotionSensor[];
extern const std::uint8_t              kBindingsPhilipsMotionSensorCount;

// ── Hue dimmer switch (RWL020/021/022) — manuSpecificPhilips 0xFC00 ─
extern const ::zhc::FzConverter* const kFzPhilipsDimmerSwitch[];
extern const std::uint8_t              kFzPhilipsDimmerSwitchCount;
extern const ::zhc::Expose             kExposesPhilipsDimmerSwitch[];
extern const std::uint8_t              kExposesPhilipsDimmerSwitchCount;

}  // namespace zhc::devices::philips
