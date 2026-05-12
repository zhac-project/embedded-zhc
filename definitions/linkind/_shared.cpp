// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Two shared bundles for Linkind ports — covering the `m.light({})` and
// `m.light({colorTemp})` permutations seen in z2m's linkind.ts. No
// Color/ColorCT bundles needed — Linkind ships only dim and CCT lights.

#include "definitions/linkind/_shared.hpp"

namespace zhc::devices::linkind {

// ── Plain dimmable (m.light({})) ────────────────────────────────────
const ::zhc::FzConverter* const kFzLinkindLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzLinkindLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLinkindLight) / sizeof(kFzLinkindLight[0]));

const ::zhc::TzConverter* const kTzLinkindLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzLinkindLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLinkindLight) / sizeof(kTzLinkindLight[0]));

const ::zhc::Expose kExposesLinkindLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLinkindLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLinkindLight) / sizeof(kExposesLinkindLight[0]));

const ::zhc::BindingSpec kBindingsLinkindLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsLinkindLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLinkindLight) / sizeof(kBindingsLinkindLight[0]));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzLinkindCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzLinkindCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzLinkindCTLight) / sizeof(kFzLinkindCTLight[0]));

const ::zhc::TzConverter* const kTzLinkindCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzLinkindCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzLinkindCTLight) / sizeof(kTzLinkindCTLight[0]));

const ::zhc::Expose kExposesLinkindCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesLinkindCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesLinkindCTLight) / sizeof(kExposesLinkindCTLight[0]));

const ::zhc::BindingSpec kBindingsLinkindCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsLinkindCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsLinkindCTLight) / sizeof(kBindingsLinkindCTLight[0]));

}  // namespace zhc::devices::linkind
