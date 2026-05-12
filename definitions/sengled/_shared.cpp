// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {

// ── Plain dimmable (sengledLight()) ─────────────────────────────────
const ::zhc::FzConverter* const kFzSengledLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzSengledLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSengledLight) / sizeof(kFzSengledLight[0]));

const ::zhc::TzConverter* const kTzSengledLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzSengledLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSengledLight) / sizeof(kTzSengledLight[0]));

const ::zhc::Expose kExposesSengledLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSengledLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSengledLight) / sizeof(kExposesSengledLight[0]));

const ::zhc::BindingSpec kBindingsSengledLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsSengledLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSengledLight) / sizeof(kBindingsSengledLight[0]));

// ── Tunable white (sengledLight({colorTemp})) ───────────────────────
const ::zhc::FzConverter* const kFzSengledCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzSengledCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSengledCTLight) / sizeof(kFzSengledCTLight[0]));

const ::zhc::TzConverter* const kTzSengledCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzSengledCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSengledCTLight) / sizeof(kTzSengledCTLight[0]));

const ::zhc::Expose kExposesSengledCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesSengledCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSengledCTLight) / sizeof(kExposesSengledCTLight[0]));

const ::zhc::BindingSpec kBindingsSengledCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsSengledCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSengledCTLight) / sizeof(kBindingsSengledCTLight[0]));

// ── Colour-only (sengledLight({colorTemp: undefined, color: ...})) ──
const ::zhc::FzConverter* const kFzSengledColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzSengledColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSengledColorLight) / sizeof(kFzSengledColorLight[0]));

const ::zhc::TzConverter* const kTzSengledColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzSengledColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSengledColorLight) / sizeof(kTzSengledColorLight[0]));

const ::zhc::Expose kExposesSengledColorLight[] = {
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
const std::uint8_t kExposesSengledColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSengledColorLight) / sizeof(kExposesSengledColorLight[0]));

const ::zhc::BindingSpec kBindingsSengledColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsSengledColorLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSengledColorLight) / sizeof(kBindingsSengledColorLight[0]));

// ── Full RGBW (sengledLight({colorTemp, color})) ────────────────────
const ::zhc::FzConverter* const kFzSengledColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzSengledColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzSengledColorCTLight) / sizeof(kFzSengledColorCTLight[0]));

const ::zhc::TzConverter* const kTzSengledColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzSengledColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzSengledColorCTLight) / sizeof(kTzSengledColorCTLight[0]));

const ::zhc::Expose kExposesSengledColorCTLight[] = {
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
const std::uint8_t kExposesSengledColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesSengledColorCTLight) / sizeof(kExposesSengledColorCTLight[0]));

const ::zhc::BindingSpec kBindingsSengledColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsSengledColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsSengledColorCTLight) / sizeof(kBindingsSengledColorCTLight[0]));

}  // namespace zhc::devices::sengled
