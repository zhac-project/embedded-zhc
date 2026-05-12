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
const std::uint8_t kFzPaulmannLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulmannLight) / sizeof(kFzPaulmannLight[0]));

const ::zhc::TzConverter* const kTzPaulmannLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzPaulmannLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulmannLight) / sizeof(kTzPaulmannLight[0]));

const ::zhc::Expose kExposesPaulmannLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulmannLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulmannLight) / sizeof(kExposesPaulmannLight[0]));

const ::zhc::BindingSpec kBindingsPaulmannLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsPaulmannLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulmannLight) / sizeof(kBindingsPaulmannLight[0]));

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzPaulmannCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzPaulmannCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulmannCTLight) / sizeof(kFzPaulmannCTLight[0]));

const ::zhc::TzConverter* const kTzPaulmannCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzPaulmannCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulmannCTLight) / sizeof(kTzPaulmannCTLight[0]));

const ::zhc::Expose kExposesPaulmannCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulmannCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulmannCTLight) / sizeof(kExposesPaulmannCTLight[0]));

const ::zhc::BindingSpec kBindingsPaulmannCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsPaulmannCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulmannCTLight) / sizeof(kBindingsPaulmannCTLight[0]));

// ── Colour-only (m.light({color})) ──────────────────────────────────
const ::zhc::FzConverter* const kFzPaulmannColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzPaulmannColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulmannColorLight) / sizeof(kFzPaulmannColorLight[0]));

const ::zhc::TzConverter* const kTzPaulmannColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzPaulmannColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulmannColorLight) / sizeof(kTzPaulmannColorLight[0]));

const ::zhc::Expose kExposesPaulmannColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulmannColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulmannColorLight) / sizeof(kExposesPaulmannColorLight[0]));

const ::zhc::BindingSpec kBindingsPaulmannColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsPaulmannColorLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulmannColorLight) / sizeof(kBindingsPaulmannColorLight[0]));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzPaulmannColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzPaulmannColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulmannColorCTLight) / sizeof(kFzPaulmannColorCTLight[0]));

const ::zhc::TzConverter* const kTzPaulmannColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzPaulmannColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulmannColorCTLight) / sizeof(kTzPaulmannColorCTLight[0]));

const ::zhc::Expose kExposesPaulmannColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulmannColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulmannColorCTLight) / sizeof(kExposesPaulmannColorCTLight[0]));

const ::zhc::BindingSpec kBindingsPaulmannColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsPaulmannColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulmannColorCTLight) / sizeof(kBindingsPaulmannColorCTLight[0]));

}  // namespace zhc::devices::paulmann
