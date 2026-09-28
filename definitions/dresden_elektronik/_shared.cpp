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
static_assert(kFzDresdenCTLightCount == std::size(kFzDresdenCTLight));

const ::zhc::TzConverter* const kTzDresdenCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzDresdenCTLightCount == std::size(kTzDresdenCTLight));

const ::zhc::Expose kExposesDresdenCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesDresdenCTLightCount == std::size(kExposesDresdenCTLight));

const ::zhc::BindingSpec kBindingsDresdenCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsDresdenCTLightCount == std::size(kBindingsDresdenCTLight));

// ── Color + CT light  (m.light({colorTemp, color:true})) ────────────
const ::zhc::FzConverter* const kFzDresdenColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzDresdenColorCTLightCount == std::size(kFzDresdenColorCTLight));

const ::zhc::TzConverter* const kTzDresdenColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzDresdenColorCTLightCount == std::size(kTzDresdenColorCTLight));

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
static_assert(kExposesDresdenColorCTLightCount == std::size(kExposesDresdenColorCTLight));

const ::zhc::BindingSpec kBindingsDresdenColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsDresdenColorCTLightCount == std::size(kBindingsDresdenColorCTLight));

}  // namespace zhc::devices::dresden_elektronik
