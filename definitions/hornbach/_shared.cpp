// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Two shared bundles for HORNBACH (FLAIR Viyu) light ports — one per
// `m.light(...)` feature combination seen in z2m's hornbach.ts. Modelled
// after definitions/paulmann/_shared.cpp. Hornbach ships no vendor cluster
// on its lighting line, so every bundle is plain generic ZCL.
//
// Bug fixed: the auto-generator wired every def with only kFzOnOff +
// kFzBrightness (clusters 0x0006/0x0008), dropping the lightingColorCtrl
// (0x0300) axis entirely. z2m gives every hornbach bulb colorTemp, and
// six of them (the *_RGBW_* / "RGB" variants) full hue/sat colour as well.
#include "definitions/hornbach/_shared.hpp"

namespace zhc::devices::hornbach {

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzHornbachCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzHornbachCTLightCount == std::size(kFzHornbachCTLight));

const ::zhc::TzConverter* const kTzHornbachCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzHornbachCTLightCount == std::size(kTzHornbachCTLight));

const ::zhc::Expose kExposesHornbachCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired",  nullptr, nullptr, 0 },
};
static_assert(kExposesHornbachCTLightCount == std::size(kExposesHornbachCTLight));

const ::zhc::BindingSpec kBindingsHornbachCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsHornbachCTLightCount == std::size(kBindingsHornbachCTLight));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzHornbachColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzHornbachColorCTLightCount == std::size(kFzHornbachColorCTLight));

const ::zhc::TzConverter* const kTzHornbachColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzHornbachColorCTLightCount == std::size(kTzHornbachColorCTLight));

const ::zhc::Expose kExposesHornbachColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired",  nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
};
static_assert(kExposesHornbachColorCTLightCount == std::size(kExposesHornbachColorCTLight));

const ::zhc::BindingSpec kBindingsHornbachColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsHornbachColorCTLightCount == std::size(kBindingsHornbachColorCTLight));

}  // namespace zhc::devices::hornbach
