// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Leedarson ports — one per `m.light(...)`
// feature combination seen in z2m's leedarson.ts. Modelled after
// osram/_shared.
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzLeedarsonLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzLeedarsonLightCount == std::size(kFzLeedarsonLight));

const ::zhc::TzConverter* const kTzLeedarsonLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzLeedarsonLightCount == std::size(kTzLeedarsonLight));

const ::zhc::Expose kExposesLeedarsonLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesLeedarsonLightCount == std::size(kExposesLeedarsonLight));

const ::zhc::BindingSpec kBindingsLeedarsonLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsLeedarsonLightCount == std::size(kBindingsLeedarsonLight));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzLeedarsonCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzLeedarsonCTLightCount == std::size(kFzLeedarsonCTLight));

const ::zhc::TzConverter* const kTzLeedarsonCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzLeedarsonCTLightCount == std::size(kTzLeedarsonCTLight));

const ::zhc::Expose kExposesLeedarsonCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesLeedarsonCTLightCount == std::size(kExposesLeedarsonCTLight));

const ::zhc::BindingSpec kBindingsLeedarsonCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsLeedarsonCTLightCount == std::size(kBindingsLeedarsonCTLight));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzLeedarsonColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzLeedarsonColorCTLightCount == std::size(kFzLeedarsonColorCTLight));

const ::zhc::TzConverter* const kTzLeedarsonColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzLeedarsonColorCTLightCount == std::size(kTzLeedarsonColorCTLight));

const ::zhc::Expose kExposesLeedarsonColorCTLight[] = {
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
static_assert(kExposesLeedarsonColorCTLightCount == std::size(kExposesLeedarsonColorCTLight));

const ::zhc::BindingSpec kBindingsLeedarsonColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsLeedarsonColorCTLightCount == std::size(kBindingsLeedarsonColorCTLight));

}  // namespace zhc::devices::leedarson
