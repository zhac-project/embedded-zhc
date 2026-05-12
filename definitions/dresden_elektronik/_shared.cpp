// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared light bundles for Dresden Elektronik / Phoscon ports.
// See _shared.hpp for the bundle map.
#include "definitions/dresden_elektronik/_shared.hpp"

namespace zhc::devices::dresden_elektronik {

// ── CT light  (m.light({colorTemp})) ────────────────────────────────
const ::zhc::FzConverter* const kFzDresdenCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzDresdenCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzDresdenCTLight) / sizeof(kFzDresdenCTLight[0]));

const ::zhc::TzConverter* const kTzDresdenCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzDresdenCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzDresdenCTLight) / sizeof(kTzDresdenCTLight[0]));

const ::zhc::Expose kExposesDresdenCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesDresdenCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesDresdenCTLight) / sizeof(kExposesDresdenCTLight[0]));

const ::zhc::BindingSpec kBindingsDresdenCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsDresdenCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsDresdenCTLight) / sizeof(kBindingsDresdenCTLight[0]));

// ── Color + CT light  (m.light({colorTemp, color:true})) ────────────
const ::zhc::FzConverter* const kFzDresdenColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzDresdenColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzDresdenColorCTLight) / sizeof(kFzDresdenColorCTLight[0]));

const ::zhc::TzConverter* const kTzDresdenColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzDresdenColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzDresdenColorCTLight) / sizeof(kTzDresdenColorCTLight[0]));

const ::zhc::Expose kExposesDresdenColorCTLight[] = {
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
const std::uint8_t kExposesDresdenColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesDresdenColorCTLight) / sizeof(kExposesDresdenColorCTLight[0]));

const ::zhc::BindingSpec kBindingsDresdenColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsDresdenColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsDresdenColorCTLight) / sizeof(kBindingsDresdenColorCTLight[0]));

}  // namespace zhc::devices::dresden_elektronik
