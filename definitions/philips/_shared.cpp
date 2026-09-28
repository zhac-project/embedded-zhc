// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared bundles for Philips Hue ports.  The four light tiers cover
// ~98 % of Hue device coverage; sensors and the dimmer switch use the
// bespoke arrays below.
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {

// ── Plain light  (philips.m.light()) ────────────────────────────────
const ::zhc::FzConverter* const kFzPhilipsLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzPhilipsLightCount == std::size(kFzPhilipsLight));

const ::zhc::TzConverter* const kTzPhilipsLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzPhilipsLightCount == std::size(kTzPhilipsLight));

const ::zhc::Expose kExposesPhilipsLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPhilipsLightCount == std::size(kExposesPhilipsLight));

const ::zhc::BindingSpec kBindingsPhilipsLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsPhilipsLightCount == std::size(kBindingsPhilipsLight));

// ── White-ambiance light  (philips.m.light({colorTemp})) ────────────
const ::zhc::FzConverter* const kFzPhilipsCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzPhilipsCTLightCount == std::size(kFzPhilipsCTLight));

const ::zhc::TzConverter* const kTzPhilipsCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzPhilipsCTLightCount == std::size(kTzPhilipsCTLight));

const ::zhc::Expose kExposesPhilipsCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesPhilipsCTLightCount == std::size(kExposesPhilipsCTLight));

const ::zhc::BindingSpec kBindingsPhilipsCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsPhilipsCTLightCount == std::size(kBindingsPhilipsCTLight));

// ── Colour-only light  (legacy LLC0xx — Hue Living Colors) ──────────
const ::zhc::FzConverter* const kFzPhilipsColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzPhilipsColorLightCount == std::size(kFzPhilipsColorLight));

const ::zhc::TzConverter* const kTzPhilipsColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzPhilipsColorLightCount == std::size(kTzPhilipsColorLight));

const ::zhc::Expose kExposesPhilipsColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPhilipsColorLightCount == std::size(kExposesPhilipsColorLight));

// ── Full-spectrum colour + CT  (philips.m.light({colorTemp, color})) ─
const ::zhc::FzConverter* const kFzPhilipsColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzPhilipsColorCTLightCount == std::size(kFzPhilipsColorCTLight));

const ::zhc::TzConverter* const kTzPhilipsColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzPhilipsColorCTLightCount == std::size(kTzPhilipsColorCTLight));

const ::zhc::Expose kExposesPhilipsColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPhilipsColorCTLightCount == std::size(kExposesPhilipsColorCTLight));

// ── Hue motion sensor (SML00x) ──────────────────────────────────────
// z2m wires `fz.battery, fz.occupancy, fz.temperature` plus `m.illuminance()`.
// `kFzOccupancy` (generic msOccupancySensing 0x0406 decoder, attr 0x0000 bit
// 0) closes z2m's `fz.occupancy` for the `occupancy` expose below — the
// 0x0406 binding is already declared in kBindingsPhilipsMotionSensor.  The
// remaining z2m channels (motion_sensitivity / led_indication /
// occupancy_timeout via philips.tz.hue_motion_*) are Hue-specific
// manuSpecific writes with no generic converter and stay unported (tracked in
// docs/PHILIPS_PARITY.md "runtime gaps").
const ::zhc::FzConverter* const kFzPhilipsMotionSensor[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIlluminance,
};
static_assert(kFzPhilipsMotionSensorCount == std::size(kFzPhilipsMotionSensor));

const ::zhc::Expose kExposesPhilipsMotionSensor[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",   nullptr, nullptr, 0 },
    { "temperature", ::zhc::ExposeType::Numeric, ::zhc::Access::State, "°C",  nullptr, nullptr, 0 },
    { "illuminance", ::zhc::ExposeType::Numeric, ::zhc::Access::State, "lx",  nullptr, nullptr, 0 },
    { "occupancy",   ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPhilipsMotionSensorCount == std::size(kExposesPhilipsMotionSensor));

const ::zhc::BindingSpec kBindingsPhilipsMotionSensor[] = {
    { 2, 0x0001 }, // genPowerCfg
    { 2, 0x0400 }, // msIlluminanceMeasurement
    { 2, 0x0402 }, // msTemperatureMeasurement
    { 2, 0x0406 }, // msOccupancySensing
};
static_assert(kBindingsPhilipsMotionSensorCount == std::size(kBindingsPhilipsMotionSensor));

// ── Hue dimmer switch  (RWL020 / RWL021 / RWL022) ───────────────────
const ::zhc::FzConverter* const kFzPhilipsDimmerSwitch[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzHueDimmerNotification,
    &::zhc::generic::kFzCommandRecall,
};
static_assert(kFzPhilipsDimmerSwitchCount == std::size(kFzPhilipsDimmerSwitch));

const ::zhc::Expose kExposesPhilipsDimmerSwitch[] = {
    { "battery", ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0 },
    { "action",  ::zhc::ExposeType::Enum,    ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPhilipsDimmerSwitchCount == std::size(kExposesPhilipsDimmerSwitch));

}  // namespace zhc::devices::philips
