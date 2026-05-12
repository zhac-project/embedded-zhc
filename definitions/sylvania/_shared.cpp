// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Sylvania ports — one per
// ledvanceLight()/ledvanceOnOff() feature combination seen in z2m's
// sylvania.ts. Modelled after osram/_shared / ledvance/_shared.
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {

// ── On/off only (ledvanceOnOff) ─────────────────────────────────────
const ::zhc::FzConverter* const kFzSylvaniaOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
const std::uint8_t kFzSylvaniaOnOffCount =
    static_cast<std::uint8_t>(sizeof(kFzSylvaniaOnOff) / sizeof(kFzSylvaniaOnOff[0]));

const ::zhc::TzConverter* const kTzSylvaniaOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzSylvaniaOnOffCount =
    static_cast<std::uint8_t>(sizeof(kTzSylvaniaOnOff) / sizeof(kTzSylvaniaOnOff[0]));

const ::zhc::Expose kExposesSylvaniaOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSylvaniaOnOffCount =
    static_cast<std::uint8_t>(sizeof(kExposesSylvaniaOnOff) / sizeof(kExposesSylvaniaOnOff[0]));

const ::zhc::BindingSpec kBindingsSylvaniaOnOff[] = {
    { 1, 0x0006 },
};
const std::uint8_t kBindingsSylvaniaOnOffCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSylvaniaOnOff) / sizeof(kBindingsSylvaniaOnOff[0]));

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
const ::zhc::FzConverter* const kFzSylvaniaDim[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzSylvaniaDimCount =
    static_cast<std::uint8_t>(sizeof(kFzSylvaniaDim) / sizeof(kFzSylvaniaDim[0]));

const ::zhc::TzConverter* const kTzSylvaniaDim[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzSylvaniaDimCount =
    static_cast<std::uint8_t>(sizeof(kTzSylvaniaDim) / sizeof(kTzSylvaniaDim[0]));

const ::zhc::Expose kExposesSylvaniaDim[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSylvaniaDimCount =
    static_cast<std::uint8_t>(sizeof(kExposesSylvaniaDim) / sizeof(kExposesSylvaniaDim[0]));

const ::zhc::BindingSpec kBindingsSylvaniaDim[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsSylvaniaDimCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSylvaniaDim) / sizeof(kBindingsSylvaniaDim[0]));

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
const ::zhc::FzConverter* const kFzSylvaniaLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzSylvaniaLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSylvaniaLight) / sizeof(kFzSylvaniaLight[0]));

const ::zhc::TzConverter* const kTzSylvaniaLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzSylvaniaLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSylvaniaLight) / sizeof(kTzSylvaniaLight[0]));

const ::zhc::Expose kExposesSylvaniaLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSylvaniaLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSylvaniaLight) / sizeof(kExposesSylvaniaLight[0]));

const ::zhc::BindingSpec kBindingsSylvaniaLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsSylvaniaLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSylvaniaLight) / sizeof(kBindingsSylvaniaLight[0]));

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
const ::zhc::FzConverter* const kFzSylvaniaColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzSylvaniaColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSylvaniaColorLight) / sizeof(kFzSylvaniaColorLight[0]));

const ::zhc::TzConverter* const kTzSylvaniaColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzSylvaniaColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSylvaniaColorLight) / sizeof(kTzSylvaniaColorLight[0]));

const ::zhc::Expose kExposesSylvaniaColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSylvaniaColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSylvaniaColorLight) / sizeof(kExposesSylvaniaColorLight[0]));

const ::zhc::BindingSpec kBindingsSylvaniaColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsSylvaniaColorLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSylvaniaColorLight) / sizeof(kBindingsSylvaniaColorLight[0]));

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzSylvaniaColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzSylvaniaColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSylvaniaColorCTLight) / sizeof(kFzSylvaniaColorCTLight[0]));

const ::zhc::TzConverter* const kTzSylvaniaColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzSylvaniaColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSylvaniaColorCTLight) / sizeof(kTzSylvaniaColorCTLight[0]));

const ::zhc::Expose kExposesSylvaniaColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSylvaniaColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSylvaniaColorCTLight) / sizeof(kExposesSylvaniaColorCTLight[0]));

const ::zhc::BindingSpec kBindingsSylvaniaColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsSylvaniaColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSylvaniaColorCTLight) / sizeof(kBindingsSylvaniaColorCTLight[0]));

}  // namespace zhc::devices::sylvania
