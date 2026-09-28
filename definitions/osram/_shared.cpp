// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Osram ports — one per ledvanceLight()/ledvanceOnOff()
// feature combination seen in z2m's osram.ts. Modelled after gledopto/_shared.
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {

// ── On/off only (ledvanceOnOff) ─────────────────────────────────────
const ::zhc::FzConverter* const kFzOsramOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
static_assert(kFzOsramOnOffCount == std::size(kFzOsramOnOff));

const ::zhc::TzConverter* const kTzOsramOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzOsramOnOffCount == std::size(kTzOsramOnOff));

const ::zhc::Expose kExposesOsramOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesOsramOnOffCount == std::size(kExposesOsramOnOff));

const ::zhc::BindingSpec kBindingsOsramOnOff[] = {
    { 1, 0x0006 },
};
static_assert(kBindingsOsramOnOffCount == std::size(kBindingsOsramOnOff));

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
const ::zhc::FzConverter* const kFzOsramLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzOsramLightCount == std::size(kFzOsramLight));

const ::zhc::TzConverter* const kTzOsramLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzOsramLightCount == std::size(kTzOsramLight));

const ::zhc::Expose kExposesOsramLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesOsramLightCount == std::size(kExposesOsramLight));

const ::zhc::BindingSpec kBindingsOsramLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsOsramLightCount == std::size(kBindingsOsramLight));

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
const ::zhc::FzConverter* const kFzOsramCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzOsramCTLightCount == std::size(kFzOsramCTLight));

const ::zhc::TzConverter* const kTzOsramCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzOsramCTLightCount == std::size(kTzOsramCTLight));

const ::zhc::Expose kExposesOsramCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesOsramCTLightCount == std::size(kExposesOsramCTLight));

const ::zhc::BindingSpec kBindingsOsramCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsOsramCTLightCount == std::size(kBindingsOsramCTLight));

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
const ::zhc::FzConverter* const kFzOsramColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzOsramColorLightCount == std::size(kFzOsramColorLight));

const ::zhc::TzConverter* const kTzOsramColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzOsramColorLightCount == std::size(kTzOsramColorLight));

const ::zhc::Expose kExposesOsramColorLight[] = {
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
static_assert(kExposesOsramColorLightCount == std::size(kExposesOsramColorLight));

const ::zhc::BindingSpec kBindingsOsramColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsOsramColorLightCount == std::size(kBindingsOsramColorLight));

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzOsramColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzOsramColorCTLightCount == std::size(kFzOsramColorCTLight));

const ::zhc::TzConverter* const kTzOsramColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzOsramColorCTLightCount == std::size(kTzOsramColorCTLight));

const ::zhc::Expose kExposesOsramColorCTLight[] = {
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
static_assert(kExposesOsramColorCTLightCount == std::size(kExposesOsramColorCTLight));

const ::zhc::BindingSpec kBindingsOsramColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsOsramColorCTLightCount == std::size(kBindingsOsramColorCTLight));

}  // namespace zhc::devices::osram
