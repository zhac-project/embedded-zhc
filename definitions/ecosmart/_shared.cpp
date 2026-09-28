// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Ecosmart ports — one per `m.light(...)` feature
// combination seen in z2m's `ecosmart.ts`. Modelled on
// definitions/aurora_lighting/_shared.cpp / definitions/gledopto/_shared.cpp.
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzEcosmartLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzEcosmartLightCount == std::size(kFzEcosmartLight));

const ::zhc::TzConverter* const kTzEcosmartLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzEcosmartLightCount == std::size(kTzEcosmartLight));

const ::zhc::Expose kExposesEcosmartLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesEcosmartLightCount == std::size(kExposesEcosmartLight));

const ::zhc::BindingSpec kBindingsEcosmartLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsEcosmartLightCount == std::size(kBindingsEcosmartLight));

// ── CCT (m.light({colorTemp: ...})) ─────────────────────────────────
const ::zhc::FzConverter* const kFzEcosmartCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzEcosmartCTLightCount == std::size(kFzEcosmartCTLight));

const ::zhc::TzConverter* const kTzEcosmartCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzEcosmartCTLightCount == std::size(kTzEcosmartCTLight));

const ::zhc::Expose kExposesEcosmartCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesEcosmartCTLightCount == std::size(kExposesEcosmartCTLight));

const ::zhc::BindingSpec kBindingsEcosmartCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsEcosmartCTLightCount == std::size(kBindingsEcosmartCTLight));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzEcosmartColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzEcosmartColorCTLightCount == std::size(kFzEcosmartColorCTLight));

const ::zhc::TzConverter* const kTzEcosmartColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzEcosmartColorCTLightCount == std::size(kTzEcosmartColorCTLight));

const ::zhc::Expose kExposesEcosmartColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesEcosmartColorCTLightCount == std::size(kExposesEcosmartColorCTLight));

const ::zhc::BindingSpec kBindingsEcosmartColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsEcosmartColorCTLightCount == std::size(kBindingsEcosmartColorCTLight));

}  // namespace zhc::devices::ecosmart
