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
const std::uint8_t kFzPhilipsLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPhilipsLight) / sizeof(kFzPhilipsLight[0]));

const ::zhc::TzConverter* const kTzPhilipsLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzPhilipsLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPhilipsLight) / sizeof(kTzPhilipsLight[0]));

const ::zhc::Expose kExposesPhilipsLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPhilipsLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPhilipsLight) / sizeof(kExposesPhilipsLight[0]));

const ::zhc::BindingSpec kBindingsPhilipsLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsPhilipsLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPhilipsLight) / sizeof(kBindingsPhilipsLight[0]));

// ── White-ambiance light  (philips.m.light({colorTemp})) ────────────
const ::zhc::FzConverter* const kFzPhilipsCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzPhilipsCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPhilipsCTLight) / sizeof(kFzPhilipsCTLight[0]));

const ::zhc::TzConverter* const kTzPhilipsCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzPhilipsCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPhilipsCTLight) / sizeof(kTzPhilipsCTLight[0]));

const ::zhc::Expose kExposesPhilipsCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPhilipsCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPhilipsCTLight) / sizeof(kExposesPhilipsCTLight[0]));

const ::zhc::BindingSpec kBindingsPhilipsCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsPhilipsCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPhilipsCTLight) / sizeof(kBindingsPhilipsCTLight[0]));

// ── Colour-only light  (legacy LLC0xx — Hue Living Colors) ──────────
const ::zhc::FzConverter* const kFzPhilipsColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzPhilipsColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPhilipsColorLight) / sizeof(kFzPhilipsColorLight[0]));

const ::zhc::TzConverter* const kTzPhilipsColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzPhilipsColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPhilipsColorLight) / sizeof(kTzPhilipsColorLight[0]));

const ::zhc::Expose kExposesPhilipsColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPhilipsColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPhilipsColorLight) / sizeof(kExposesPhilipsColorLight[0]));

// ── Full-spectrum colour + CT  (philips.m.light({colorTemp, color})) ─
const ::zhc::FzConverter* const kFzPhilipsColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzPhilipsColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPhilipsColorCTLight) / sizeof(kFzPhilipsColorCTLight[0]));

const ::zhc::TzConverter* const kTzPhilipsColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzPhilipsColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPhilipsColorCTLight) / sizeof(kTzPhilipsColorCTLight[0]));

const ::zhc::Expose kExposesPhilipsColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPhilipsColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPhilipsColorCTLight) / sizeof(kExposesPhilipsColorCTLight[0]));

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
const std::uint8_t kFzPhilipsMotionSensorCount =
    static_cast<std::uint8_t>(sizeof(kFzPhilipsMotionSensor) / sizeof(kFzPhilipsMotionSensor[0]));

const ::zhc::Expose kExposesPhilipsMotionSensor[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",   nullptr, nullptr, 0 },
    { "temperature", ::zhc::ExposeType::Numeric, ::zhc::Access::State, "°C",  nullptr, nullptr, 0 },
    { "illuminance", ::zhc::ExposeType::Numeric, ::zhc::Access::State, "lx",  nullptr, nullptr, 0 },
    { "occupancy",   ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPhilipsMotionSensorCount =
    static_cast<std::uint8_t>(sizeof(kExposesPhilipsMotionSensor) / sizeof(kExposesPhilipsMotionSensor[0]));

const ::zhc::BindingSpec kBindingsPhilipsMotionSensor[] = {
    { 2, 0x0001 }, // genPowerCfg
    { 2, 0x0400 }, // msIlluminanceMeasurement
    { 2, 0x0402 }, // msTemperatureMeasurement
    { 2, 0x0406 }, // msOccupancySensing
};
const std::uint8_t kBindingsPhilipsMotionSensorCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPhilipsMotionSensor) / sizeof(kBindingsPhilipsMotionSensor[0]));

// ── Hue dimmer switch  (RWL020 / RWL021 / RWL022) ───────────────────
const ::zhc::FzConverter* const kFzPhilipsDimmerSwitch[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzHueDimmerNotification,
    &::zhc::generic::kFzCommandRecall,
};
const std::uint8_t kFzPhilipsDimmerSwitchCount =
    static_cast<std::uint8_t>(sizeof(kFzPhilipsDimmerSwitch) / sizeof(kFzPhilipsDimmerSwitch[0]));

const ::zhc::Expose kExposesPhilipsDimmerSwitch[] = {
    { "battery", ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0 },
    { "action",  ::zhc::ExposeType::Enum,    ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPhilipsDimmerSwitchCount =
    static_cast<std::uint8_t>(sizeof(kExposesPhilipsDimmerSwitch) / sizeof(kExposesPhilipsDimmerSwitch[0]));

}  // namespace zhc::devices::philips
