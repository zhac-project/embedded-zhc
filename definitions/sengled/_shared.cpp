// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {

// ── Plain dimmable (sengledLight()) ─────────────────────────────────
const ::zhc::FzConverter* const kFzSengledLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzSengledLightCount == std::size(kFzSengledLight));

const ::zhc::TzConverter* const kTzSengledLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzSengledLightCount == std::size(kTzSengledLight));

const ::zhc::Expose kExposesSengledLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesSengledLightCount == std::size(kExposesSengledLight));

const ::zhc::BindingSpec kBindingsSengledLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsSengledLightCount == std::size(kBindingsSengledLight));

// ── Tunable white (sengledLight({colorTemp})) ───────────────────────
const ::zhc::FzConverter* const kFzSengledCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzSengledCTLightCount == std::size(kFzSengledCTLight));

const ::zhc::TzConverter* const kTzSengledCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzSengledCTLightCount == std::size(kTzSengledCTLight));

const ::zhc::Expose kExposesSengledCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesSengledCTLightCount == std::size(kExposesSengledCTLight));

const ::zhc::BindingSpec kBindingsSengledCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsSengledCTLightCount == std::size(kBindingsSengledCTLight));

// ── Colour-only (sengledLight({colorTemp: undefined, color: ...})) ──
const ::zhc::FzConverter* const kFzSengledColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzSengledColorLightCount == std::size(kFzSengledColorLight));

const ::zhc::TzConverter* const kTzSengledColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzSengledColorLightCount == std::size(kTzSengledColorLight));

const ::zhc::Expose kExposesSengledColorLight[] = {
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
static_assert(kExposesSengledColorLightCount == std::size(kExposesSengledColorLight));

const ::zhc::BindingSpec kBindingsSengledColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsSengledColorLightCount == std::size(kBindingsSengledColorLight));

// ── Full RGBW (sengledLight({colorTemp, color})) ────────────────────
const ::zhc::FzConverter* const kFzSengledColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzSengledColorCTLightCount == std::size(kFzSengledColorCTLight));

const ::zhc::TzConverter* const kTzSengledColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzSengledColorCTLightCount == std::size(kTzSengledColorCTLight));

const ::zhc::Expose kExposesSengledColorCTLight[] = {
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
static_assert(kExposesSengledColorCTLightCount == std::size(kExposesSengledColorCTLight));

const ::zhc::BindingSpec kBindingsSengledColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsSengledColorCTLightCount == std::size(kBindingsSengledColorCTLight));

}  // namespace zhc::devices::sengled
