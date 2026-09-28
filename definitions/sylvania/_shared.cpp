// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Sylvania ports — one per
// ledvanceLight()/ledvanceOnOff() feature combination seen in z2m's
// sylvania.ts. Modelled after osram/_shared / ledvance/_shared.
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {

// ── On/off only (ledvanceOnOff) ─────────────────────────────────────
const ::zhc::FzConverter* const kFzSylvaniaOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
static_assert(kFzSylvaniaOnOffCount == std::size(kFzSylvaniaOnOff));

const ::zhc::TzConverter* const kTzSylvaniaOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzSylvaniaOnOffCount == std::size(kTzSylvaniaOnOff));

const ::zhc::Expose kExposesSylvaniaOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesSylvaniaOnOffCount == std::size(kExposesSylvaniaOnOff));

const ::zhc::BindingSpec kBindingsSylvaniaOnOff[] = {
    { 1, 0x0006 },
};
static_assert(kBindingsSylvaniaOnOffCount == std::size(kBindingsSylvaniaOnOff));

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
const ::zhc::FzConverter* const kFzSylvaniaDim[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzSylvaniaDimCount == std::size(kFzSylvaniaDim));

const ::zhc::TzConverter* const kTzSylvaniaDim[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzSylvaniaDimCount == std::size(kTzSylvaniaDim));

const ::zhc::Expose kExposesSylvaniaDim[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesSylvaniaDimCount == std::size(kExposesSylvaniaDim));

const ::zhc::BindingSpec kBindingsSylvaniaDim[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsSylvaniaDimCount == std::size(kBindingsSylvaniaDim));

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
const ::zhc::FzConverter* const kFzSylvaniaLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzSylvaniaLightCount == std::size(kFzSylvaniaLight));

const ::zhc::TzConverter* const kTzSylvaniaLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzSylvaniaLightCount == std::size(kTzSylvaniaLight));

const ::zhc::Expose kExposesSylvaniaLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesSylvaniaLightCount == std::size(kExposesSylvaniaLight));

const ::zhc::BindingSpec kBindingsSylvaniaLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsSylvaniaLightCount == std::size(kBindingsSylvaniaLight));

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
const ::zhc::FzConverter* const kFzSylvaniaColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzSylvaniaColorLightCount == std::size(kFzSylvaniaColorLight));

const ::zhc::TzConverter* const kTzSylvaniaColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzSylvaniaColorLightCount == std::size(kTzSylvaniaColorLight));

const ::zhc::Expose kExposesSylvaniaColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesSylvaniaColorLightCount == std::size(kExposesSylvaniaColorLight));

const ::zhc::BindingSpec kBindingsSylvaniaColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsSylvaniaColorLightCount == std::size(kBindingsSylvaniaColorLight));

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzSylvaniaColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzSylvaniaColorCTLightCount == std::size(kFzSylvaniaColorCTLight));

const ::zhc::TzConverter* const kTzSylvaniaColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzSylvaniaColorCTLightCount == std::size(kTzSylvaniaColorCTLight));

const ::zhc::Expose kExposesSylvaniaColorCTLight[] = {
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
static_assert(kExposesSylvaniaColorCTLightCount == std::size(kExposesSylvaniaColorCTLight));

const ::zhc::BindingSpec kBindingsSylvaniaColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsSylvaniaColorCTLightCount == std::size(kBindingsSylvaniaColorCTLight));

}  // namespace zhc::devices::sylvania
