// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Four shared bundles for Paulmann light ports — one per `m.light(...)`
// feature combination seen in z2m's paulmann.ts. Modelled after
// `definitions/innr/_shared.cpp`. Paulmann ships no vendor-specific
// cluster on its lighting line, so every bundle is plain generic ZCL.
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzPaulmannLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzPaulmannLightCount == std::size(kFzPaulmannLight));

const ::zhc::TzConverter* const kTzPaulmannLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzPaulmannLightCount == std::size(kTzPaulmannLight));

const ::zhc::Expose kExposesPaulmannLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPaulmannLightCount == std::size(kExposesPaulmannLight));

const ::zhc::BindingSpec kBindingsPaulmannLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsPaulmannLightCount == std::size(kBindingsPaulmannLight));

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzPaulmannCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzPaulmannCTLightCount == std::size(kFzPaulmannCTLight));

const ::zhc::TzConverter* const kTzPaulmannCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzPaulmannCTLightCount == std::size(kTzPaulmannCTLight));

const ::zhc::Expose kExposesPaulmannCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesPaulmannCTLightCount == std::size(kExposesPaulmannCTLight));

const ::zhc::BindingSpec kBindingsPaulmannCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsPaulmannCTLightCount == std::size(kBindingsPaulmannCTLight));

// ── Colour-only (m.light({color})) ──────────────────────────────────
const ::zhc::FzConverter* const kFzPaulmannColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzPaulmannColorLightCount == std::size(kFzPaulmannColorLight));

const ::zhc::TzConverter* const kTzPaulmannColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzPaulmannColorLightCount == std::size(kTzPaulmannColorLight));

const ::zhc::Expose kExposesPaulmannColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPaulmannColorLightCount == std::size(kExposesPaulmannColorLight));

const ::zhc::BindingSpec kBindingsPaulmannColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsPaulmannColorLightCount == std::size(kBindingsPaulmannColorLight));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzPaulmannColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzPaulmannColorCTLightCount == std::size(kFzPaulmannColorCTLight));

const ::zhc::TzConverter* const kTzPaulmannColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzPaulmannColorCTLightCount == std::size(kTzPaulmannColorCTLight));

const ::zhc::Expose kExposesPaulmannColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPaulmannColorCTLightCount == std::size(kExposesPaulmannColorCTLight));

const ::zhc::BindingSpec kBindingsPaulmannColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsPaulmannColorCTLightCount == std::size(kBindingsPaulmannColorCTLight));

}  // namespace zhc::devices::paulmann
