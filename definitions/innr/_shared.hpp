// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Innr light plumbing. Innr's z2m surface for lighting is purely
// `m.light(...)` (occasionally with `colorTemp`/`color`), so the
// generic ZCL pair `kFz{OnOff,Brightness,ColorTemperature,Color}` +
// `kTz{OnOff,Brightness,ColorTemp,Color}` covers every bulb, ceiling
// lamp, and LED strip in the catalogue. We flatten the three feature
// combinations seen in `innr.ts` to three shared bundles so future
// regenerated ports can collapse to a one-liner.
//
//   Tier         | feature set                            | bindings
//   -------------+----------------------------------------+----------
//   InnrLight    | on/off + brightness                    | 0x0006, 0x0008
//   InnrCTLight  | + color_temp (mired)                   | + 0x0300
//   InnrColorCT  | color_temp + color (xy + hs)           | + 0x0300
//
// Innr ships no colour-only-without-CT models (every "C" SKU is white
// + colour), so we omit the gledopto-style ColorLight tier.
//
// Existing per-port inline arrays under `generated/Inn_*.cpp` are left
// as-is; this header is a drop-in target for the next regeneration.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzInnrLight[];
inline constexpr std::uint8_t          kFzInnrLightCount = 2;

extern const ::zhc::TzConverter* const kTzInnrLight[];
inline constexpr std::uint8_t          kTzInnrLightCount = 2;

extern const ::zhc::Expose             kExposesInnrLight[];
inline constexpr std::uint8_t          kExposesInnrLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsInnrLight[];
inline constexpr std::uint8_t          kBindingsInnrLightCount = 2;

// ── White-ambiance / CCT (m.light({colorTemp})) ─────────────────────
extern const ::zhc::FzConverter* const kFzInnrCTLight[];
inline constexpr std::uint8_t          kFzInnrCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzInnrCTLight[];
inline constexpr std::uint8_t          kTzInnrCTLightCount = 3;

extern const ::zhc::Expose             kExposesInnrCTLight[];
inline constexpr std::uint8_t          kExposesInnrCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsInnrCTLight[];
inline constexpr std::uint8_t          kBindingsInnrCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzInnrColorCTLight[];
inline constexpr std::uint8_t          kFzInnrColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzInnrColorCTLight[];
inline constexpr std::uint8_t          kTzInnrColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesInnrColorCTLight[];
inline constexpr std::uint8_t          kExposesInnrColorCTLightCount = 5;

extern const ::zhc::BindingSpec        kBindingsInnrColorCTLight[];
inline constexpr std::uint8_t          kBindingsInnrColorCTLightCount = 3;

}  // namespace zhc::devices::innr
