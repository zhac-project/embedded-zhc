// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Two shared bundles for EGLO connect bulb ports — one per `m.light()`
// feature combination seen in z2m's eglo.ts. Modelled after awox/_shared.
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzEgloCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzEgloCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzEgloCTLight) / sizeof(kFzEgloCTLight[0]));

const ::zhc::TzConverter* const kTzEgloCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzEgloCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzEgloCTLight) / sizeof(kTzEgloCTLight[0]));

const ::zhc::Expose kExposesEgloCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesEgloCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesEgloCTLight) / sizeof(kExposesEgloCTLight[0]));

const ::zhc::BindingSpec kBindingsEgloCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsEgloCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsEgloCTLight) / sizeof(kBindingsEgloCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color:{modes:["xy","hs"]}})) ─────
const ::zhc::FzConverter* const kFzEgloColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzEgloColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzEgloColorCTLight) / sizeof(kFzEgloColorCTLight[0]));

const ::zhc::TzConverter* const kTzEgloColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzEgloColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzEgloColorCTLight) / sizeof(kTzEgloColorCTLight[0]));

const ::zhc::Expose kExposesEgloColorCTLight[] = {
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
const std::uint8_t kExposesEgloColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesEgloColorCTLight) / sizeof(kExposesEgloColorCTLight[0]));

const ::zhc::BindingSpec kBindingsEgloColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsEgloColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsEgloColorCTLight) / sizeof(kBindingsEgloColorCTLight[0]));

}  // namespace zhc::devices::eglo
