// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared bundles for Innr light ports — one per `m.light(...)`
// feature combination seen in z2m's innr.ts. Modelled after
// `definitions/gledopto/_shared.cpp` (which itself follows Philips'
// four-tier split). Innr has no colour-only-without-CT SKU, so the
// ColorLight tier is intentionally omitted.
#include "definitions/innr/_shared.hpp"

namespace zhc::devices::innr {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzInnrLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzInnrLightCount =
    static_cast<std::uint8_t>(sizeof(kFzInnrLight) / sizeof(kFzInnrLight[0]));

const ::zhc::TzConverter* const kTzInnrLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzInnrLightCount =
    static_cast<std::uint8_t>(sizeof(kTzInnrLight) / sizeof(kTzInnrLight[0]));

const ::zhc::Expose kExposesInnrLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesInnrLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesInnrLight) / sizeof(kExposesInnrLight[0]));

const ::zhc::BindingSpec kBindingsInnrLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsInnrLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsInnrLight) / sizeof(kBindingsInnrLight[0]));

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzInnrCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzInnrCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzInnrCTLight) / sizeof(kFzInnrCTLight[0]));

const ::zhc::TzConverter* const kTzInnrCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzInnrCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzInnrCTLight) / sizeof(kTzInnrCTLight[0]));

const ::zhc::Expose kExposesInnrCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesInnrCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesInnrCTLight) / sizeof(kExposesInnrCTLight[0]));

const ::zhc::BindingSpec kBindingsInnrCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsInnrCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsInnrCTLight) / sizeof(kBindingsInnrCTLight[0]));

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
const ::zhc::FzConverter* const kFzInnrColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzInnrColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzInnrColorCTLight) / sizeof(kFzInnrColorCTLight[0]));

const ::zhc::TzConverter* const kTzInnrColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzInnrColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzInnrColorCTLight) / sizeof(kTzInnrColorCTLight[0]));

const ::zhc::Expose kExposesInnrColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_xy",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_hs",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesInnrColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesInnrColorCTLight) / sizeof(kExposesInnrColorCTLight[0]));

const ::zhc::BindingSpec kBindingsInnrColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsInnrColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsInnrColorCTLight) / sizeof(kBindingsInnrColorCTLight[0]));

}  // namespace zhc::devices::innr
