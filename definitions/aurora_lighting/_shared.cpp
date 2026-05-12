// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Aurora-AOne bulb ports — one per m.light()
// feature combination seen in z2m's aurora_lighting.ts. Modelled after
// gledopto/_shared.cpp.
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzAuroraLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzAuroraLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAuroraLight) / sizeof(kFzAuroraLight[0]));

const ::zhc::TzConverter* const kTzAuroraLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzAuroraLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAuroraLight) / sizeof(kTzAuroraLight[0]));

const ::zhc::Expose kExposesAuroraLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAuroraLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAuroraLight) / sizeof(kExposesAuroraLight[0]));

const ::zhc::BindingSpec kBindingsAuroraLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsAuroraLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAuroraLight) / sizeof(kBindingsAuroraLight[0]));

// ── CCT (m.light({colorTemp: ...})) ─────────────────────────────────
const ::zhc::FzConverter* const kFzAuroraCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzAuroraCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAuroraCTLight) / sizeof(kFzAuroraCTLight[0]));

const ::zhc::TzConverter* const kTzAuroraCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzAuroraCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAuroraCTLight) / sizeof(kTzAuroraCTLight[0]));

const ::zhc::Expose kExposesAuroraCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAuroraCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAuroraCTLight) / sizeof(kExposesAuroraCTLight[0]));

const ::zhc::BindingSpec kBindingsAuroraCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAuroraCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAuroraCTLight) / sizeof(kBindingsAuroraCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzAuroraColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzAuroraColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAuroraColorCTLight) / sizeof(kFzAuroraColorCTLight[0]));

const ::zhc::TzConverter* const kTzAuroraColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzAuroraColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAuroraColorCTLight) / sizeof(kTzAuroraColorCTLight[0]));

const ::zhc::Expose kExposesAuroraColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAuroraColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAuroraColorCTLight) / sizeof(kExposesAuroraColorCTLight[0]));

const ::zhc::BindingSpec kBindingsAuroraColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAuroraColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAuroraColorCTLight) / sizeof(kBindingsAuroraColorCTLight[0]));

}  // namespace zhc::devices::aurora_lighting
