// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for AwoX bulb ports — one per `m.light()` feature
// combination seen in z2m's awox.ts. Modelled after osram/_shared.
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {

// ── Plain dimmable (m.light({})) ────────────────────────────────────
const ::zhc::FzConverter* const kFzAwoxLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzAwoxLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAwoxLight) / sizeof(kFzAwoxLight[0]));

const ::zhc::TzConverter* const kTzAwoxLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzAwoxLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAwoxLight) / sizeof(kTzAwoxLight[0]));

const ::zhc::Expose kExposesAwoxLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAwoxLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAwoxLight) / sizeof(kExposesAwoxLight[0]));

const ::zhc::BindingSpec kBindingsAwoxLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsAwoxLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAwoxLight) / sizeof(kBindingsAwoxLight[0]));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzAwoxCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzAwoxCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAwoxCTLight) / sizeof(kFzAwoxCTLight[0]));

const ::zhc::TzConverter* const kTzAwoxCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzAwoxCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAwoxCTLight) / sizeof(kTzAwoxCTLight[0]));

const ::zhc::Expose kExposesAwoxCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAwoxCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAwoxCTLight) / sizeof(kExposesAwoxCTLight[0]));

const ::zhc::BindingSpec kBindingsAwoxCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAwoxCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAwoxCTLight) / sizeof(kBindingsAwoxCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color: {modes:["xy","hs"]}})) ────
const ::zhc::FzConverter* const kFzAwoxColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzAwoxColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAwoxColorCTLight) / sizeof(kFzAwoxColorCTLight[0]));

const ::zhc::TzConverter* const kTzAwoxColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzAwoxColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAwoxColorCTLight) / sizeof(kTzAwoxColorCTLight[0]));

const ::zhc::Expose kExposesAwoxColorCTLight[] = {
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
const std::uint8_t kExposesAwoxColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAwoxColorCTLight) / sizeof(kExposesAwoxColorCTLight[0]));

const ::zhc::BindingSpec kBindingsAwoxColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAwoxColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAwoxColorCTLight) / sizeof(kBindingsAwoxColorCTLight[0]));

}  // namespace zhc::devices::awox
