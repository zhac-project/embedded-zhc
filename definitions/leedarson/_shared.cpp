// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Leedarson ports — one per `m.light(...)`
// feature combination seen in z2m's leedarson.ts. Modelled after
// osram/_shared.
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzLeedarsonLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzLeedarsonLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLeedarsonLight) / sizeof(kFzLeedarsonLight[0]));

const ::zhc::TzConverter* const kTzLeedarsonLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzLeedarsonLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLeedarsonLight) / sizeof(kTzLeedarsonLight[0]));

const ::zhc::Expose kExposesLeedarsonLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLeedarsonLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLeedarsonLight) / sizeof(kExposesLeedarsonLight[0]));

const ::zhc::BindingSpec kBindingsLeedarsonLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsLeedarsonLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLeedarsonLight) / sizeof(kBindingsLeedarsonLight[0]));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzLeedarsonCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzLeedarsonCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLeedarsonCTLight) / sizeof(kFzLeedarsonCTLight[0]));

const ::zhc::TzConverter* const kTzLeedarsonCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzLeedarsonCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLeedarsonCTLight) / sizeof(kTzLeedarsonCTLight[0]));

const ::zhc::Expose kExposesLeedarsonCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLeedarsonCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLeedarsonCTLight) / sizeof(kExposesLeedarsonCTLight[0]));

const ::zhc::BindingSpec kBindingsLeedarsonCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsLeedarsonCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLeedarsonCTLight) / sizeof(kBindingsLeedarsonCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzLeedarsonColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzLeedarsonColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLeedarsonColorCTLight) / sizeof(kFzLeedarsonColorCTLight[0]));

const ::zhc::TzConverter* const kTzLeedarsonColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzLeedarsonColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLeedarsonColorCTLight) / sizeof(kTzLeedarsonColorCTLight[0]));

const ::zhc::Expose kExposesLeedarsonColorCTLight[] = {
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
const std::uint8_t kExposesLeedarsonColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLeedarsonColorCTLight) / sizeof(kExposesLeedarsonColorCTLight[0]));

const ::zhc::BindingSpec kBindingsLeedarsonColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsLeedarsonColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLeedarsonColorCTLight) / sizeof(kBindingsLeedarsonColorCTLight[0]));

}  // namespace zhc::devices::leedarson
