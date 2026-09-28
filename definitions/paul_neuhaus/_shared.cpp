// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Paul Neuhaus light ports — one per
// `m.light(...)` feature combination seen in z2m's paul_neuhaus.ts.
// Modelled after `definitions/paulmann/_shared.cpp`. Paul Neuhaus
// ships no vendor-specific cluster on its lighting line, so every
// bundle is plain generic ZCL.
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzPaulNeuhausLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzPaulNeuhausLightCount == std::size(kFzPaulNeuhausLight));

const ::zhc::TzConverter* const kTzPaulNeuhausLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzPaulNeuhausLightCount == std::size(kTzPaulNeuhausLight));

const ::zhc::Expose kExposesPaulNeuhausLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPaulNeuhausLightCount == std::size(kExposesPaulNeuhausLight));

const ::zhc::BindingSpec kBindingsPaulNeuhausLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsPaulNeuhausLightCount == std::size(kBindingsPaulNeuhausLight));

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzPaulNeuhausCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzPaulNeuhausCTLightCount == std::size(kFzPaulNeuhausCTLight));

const ::zhc::TzConverter* const kTzPaulNeuhausCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzPaulNeuhausCTLightCount == std::size(kTzPaulNeuhausCTLight));

const ::zhc::Expose kExposesPaulNeuhausCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesPaulNeuhausCTLightCount == std::size(kExposesPaulNeuhausCTLight));

const ::zhc::BindingSpec kBindingsPaulNeuhausCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsPaulNeuhausCTLightCount == std::size(kBindingsPaulNeuhausCTLight));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzPaulNeuhausColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzPaulNeuhausColorCTLightCount == std::size(kFzPaulNeuhausColorCTLight));

const ::zhc::TzConverter* const kTzPaulNeuhausColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzPaulNeuhausColorCTLightCount == std::size(kTzPaulNeuhausColorCTLight));

const ::zhc::Expose kExposesPaulNeuhausColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesPaulNeuhausColorCTLightCount == std::size(kExposesPaulNeuhausColorCTLight));

const ::zhc::BindingSpec kBindingsPaulNeuhausColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsPaulNeuhausColorCTLightCount == std::size(kBindingsPaulNeuhausColorCTLight));

}  // namespace zhc::devices::paul_neuhaus
