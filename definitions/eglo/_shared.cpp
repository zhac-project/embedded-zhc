// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Two shared bundles for EGLO connect bulb ports — one per `m.light()`
// feature combination seen in z2m's eglo.ts. Modelled after awox/_shared.
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzEgloCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzEgloCTLightCount == std::size(kFzEgloCTLight));

const ::zhc::TzConverter* const kTzEgloCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzEgloCTLightCount == std::size(kTzEgloCTLight));

const ::zhc::Expose kExposesEgloCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesEgloCTLightCount == std::size(kExposesEgloCTLight));

const ::zhc::BindingSpec kBindingsEgloCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsEgloCTLightCount == std::size(kBindingsEgloCTLight));

// ── Full RGBW (m.light({colorTemp, color:{modes:["xy","hs"]}})) ─────
const ::zhc::FzConverter* const kFzEgloColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzEgloColorCTLightCount == std::size(kFzEgloColorCTLight));

const ::zhc::TzConverter* const kTzEgloColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzEgloColorCTLightCount == std::size(kTzEgloColorCTLight));

const ::zhc::Expose kExposesEgloColorCTLight[] = {
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
static_assert(kExposesEgloColorCTLightCount == std::size(kExposesEgloColorCTLight));

const ::zhc::BindingSpec kBindingsEgloColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsEgloColorCTLightCount == std::size(kBindingsEgloColorCTLight));

}  // namespace zhc::devices::eglo
