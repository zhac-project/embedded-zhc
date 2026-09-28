// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for AwoX bulb ports — one per `m.light()` feature
// combination seen in z2m's awox.ts. Modelled after osram/_shared.
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {

// ── Plain dimmable (m.light({})) ────────────────────────────────────
const ::zhc::FzConverter* const kFzAwoxLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzAwoxLightCount == std::size(kFzAwoxLight));

const ::zhc::TzConverter* const kTzAwoxLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzAwoxLightCount == std::size(kTzAwoxLight));

const ::zhc::Expose kExposesAwoxLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesAwoxLightCount == std::size(kExposesAwoxLight));

const ::zhc::BindingSpec kBindingsAwoxLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsAwoxLightCount == std::size(kBindingsAwoxLight));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzAwoxCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzAwoxCTLightCount == std::size(kFzAwoxCTLight));

const ::zhc::TzConverter* const kTzAwoxCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzAwoxCTLightCount == std::size(kTzAwoxCTLight));

const ::zhc::Expose kExposesAwoxCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesAwoxCTLightCount == std::size(kExposesAwoxCTLight));

const ::zhc::BindingSpec kBindingsAwoxCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsAwoxCTLightCount == std::size(kBindingsAwoxCTLight));

// ── Full RGBW (m.light({colorTemp, color: {modes:["xy","hs"]}})) ────
const ::zhc::FzConverter* const kFzAwoxColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzAwoxColorCTLightCount == std::size(kFzAwoxColorCTLight));

const ::zhc::TzConverter* const kTzAwoxColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzAwoxColorCTLightCount == std::size(kTzAwoxColorCTLight));

const ::zhc::Expose kExposesAwoxColorCTLight[] = {
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
static_assert(kExposesAwoxColorCTLightCount == std::size(kExposesAwoxColorCTLight));

const ::zhc::BindingSpec kBindingsAwoxColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsAwoxColorCTLightCount == std::size(kBindingsAwoxColorCTLight));

}  // namespace zhc::devices::awox
