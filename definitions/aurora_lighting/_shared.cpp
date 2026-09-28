// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Aurora-AOne bulb ports — one per m.light()
// feature combination seen in z2m's aurora_lighting.ts. Modelled after
// gledopto/_shared.cpp.
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzAuroraLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzAuroraLightCount == std::size(kFzAuroraLight));

const ::zhc::TzConverter* const kTzAuroraLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzAuroraLightCount == std::size(kTzAuroraLight));

const ::zhc::Expose kExposesAuroraLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesAuroraLightCount == std::size(kExposesAuroraLight));

const ::zhc::BindingSpec kBindingsAuroraLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsAuroraLightCount == std::size(kBindingsAuroraLight));

// ── CCT (m.light({colorTemp: ...})) ─────────────────────────────────
const ::zhc::FzConverter* const kFzAuroraCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzAuroraCTLightCount == std::size(kFzAuroraCTLight));

const ::zhc::TzConverter* const kTzAuroraCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzAuroraCTLightCount == std::size(kTzAuroraCTLight));

const ::zhc::Expose kExposesAuroraCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesAuroraCTLightCount == std::size(kExposesAuroraCTLight));

const ::zhc::BindingSpec kBindingsAuroraCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsAuroraCTLightCount == std::size(kBindingsAuroraCTLight));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzAuroraColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzAuroraColorCTLightCount == std::size(kFzAuroraColorCTLight));

const ::zhc::TzConverter* const kTzAuroraColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzAuroraColorCTLightCount == std::size(kTzAuroraColorCTLight));

const ::zhc::Expose kExposesAuroraColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesAuroraColorCTLightCount == std::size(kExposesAuroraColorCTLight));

const ::zhc::BindingSpec kBindingsAuroraColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsAuroraColorCTLightCount == std::size(kBindingsAuroraColorCTLight));

}  // namespace zhc::devices::aurora_lighting
