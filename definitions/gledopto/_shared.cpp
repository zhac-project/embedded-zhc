// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Four shared bundles for Gledopto ports — one per gledoptoLight() feature
// combination seen in z2m's gledopto.ts. Modelled after Philips' four-tier
// split (definitions/philips/_shared.cpp).
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzGledoptoLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzGledoptoLightCount == std::size(kFzGledoptoLight));

const ::zhc::TzConverter* const kTzGledoptoLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzGledoptoLightCount == std::size(kTzGledoptoLight));

const ::zhc::Expose kExposesGledoptoLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesGledoptoLightCount == std::size(kExposesGledoptoLight));

const ::zhc::BindingSpec kBindingsGledoptoLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsGledoptoLightCount == std::size(kBindingsGledoptoLight));

// ── CCT (gledoptoLight({colorTemp})) ────────────────────────────────
const ::zhc::FzConverter* const kFzGledoptoCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzGledoptoCTLightCount == std::size(kFzGledoptoCTLight));

const ::zhc::TzConverter* const kTzGledoptoCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzGledoptoCTLightCount == std::size(kTzGledoptoCTLight));

const ::zhc::Expose kExposesGledoptoCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesGledoptoCTLightCount == std::size(kExposesGledoptoCTLight));

const ::zhc::BindingSpec kBindingsGledoptoCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsGledoptoCTLightCount == std::size(kBindingsGledoptoCTLight));

// ── Colour-only (gledoptoLight({color: true})) ──────────────────────
const ::zhc::FzConverter* const kFzGledoptoColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzGledoptoColorLightCount == std::size(kFzGledoptoColorLight));

const ::zhc::TzConverter* const kTzGledoptoColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzGledoptoColorLightCount == std::size(kTzGledoptoColorLight));

const ::zhc::Expose kExposesGledoptoColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesGledoptoColorLightCount == std::size(kExposesGledoptoColorLight));

const ::zhc::BindingSpec kBindingsGledoptoColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsGledoptoColorLightCount == std::size(kBindingsGledoptoColorLight));

// ── Full RGBW (gledoptoLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzGledoptoColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzGledoptoColorCTLightCount == std::size(kFzGledoptoColorCTLight));

const ::zhc::TzConverter* const kTzGledoptoColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzGledoptoColorCTLightCount == std::size(kTzGledoptoColorCTLight));

const ::zhc::Expose kExposesGledoptoColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesGledoptoColorCTLightCount == std::size(kExposesGledoptoColorCTLight));

const ::zhc::BindingSpec kBindingsGledoptoColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsGledoptoColorCTLightCount == std::size(kBindingsGledoptoColorCTLight));

}  // namespace zhc::devices::gledopto
