// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Osram ports — one per ledvanceLight()/ledvanceOnOff()
// feature combination seen in z2m's osram.ts. Modelled after gledopto/_shared.
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {

// ── On/off only (ledvanceOnOff) ─────────────────────────────────────
const ::zhc::FzConverter* const kFzOsramOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
const std::uint8_t kFzOsramOnOffCount =
    static_cast<std::uint8_t>(sizeof(kFzOsramOnOff) / sizeof(kFzOsramOnOff[0]));

const ::zhc::TzConverter* const kTzOsramOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzOsramOnOffCount =
    static_cast<std::uint8_t>(sizeof(kTzOsramOnOff) / sizeof(kTzOsramOnOff[0]));

const ::zhc::Expose kExposesOsramOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesOsramOnOffCount =
    static_cast<std::uint8_t>(sizeof(kExposesOsramOnOff) / sizeof(kExposesOsramOnOff[0]));

const ::zhc::BindingSpec kBindingsOsramOnOff[] = {
    { 1, 0x0006 },
};
const std::uint8_t kBindingsOsramOnOffCount =
    static_cast<std::uint8_t>(sizeof(kBindingsOsramOnOff) / sizeof(kBindingsOsramOnOff[0]));

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
const ::zhc::FzConverter* const kFzOsramLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzOsramLightCount =
    static_cast<std::uint8_t>(sizeof(kFzOsramLight) / sizeof(kFzOsramLight[0]));

const ::zhc::TzConverter* const kTzOsramLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzOsramLightCount =
    static_cast<std::uint8_t>(sizeof(kTzOsramLight) / sizeof(kTzOsramLight[0]));

const ::zhc::Expose kExposesOsramLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesOsramLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesOsramLight) / sizeof(kExposesOsramLight[0]));

const ::zhc::BindingSpec kBindingsOsramLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsOsramLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsOsramLight) / sizeof(kBindingsOsramLight[0]));

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
const ::zhc::FzConverter* const kFzOsramCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzOsramCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzOsramCTLight) / sizeof(kFzOsramCTLight[0]));

const ::zhc::TzConverter* const kTzOsramCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzOsramCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzOsramCTLight) / sizeof(kTzOsramCTLight[0]));

const ::zhc::Expose kExposesOsramCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesOsramCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesOsramCTLight) / sizeof(kExposesOsramCTLight[0]));

const ::zhc::BindingSpec kBindingsOsramCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsOsramCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsOsramCTLight) / sizeof(kBindingsOsramCTLight[0]));

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
const ::zhc::FzConverter* const kFzOsramColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzOsramColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzOsramColorLight) / sizeof(kFzOsramColorLight[0]));

const ::zhc::TzConverter* const kTzOsramColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzOsramColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzOsramColorLight) / sizeof(kTzOsramColorLight[0]));

const ::zhc::Expose kExposesOsramColorLight[] = {
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
const std::uint8_t kExposesOsramColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesOsramColorLight) / sizeof(kExposesOsramColorLight[0]));

const ::zhc::BindingSpec kBindingsOsramColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsOsramColorLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsOsramColorLight) / sizeof(kBindingsOsramColorLight[0]));

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
const ::zhc::FzConverter* const kFzOsramColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzOsramColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzOsramColorCTLight) / sizeof(kFzOsramColorCTLight[0]));

const ::zhc::TzConverter* const kTzOsramColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzOsramColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzOsramColorCTLight) / sizeof(kTzOsramColorCTLight[0]));

const ::zhc::Expose kExposesOsramColorCTLight[] = {
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
const std::uint8_t kExposesOsramColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesOsramColorCTLight) / sizeof(kExposesOsramColorCTLight[0]));

const ::zhc::BindingSpec kBindingsOsramColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsOsramColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsOsramColorCTLight) / sizeof(kBindingsOsramColorCTLight[0]));

}  // namespace zhc::devices::osram
