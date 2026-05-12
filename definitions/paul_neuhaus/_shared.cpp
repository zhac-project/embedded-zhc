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
const std::uint8_t kFzPaulNeuhausLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulNeuhausLight) / sizeof(kFzPaulNeuhausLight[0]));

const ::zhc::TzConverter* const kTzPaulNeuhausLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzPaulNeuhausLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulNeuhausLight) / sizeof(kTzPaulNeuhausLight[0]));

const ::zhc::Expose kExposesPaulNeuhausLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulNeuhausLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulNeuhausLight) / sizeof(kExposesPaulNeuhausLight[0]));

const ::zhc::BindingSpec kBindingsPaulNeuhausLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsPaulNeuhausLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulNeuhausLight) / sizeof(kBindingsPaulNeuhausLight[0]));

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzPaulNeuhausCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzPaulNeuhausCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulNeuhausCTLight) / sizeof(kFzPaulNeuhausCTLight[0]));

const ::zhc::TzConverter* const kTzPaulNeuhausCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzPaulNeuhausCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulNeuhausCTLight) / sizeof(kTzPaulNeuhausCTLight[0]));

const ::zhc::Expose kExposesPaulNeuhausCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulNeuhausCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulNeuhausCTLight) / sizeof(kExposesPaulNeuhausCTLight[0]));

const ::zhc::BindingSpec kBindingsPaulNeuhausCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsPaulNeuhausCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulNeuhausCTLight) / sizeof(kBindingsPaulNeuhausCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzPaulNeuhausColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzPaulNeuhausColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzPaulNeuhausColorCTLight) / sizeof(kFzPaulNeuhausColorCTLight[0]));

const ::zhc::TzConverter* const kTzPaulNeuhausColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzPaulNeuhausColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzPaulNeuhausColorCTLight) / sizeof(kTzPaulNeuhausColorCTLight[0]));

const ::zhc::Expose kExposesPaulNeuhausColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesPaulNeuhausColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesPaulNeuhausColorCTLight) / sizeof(kExposesPaulNeuhausColorCTLight[0]));

const ::zhc::BindingSpec kBindingsPaulNeuhausColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsPaulNeuhausColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsPaulNeuhausColorCTLight) / sizeof(kBindingsPaulNeuhausColorCTLight[0]));

}  // namespace zhc::devices::paul_neuhaus
