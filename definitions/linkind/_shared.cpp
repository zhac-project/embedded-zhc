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
static_assert(kFzLinkindLightCount == std::size(kFzLinkindLight));

const ::zhc::TzConverter* const kTzLinkindLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzLinkindLightCount == std::size(kTzLinkindLight));

const ::zhc::Expose kExposesLinkindLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesLinkindLightCount == std::size(kExposesLinkindLight));

const ::zhc::BindingSpec kBindingsLinkindLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsLinkindLightCount == std::size(kBindingsLinkindLight));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzLinkindCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzLinkindCTLightCount == std::size(kFzLinkindCTLight));

const ::zhc::TzConverter* const kTzLinkindCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzLinkindCTLightCount == std::size(kTzLinkindCTLight));

const ::zhc::Expose kExposesLinkindCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesLinkindCTLightCount == std::size(kExposesLinkindCTLight));

const ::zhc::BindingSpec kBindingsLinkindCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsLinkindCTLightCount == std::size(kBindingsLinkindCTLight));

}  // namespace zhc::devices::linkind
