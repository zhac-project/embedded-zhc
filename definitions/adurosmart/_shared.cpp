// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Six shared bundles for AduroSmart ports — one per m.light()/m.onOff()/
// m.electricityMeter() feature combination seen in z2m's adurosmart.ts.
// Modelled after ledvance/_shared.

#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
const ::zhc::FzConverter* const kFzAduOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
static_assert(kFzAduOnOffCount == std::size(kFzAduOnOff));

const ::zhc::TzConverter* const kTzAduOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzAduOnOffCount == std::size(kTzAduOnOff));

const ::zhc::Expose kExposesAduOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesAduOnOffCount == std::size(kExposesAduOnOff));

const ::zhc::BindingSpec kBindingsAduOnOff[] = {
    { 1, 0x0006 },
};
static_assert(kBindingsAduOnOffCount == std::size(kBindingsAduOnOff));

// ── On/off + electrical-only meter (m.onOff() + m.electricityMeter
//    ({cluster:"electrical"})) ─────────────────────────────────────
// z2m's `cluster:"electrical"` reads ONLY haElectricalMeasurement
// (0x0B04) → power/voltage/current. It does NOT read seMetering
// (0x0702) and exposes NO `energy` (genericMeter electrical branch,
// modernExtend.ts L2296-2322). 81848 additionally uses the legacy
// fz.electrical_measurement (same 0x0B04 surface). So no kFzMetering,
// no `energy` expose, no 0x0702 binding.
const ::zhc::FzConverter* const kFzAduOnOffEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
};
static_assert(kFzAduOnOffEMCount == std::size(kFzAduOnOffEM));

const ::zhc::TzConverter* const kTzAduOnOffEM[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzAduOnOffEMCount == std::size(kTzAduOnOffEM));

const ::zhc::Expose kExposesAduOnOffEM[] = {
    { "state",   ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "power",   ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "W",   nullptr, nullptr, 0 },
    { "voltage", ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "V",   nullptr, nullptr, 0 },
    { "current", ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "A",   nullptr, nullptr, 0 },
};
static_assert(kExposesAduOnOffEMCount == std::size(kExposesAduOnOffEM));

const ::zhc::BindingSpec kBindingsAduOnOffEM[] = {
    { 1, 0x0006 }, { 1, 0x0B04 },
};
static_assert(kBindingsAduOnOffEMCount == std::size(kBindingsAduOnOffEM));

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzAduLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzAduLightCount == std::size(kFzAduLight));

const ::zhc::TzConverter* const kTzAduLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzAduLightCount == std::size(kTzAduLight));

const ::zhc::Expose kExposesAduLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesAduLightCount == std::size(kExposesAduLight));

const ::zhc::BindingSpec kBindingsAduLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsAduLightCount == std::size(kBindingsAduLight));

// ── Dimmable + electrical-only meter (m.light() + m.electricityMeter
//    ({cluster:"electrical"})) ─────────────────────────────────────
// Same electrical-only surface as kFzAduOnOffEM: 0x0B04 power/voltage/
// current, NO seMetering 0x0702, NO `energy`.
const ::zhc::FzConverter* const kFzAduLightEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzElectricalMeasurement,
};
static_assert(kFzAduLightEMCount == std::size(kFzAduLightEM));

const ::zhc::TzConverter* const kTzAduLightEM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzAduLightEMCount == std::size(kTzAduLightEM));

const ::zhc::Expose kExposesAduLightEM[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "W",   nullptr, nullptr, 0 },
    { "voltage",    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "V",   nullptr, nullptr, 0 },
    { "current",    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "A",   nullptr, nullptr, 0 },
};
static_assert(kExposesAduLightEMCount == std::size(kExposesAduLightEM));

const ::zhc::BindingSpec kBindingsAduLightEM[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0B04 },
};
static_assert(kBindingsAduLightEMCount == std::size(kBindingsAduLightEM));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzAduCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzAduCTLightCount == std::size(kFzAduCTLight));

const ::zhc::TzConverter* const kTzAduCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzAduCTLightCount == std::size(kTzAduCTLight));

const ::zhc::Expose kExposesAduCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesAduCTLightCount == std::size(kExposesAduCTLight));

const ::zhc::BindingSpec kBindingsAduCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsAduCTLightCount == std::size(kBindingsAduCTLight));

// ── Full RGBCCT (m.light({colorTemp, color})) ───────────────────────
const ::zhc::FzConverter* const kFzAduColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzAduColorCTLightCount == std::size(kFzAduColorCTLight));

const ::zhc::TzConverter* const kTzAduColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzAduColorCTLightCount == std::size(kTzAduColorCTLight));

const ::zhc::Expose kExposesAduColorCTLight[] = {
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
static_assert(kExposesAduColorCTLightCount == std::size(kExposesAduColorCTLight));

const ::zhc::BindingSpec kBindingsAduColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsAduColorCTLightCount == std::size(kBindingsAduColorCTLight));

}  // namespace zhc::devices::adurosmart
