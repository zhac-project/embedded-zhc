// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Müller-Licht (tint) ports — one per
// mullerLichtLight()/m.onOff() feature combination seen in muller_licht.ts.
// Modelled after osram/_shared.
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
const ::zhc::FzConverter* const kFzMullerLichtOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
const std::uint8_t kFzMullerLichtOnOffCount =
    static_cast<std::uint8_t>(sizeof(kFzMullerLichtOnOff) / sizeof(kFzMullerLichtOnOff[0]));

const ::zhc::TzConverter* const kTzMullerLichtOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzMullerLichtOnOffCount =
    static_cast<std::uint8_t>(sizeof(kTzMullerLichtOnOff) / sizeof(kTzMullerLichtOnOff[0]));

const ::zhc::Expose kExposesMullerLichtOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesMullerLichtOnOffCount =
    static_cast<std::uint8_t>(sizeof(kExposesMullerLichtOnOff) / sizeof(kExposesMullerLichtOnOff[0]));

const ::zhc::BindingSpec kBindingsMullerLichtOnOff[] = {
    { 1, 0x0006 },
};
const std::uint8_t kBindingsMullerLichtOnOffCount =
    static_cast<std::uint8_t>(sizeof(kBindingsMullerLichtOnOff) / sizeof(kBindingsMullerLichtOnOff[0]));

// ── Dimmable (m.light({})) ──────────────────────────────────────────
const ::zhc::FzConverter* const kFzMullerLichtLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzMullerLichtLightCount =
    static_cast<std::uint8_t>(sizeof(kFzMullerLichtLight) / sizeof(kFzMullerLichtLight[0]));

const ::zhc::TzConverter* const kTzMullerLichtLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzMullerLichtLightCount =
    static_cast<std::uint8_t>(sizeof(kTzMullerLichtLight) / sizeof(kTzMullerLichtLight[0]));

const ::zhc::Expose kExposesMullerLichtLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesMullerLichtLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesMullerLichtLight) / sizeof(kExposesMullerLichtLight[0]));

const ::zhc::BindingSpec kBindingsMullerLichtLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsMullerLichtLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsMullerLichtLight) / sizeof(kBindingsMullerLichtLight[0]));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzMullerLichtCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzMullerLichtCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzMullerLichtCTLight) / sizeof(kFzMullerLichtCTLight[0]));

const ::zhc::TzConverter* const kTzMullerLichtCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzMullerLichtCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzMullerLichtCTLight) / sizeof(kTzMullerLichtCTLight[0]));

const ::zhc::Expose kExposesMullerLichtCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired",  nullptr, nullptr, 0 },
};
const std::uint8_t kExposesMullerLichtCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesMullerLichtCTLight) / sizeof(kExposesMullerLichtCTLight[0]));

const ::zhc::BindingSpec kBindingsMullerLichtCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsMullerLichtCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsMullerLichtCTLight) / sizeof(kBindingsMullerLichtCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzMullerLichtColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzMullerLichtColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzMullerLichtColorCTLight) / sizeof(kFzMullerLichtColorCTLight[0]));

const ::zhc::TzConverter* const kTzMullerLichtColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzMullerLichtColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzMullerLichtColorCTLight) / sizeof(kTzMullerLichtColorCTLight[0]));

const ::zhc::Expose kExposesMullerLichtColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired",  nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesMullerLichtColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesMullerLichtColorCTLight) / sizeof(kExposesMullerLichtColorCTLight[0]));

const ::zhc::BindingSpec kBindingsMullerLichtColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsMullerLichtColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsMullerLichtColorCTLight) / sizeof(kBindingsMullerLichtColorCTLight[0]));

}  // namespace zhc::devices::muller_licht
