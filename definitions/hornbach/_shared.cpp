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
const std::uint8_t kFzHornbachCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzHornbachCTLight) / sizeof(kFzHornbachCTLight[0]));

const ::zhc::TzConverter* const kTzHornbachCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzHornbachCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzHornbachCTLight) / sizeof(kTzHornbachCTLight[0]));

const ::zhc::Expose kExposesHornbachCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired",  nullptr, nullptr, 0 },
};
const std::uint8_t kExposesHornbachCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesHornbachCTLight) / sizeof(kExposesHornbachCTLight[0]));

const ::zhc::BindingSpec kBindingsHornbachCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsHornbachCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsHornbachCTLight) / sizeof(kBindingsHornbachCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzHornbachColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzHornbachColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzHornbachColorCTLight) / sizeof(kFzHornbachColorCTLight[0]));

const ::zhc::TzConverter* const kTzHornbachColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzHornbachColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzHornbachColorCTLight) / sizeof(kTzHornbachColorCTLight[0]));

const ::zhc::Expose kExposesHornbachColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired",  nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
};
const std::uint8_t kExposesHornbachColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesHornbachColorCTLight) / sizeof(kExposesHornbachColorCTLight[0]));

const ::zhc::BindingSpec kBindingsHornbachColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsHornbachColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsHornbachColorCTLight) / sizeof(kBindingsHornbachColorCTLight[0]));

}  // namespace zhc::devices::hornbach
