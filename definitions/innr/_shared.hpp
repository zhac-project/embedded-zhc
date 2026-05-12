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
extern const std::uint8_t              kFzInnrLightCount;

extern const ::zhc::TzConverter* const kTzInnrLight[];
extern const std::uint8_t              kTzInnrLightCount;

extern const ::zhc::Expose             kExposesInnrLight[];
extern const std::uint8_t              kExposesInnrLightCount;

extern const ::zhc::BindingSpec        kBindingsInnrLight[];
extern const std::uint8_t              kBindingsInnrLightCount;

// ── White-ambiance / CCT (m.light({colorTemp})) ─────────────────────
extern const ::zhc::FzConverter* const kFzInnrCTLight[];
extern const std::uint8_t              kFzInnrCTLightCount;

extern const ::zhc::TzConverter* const kTzInnrCTLight[];
extern const std::uint8_t              kTzInnrCTLightCount;

extern const ::zhc::Expose             kExposesInnrCTLight[];
extern const std::uint8_t              kExposesInnrCTLightCount;

extern const ::zhc::BindingSpec        kBindingsInnrCTLight[];
extern const std::uint8_t              kBindingsInnrCTLightCount;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzInnrColorCTLight[];
extern const std::uint8_t              kFzInnrColorCTLightCount;

extern const ::zhc::TzConverter* const kTzInnrColorCTLight[];
extern const std::uint8_t              kTzInnrColorCTLightCount;

extern const ::zhc::Expose             kExposesInnrColorCTLight[];
extern const std::uint8_t              kExposesInnrColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsInnrColorCTLight[];
extern const std::uint8_t              kBindingsInnrColorCTLightCount;

}  // namespace zhc::devices::innr
