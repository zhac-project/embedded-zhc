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
const std::uint8_t kFzLedvanceOnOffCount =
    static_cast<std::uint8_t>(sizeof(kFzLedvanceOnOff) / sizeof(kFzLedvanceOnOff[0]));

const ::zhc::TzConverter* const kTzLedvanceOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzLedvanceOnOffCount =
    static_cast<std::uint8_t>(sizeof(kTzLedvanceOnOff) / sizeof(kTzLedvanceOnOff[0]));

const ::zhc::Expose kExposesLedvanceOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLedvanceOnOffCount =
    static_cast<std::uint8_t>(sizeof(kExposesLedvanceOnOff) / sizeof(kExposesLedvanceOnOff[0]));

const ::zhc::BindingSpec kBindingsLedvanceOnOff[] = {
    { 1, 0x0006 },
};
const std::uint8_t kBindingsLedvanceOnOffCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLedvanceOnOff) / sizeof(kBindingsLedvanceOnOff[0]));

// ── On/off + electricity meter (ledvanceOnOff + m.electricityMeter) ──
const ::zhc::FzConverter* const kFzLedvanceOnOffEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const std::uint8_t kFzLedvanceOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kFzLedvanceOnOffEM) / sizeof(kFzLedvanceOnOffEM[0]));

const ::zhc::TzConverter* const kTzLedvanceOnOffEM[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzLedvanceOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kTzLedvanceOnOffEM) / sizeof(kTzLedvanceOnOffEM[0]));

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
const std::uint8_t kExposesLedvanceOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kExposesLedvanceOnOffEM) / sizeof(kExposesLedvanceOnOffEM[0]));

const ::zhc::BindingSpec kBindingsLedvanceOnOffEM[] = {
    { 1, 0x0006 }, { 1, 0x0702 }, { 1, 0x0B04 },
};
const std::uint8_t kBindingsLedvanceOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLedvanceOnOffEM) / sizeof(kBindingsLedvanceOnOffEM[0]));

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
const ::zhc::FzConverter* const kFzLedvanceDim[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzLedvanceDimCount =
    static_cast<std::uint8_t>(sizeof(kFzLedvanceDim) / sizeof(kFzLedvanceDim[0]));

const ::zhc::TzConverter* const kTzLedvanceDim[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzLedvanceDimCount =
    static_cast<std::uint8_t>(sizeof(kTzLedvanceDim) / sizeof(kTzLedvanceDim[0]));

const ::zhc::Expose kExposesLedvanceDim[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLedvanceDimCount =
    static_cast<std::uint8_t>(sizeof(kExposesLedvanceDim) / sizeof(kExposesLedvanceDim[0]));

const ::zhc::BindingSpec kBindingsLedvanceDim[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsLedvanceDimCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLedvanceDim) / sizeof(kBindingsLedvanceDim[0]));

// ── Dimmable + electricity meter (m.light + m.electricityMeter) ──────
const ::zhc::FzConverter* const kFzLedvanceDimEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const std::uint8_t kFzLedvanceDimEMCount =
    static_cast<std::uint8_t>(sizeof(kFzLedvanceDimEM) / sizeof(kFzLedvanceDimEM[0]));

const ::zhc::TzConverter* const kTzLedvanceDimEM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzLedvanceDimEMCount =
    static_cast<std::uint8_t>(sizeof(kTzLedvanceDimEM) / sizeof(kTzLedvanceDimEM[0]));

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
const std::uint8_t kExposesLedvanceDimEMCount =
    static_cast<std::uint8_t>(sizeof(kExposesLedvanceDimEM) / sizeof(kExposesLedvanceDimEM[0]));

const ::zhc::BindingSpec kBindingsLedvanceDimEM[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0702 }, { 1, 0x0B04 },
};
const std::uint8_t kBindingsLedvanceDimEMCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLedvanceDimEM) / sizeof(kBindingsLedvanceDimEM[0]));

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
const ::zhc::FzConverter* const kFzLedvanceLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzLedvanceLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLedvanceLight) / sizeof(kFzLedvanceLight[0]));

const ::zhc::TzConverter* const kTzLedvanceLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzLedvanceLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLedvanceLight) / sizeof(kTzLedvanceLight[0]));

const ::zhc::Expose kExposesLedvanceLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLedvanceLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLedvanceLight) / sizeof(kExposesLedvanceLight[0]));

const ::zhc::BindingSpec kBindingsLedvanceLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsLedvanceLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLedvanceLight) / sizeof(kBindingsLedvanceLight[0]));

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzLedvanceColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzLedvanceColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLedvanceColorCTLight) / sizeof(kFzLedvanceColorCTLight[0]));

const ::zhc::TzConverter* const kTzLedvanceColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzLedvanceColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLedvanceColorCTLight) / sizeof(kTzLedvanceColorCTLight[0]));

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
const std::uint8_t kExposesLedvanceColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLedvanceColorCTLight) / sizeof(kExposesLedvanceColorCTLight[0]));

const ::zhc::BindingSpec kBindingsLedvanceColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsLedvanceColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLedvanceColorCTLight) / sizeof(kBindingsLedvanceColorCTLight[0]));

}  // namespace zhc::devices::ledvance
