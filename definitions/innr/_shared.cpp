// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Innr light ports — one per `m.light(...)`
// feature combination seen in z2m's innr.ts. Modelled after
// `definitions/gledopto/_shared.cpp` (which itself follows Philips'
// four-tier split). Innr has no colour-only-without-CT SKU, so the
// ColorLight tier is intentionally omitted.
#include "definitions/innr/_shared.hpp"

namespace zhc::devices::innr {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzInnrLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzInnrLightCount == std::size(kFzInnrLight));

const ::zhc::TzConverter* const kTzInnrLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzInnrLightCount == std::size(kTzInnrLight));

const ::zhc::Expose kExposesInnrLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesInnrLightCount == std::size(kExposesInnrLight));

const ::zhc::BindingSpec kBindingsInnrLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsInnrLightCount == std::size(kBindingsInnrLight));

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzInnrCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzInnrCTLightCount == std::size(kFzInnrCTLight));

const ::zhc::TzConverter* const kTzInnrCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzInnrCTLightCount == std::size(kTzInnrCTLight));

const ::zhc::Expose kExposesInnrCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesInnrCTLightCount == std::size(kExposesInnrCTLight));

const ::zhc::BindingSpec kBindingsInnrCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsInnrCTLightCount == std::size(kBindingsInnrCTLight));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzInnrColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzInnrColorCTLightCount == std::size(kFzInnrColorCTLight));

const ::zhc::TzConverter* const kTzInnrColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzInnrColorCTLightCount == std::size(kTzInnrColorCTLight));

const ::zhc::Expose kExposesInnrColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesInnrColorCTLightCount == std::size(kExposesInnrColorCTLight));

const ::zhc::BindingSpec kBindingsInnrColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsInnrColorCTLightCount == std::size(kBindingsInnrColorCTLight));

}  // namespace zhc::devices::innr
