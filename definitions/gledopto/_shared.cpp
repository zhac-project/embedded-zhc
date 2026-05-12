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
const std::uint8_t kFzGledoptoLightCount =
    static_cast<std::uint8_t>(sizeof(kFzGledoptoLight) / sizeof(kFzGledoptoLight[0]));

const ::zhc::TzConverter* const kTzGledoptoLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzGledoptoLightCount =
    static_cast<std::uint8_t>(sizeof(kTzGledoptoLight) / sizeof(kTzGledoptoLight[0]));

const ::zhc::Expose kExposesGledoptoLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesGledoptoLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesGledoptoLight) / sizeof(kExposesGledoptoLight[0]));

const ::zhc::BindingSpec kBindingsGledoptoLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsGledoptoLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsGledoptoLight) / sizeof(kBindingsGledoptoLight[0]));

// ── CCT (gledoptoLight({colorTemp})) ────────────────────────────────
const ::zhc::FzConverter* const kFzGledoptoCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzGledoptoCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzGledoptoCTLight) / sizeof(kFzGledoptoCTLight[0]));

const ::zhc::TzConverter* const kTzGledoptoCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzGledoptoCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzGledoptoCTLight) / sizeof(kTzGledoptoCTLight[0]));

const ::zhc::Expose kExposesGledoptoCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesGledoptoCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesGledoptoCTLight) / sizeof(kExposesGledoptoCTLight[0]));

const ::zhc::BindingSpec kBindingsGledoptoCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsGledoptoCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsGledoptoCTLight) / sizeof(kBindingsGledoptoCTLight[0]));

// ── Colour-only (gledoptoLight({color: true})) ──────────────────────
const ::zhc::FzConverter* const kFzGledoptoColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzGledoptoColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzGledoptoColorLight) / sizeof(kFzGledoptoColorLight[0]));

const ::zhc::TzConverter* const kTzGledoptoColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzGledoptoColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzGledoptoColorLight) / sizeof(kTzGledoptoColorLight[0]));

const ::zhc::Expose kExposesGledoptoColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesGledoptoColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesGledoptoColorLight) / sizeof(kExposesGledoptoColorLight[0]));

const ::zhc::BindingSpec kBindingsGledoptoColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsGledoptoColorLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsGledoptoColorLight) / sizeof(kBindingsGledoptoColorLight[0]));

// ── Full RGBW (gledoptoLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzGledoptoColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzGledoptoColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzGledoptoColorCTLight) / sizeof(kFzGledoptoColorCTLight[0]));

const ::zhc::TzConverter* const kTzGledoptoColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzGledoptoColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzGledoptoColorCTLight) / sizeof(kTzGledoptoColorCTLight[0]));

const ::zhc::Expose kExposesGledoptoColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesGledoptoColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesGledoptoColorCTLight) / sizeof(kExposesGledoptoColorCTLight[0]));

const ::zhc::BindingSpec kBindingsGledoptoColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsGledoptoColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsGledoptoColorCTLight) / sizeof(kBindingsGledoptoColorCTLight[0]));

}  // namespace zhc::devices::gledopto
