// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Leedarson light plumbing — modelled on osram/_shared.
// Leedarson's z2m surface for lights is plain `m.light(...)` over stock
// ZCL clusters; nine of the fourteen leedarson.ts entries are
// `m.light({})`, `m.light({colorTemp})` or `m.light({colorTemp,color:true})`,
// so we flatten them to three shared bundles.
//
//   Tier         | feature set                          | bindings
//   -------------+--------------------------------------+----------------
//   Light        | on/off + brightness                  | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)                 | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)         | + 0x0300
//
// Picking the wrong bundle is harmless on-device — the extra fz/tz just
// never fire — but the exposes list shows up in the SPA, so use the
// closest match.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {

// ── Plain dimmable (m.light()) ───────────────────────────────────────
extern const ::zhc::FzConverter* const kFzLeedarsonLight[];
inline constexpr std::uint8_t          kFzLeedarsonLightCount = 2;

extern const ::zhc::TzConverter* const kTzLeedarsonLight[];
inline constexpr std::uint8_t          kTzLeedarsonLightCount = 2;

extern const ::zhc::Expose             kExposesLeedarsonLight[];
inline constexpr std::uint8_t          kExposesLeedarsonLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsLeedarsonLight[];
inline constexpr std::uint8_t          kBindingsLeedarsonLightCount = 2;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzLeedarsonCTLight[];
inline constexpr std::uint8_t          kFzLeedarsonCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzLeedarsonCTLight[];
inline constexpr std::uint8_t          kTzLeedarsonCTLightCount = 3;

extern const ::zhc::Expose             kExposesLeedarsonCTLight[];
inline constexpr std::uint8_t          kExposesLeedarsonCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsLeedarsonCTLight[];
inline constexpr std::uint8_t          kBindingsLeedarsonCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzLeedarsonColorCTLight[];
inline constexpr std::uint8_t          kFzLeedarsonColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzLeedarsonColorCTLight[];
inline constexpr std::uint8_t          kTzLeedarsonColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesLeedarsonColorCTLight[];
inline constexpr std::uint8_t          kExposesLeedarsonColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsLeedarsonColorCTLight[];
inline constexpr std::uint8_t          kBindingsLeedarsonColorCTLightCount = 3;

}  // namespace zhc::devices::leedarson
