// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Six shared bundles for Ledvance ports — one per feature combination
// observed in z2m's ledvance.ts. Modelled after osram/_shared.
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {

// ── On/off only (ledvanceOnOff()) ────────────────────────────────────
const ::zhc::FzConverter* const kFzLedvanceOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
static_assert(kFzLedvanceOnOffCount == std::size(kFzLedvanceOnOff));

const ::zhc::TzConverter* const kTzLedvanceOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzLedvanceOnOffCount == std::size(kTzLedvanceOnOff));

const ::zhc::Expose kExposesLedvanceOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesLedvanceOnOffCount == std::size(kExposesLedvanceOnOff));

const ::zhc::BindingSpec kBindingsLedvanceOnOff[] = {
    { 1, 0x0006 },
};
static_assert(kBindingsLedvanceOnOffCount == std::size(kBindingsLedvanceOnOff));

// ── On/off + electricity meter (ledvanceOnOff + m.electricityMeter) ──
const ::zhc::FzConverter* const kFzLedvanceOnOffEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
static_assert(kFzLedvanceOnOffEMCount == std::size(kFzLedvanceOnOffEM));

const ::zhc::TzConverter* const kTzLedvanceOnOffEM[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzLedvanceOnOffEMCount == std::size(kTzLedvanceOnOffEM));

const ::zhc::Expose kExposesLedvanceOnOffEM[] = {
    { "state",   ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "energy",  ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "kWh", nullptr, nullptr, 0 },
    { "power",   ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "W",   nullptr, nullptr, 0 },
    { "voltage", ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "V",   nullptr, nullptr, 0 },
    { "current", ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "A",   nullptr, nullptr, 0 },
};
static_assert(kExposesLedvanceOnOffEMCount == std::size(kExposesLedvanceOnOffEM));

const ::zhc::BindingSpec kBindingsLedvanceOnOffEM[] = {
    { 1, 0x0006 }, { 1, 0x0702 }, { 1, 0x0B04 },
};
static_assert(kBindingsLedvanceOnOffEMCount == std::size(kBindingsLedvanceOnOffEM));

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
const ::zhc::FzConverter* const kFzLedvanceDim[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzLedvanceDimCount == std::size(kFzLedvanceDim));

const ::zhc::TzConverter* const kTzLedvanceDim[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzLedvanceDimCount == std::size(kTzLedvanceDim));

const ::zhc::Expose kExposesLedvanceDim[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesLedvanceDimCount == std::size(kExposesLedvanceDim));

const ::zhc::BindingSpec kBindingsLedvanceDim[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsLedvanceDimCount == std::size(kBindingsLedvanceDim));

// ── Dimmable + electricity meter (m.light + m.electricityMeter) ──────
const ::zhc::FzConverter* const kFzLedvanceDimEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
static_assert(kFzLedvanceDimEMCount == std::size(kFzLedvanceDimEM));

const ::zhc::TzConverter* const kTzLedvanceDimEM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzLedvanceDimEMCount == std::size(kTzLedvanceDimEM));

const ::zhc::Expose kExposesLedvanceDimEM[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "energy",     ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "kWh", nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "W",   nullptr, nullptr, 0 },
    { "voltage",    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "V",   nullptr, nullptr, 0 },
    { "current",    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "A",   nullptr, nullptr, 0 },
};
static_assert(kExposesLedvanceDimEMCount == std::size(kExposesLedvanceDimEM));

const ::zhc::BindingSpec kBindingsLedvanceDimEM[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0702 }, { 1, 0x0B04 },
};
static_assert(kBindingsLedvanceDimEMCount == std::size(kBindingsLedvanceDimEM));

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
const ::zhc::FzConverter* const kFzLedvanceLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzLedvanceLightCount == std::size(kFzLedvanceLight));

const ::zhc::TzConverter* const kTzLedvanceLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzLedvanceLightCount == std::size(kTzLedvanceLight));

const ::zhc::Expose kExposesLedvanceLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesLedvanceLightCount == std::size(kExposesLedvanceLight));

const ::zhc::BindingSpec kBindingsLedvanceLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsLedvanceLightCount == std::size(kBindingsLedvanceLight));

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzLedvanceColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzLedvanceColorCTLightCount == std::size(kFzLedvanceColorCTLight));

const ::zhc::TzConverter* const kTzLedvanceColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzLedvanceColorCTLightCount == std::size(kTzLedvanceColorCTLight));

const ::zhc::Expose kExposesLedvanceColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesLedvanceColorCTLightCount == std::size(kExposesLedvanceColorCTLight));

const ::zhc::BindingSpec kBindingsLedvanceColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsLedvanceColorCTLightCount == std::size(kBindingsLedvanceColorCTLight));

}  // namespace zhc::devices::ledvance
