// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Müller-Licht (tint) ports — one per
// mullerLichtLight()/m.onOff() feature combination seen in muller_licht.ts.
// Modelled after osram/_shared.
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
const ::zhc::FzConverter* const kFzMullerLichtOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
static_assert(kFzMullerLichtOnOffCount == std::size(kFzMullerLichtOnOff));

const ::zhc::TzConverter* const kTzMullerLichtOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzMullerLichtOnOffCount == std::size(kTzMullerLichtOnOff));

const ::zhc::Expose kExposesMullerLichtOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesMullerLichtOnOffCount == std::size(kExposesMullerLichtOnOff));

const ::zhc::BindingSpec kBindingsMullerLichtOnOff[] = {
    { 1, 0x0006 },
};
static_assert(kBindingsMullerLichtOnOffCount == std::size(kBindingsMullerLichtOnOff));

// ── Dimmable (m.light({})) ──────────────────────────────────────────
const ::zhc::FzConverter* const kFzMullerLichtLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzMullerLichtLightCount == std::size(kFzMullerLichtLight));

const ::zhc::TzConverter* const kTzMullerLichtLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzMullerLichtLightCount == std::size(kTzMullerLichtLight));

const ::zhc::Expose kExposesMullerLichtLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesMullerLichtLightCount == std::size(kExposesMullerLichtLight));

const ::zhc::BindingSpec kBindingsMullerLichtLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsMullerLichtLightCount == std::size(kBindingsMullerLichtLight));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzMullerLichtCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzMullerLichtCTLightCount == std::size(kFzMullerLichtCTLight));

const ::zhc::TzConverter* const kTzMullerLichtCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzMullerLichtCTLightCount == std::size(kTzMullerLichtCTLight));

const ::zhc::Expose kExposesMullerLichtCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired",  nullptr, nullptr, 0 },
};
static_assert(kExposesMullerLichtCTLightCount == std::size(kExposesMullerLichtCTLight));

const ::zhc::BindingSpec kBindingsMullerLichtCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsMullerLichtCTLightCount == std::size(kBindingsMullerLichtCTLight));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzMullerLichtColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzMullerLichtColorCTLightCount == std::size(kFzMullerLichtColorCTLight));

const ::zhc::TzConverter* const kTzMullerLichtColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzMullerLichtColorCTLightCount == std::size(kTzMullerLichtColorCTLight));

const ::zhc::Expose kExposesMullerLichtColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired",  nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesMullerLichtColorCTLightCount == std::size(kExposesMullerLichtColorCTLight));

const ::zhc::BindingSpec kBindingsMullerLichtColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsMullerLichtColorCTLightCount == std::size(kBindingsMullerLichtColorCTLight));

}  // namespace zhc::devices::muller_licht
