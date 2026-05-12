// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Ecosmart ports — one per `m.light(...)` feature
// combination seen in z2m's `ecosmart.ts`. Modelled on
// definitions/aurora_lighting/_shared.cpp / definitions/gledopto/_shared.cpp.
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzEcosmartLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzEcosmartLightCount =
    static_cast<std::uint8_t>(sizeof(kFzEcosmartLight) / sizeof(kFzEcosmartLight[0]));

const ::zhc::TzConverter* const kTzEcosmartLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzEcosmartLightCount =
    static_cast<std::uint8_t>(sizeof(kTzEcosmartLight) / sizeof(kTzEcosmartLight[0]));

const ::zhc::Expose kExposesEcosmartLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesEcosmartLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesEcosmartLight) / sizeof(kExposesEcosmartLight[0]));

const ::zhc::BindingSpec kBindingsEcosmartLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsEcosmartLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsEcosmartLight) / sizeof(kBindingsEcosmartLight[0]));

// ── CCT (m.light({colorTemp: ...})) ─────────────────────────────────
const ::zhc::FzConverter* const kFzEcosmartCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzEcosmartCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzEcosmartCTLight) / sizeof(kFzEcosmartCTLight[0]));

const ::zhc::TzConverter* const kTzEcosmartCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzEcosmartCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzEcosmartCTLight) / sizeof(kTzEcosmartCTLight[0]));

const ::zhc::Expose kExposesEcosmartCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesEcosmartCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesEcosmartCTLight) / sizeof(kExposesEcosmartCTLight[0]));

const ::zhc::BindingSpec kBindingsEcosmartCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsEcosmartCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsEcosmartCTLight) / sizeof(kBindingsEcosmartCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
const ::zhc::FzConverter* const kFzEcosmartColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzEcosmartColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzEcosmartColorCTLight) / sizeof(kFzEcosmartColorCTLight[0]));

const ::zhc::TzConverter* const kTzEcosmartColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzEcosmartColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzEcosmartColorCTLight) / sizeof(kTzEcosmartColorCTLight[0]));

const ::zhc::Expose kExposesEcosmartColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesEcosmartColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesEcosmartColorCTLight) / sizeof(kExposesEcosmartColorCTLight[0]));

const ::zhc::BindingSpec kBindingsEcosmartColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsEcosmartColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsEcosmartColorCTLight) / sizeof(kBindingsEcosmartColorCTLight[0]));

}  // namespace zhc::devices::ecosmart
