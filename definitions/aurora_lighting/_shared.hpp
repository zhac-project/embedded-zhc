// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Aurora-AOne light plumbing. Aurora's bulb surface in z2m is
// exclusively `m.light(...)`, mirroring gledopto / paulmann / sengled,
// so we flatten the three feature combinations seen in
// `aurora_lighting.ts` to three shared bundles. Each generated/Aur_*.cpp
// for a bulb stays a one-liner pointing at the appropriate tier.
//
//   Tier         | feature set                        | bindings
//   -------------+------------------------------------+----------
//   Light        | on/off + brightness                | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)               | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)       | + 0x0300
//
// Aurora has no `color: true`-only bulbs in z2m — every RGB SKU also
// exposes color_temp. Picking the wrong bundle is harmless on-device
// (the extra fz/tz never fire), but the exposes list is what surfaces
// in the SPA, so use the closest match.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzAuroraLight[];
inline constexpr std::uint8_t          kFzAuroraLightCount = 2;

extern const ::zhc::TzConverter* const kTzAuroraLight[];
inline constexpr std::uint8_t          kTzAuroraLightCount = 2;

extern const ::zhc::Expose             kExposesAuroraLight[];
inline constexpr std::uint8_t          kExposesAuroraLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsAuroraLight[];
inline constexpr std::uint8_t          kBindingsAuroraLightCount = 2;

// ── White-ambiance / CCT (m.light({colorTemp: ...})) ────────────────
extern const ::zhc::FzConverter* const kFzAuroraCTLight[];
inline constexpr std::uint8_t          kFzAuroraCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzAuroraCTLight[];
inline constexpr std::uint8_t          kTzAuroraCTLightCount = 3;

extern const ::zhc::Expose             kExposesAuroraCTLight[];
inline constexpr std::uint8_t          kExposesAuroraCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsAuroraCTLight[];
inline constexpr std::uint8_t          kBindingsAuroraCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzAuroraColorCTLight[];
inline constexpr std::uint8_t          kFzAuroraColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzAuroraColorCTLight[];
inline constexpr std::uint8_t          kTzAuroraColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesAuroraColorCTLight[];
inline constexpr std::uint8_t          kExposesAuroraColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsAuroraColorCTLight[];
inline constexpr std::uint8_t          kBindingsAuroraColorCTLightCount = 3;

}  // namespace zhc::devices::aurora_lighting
