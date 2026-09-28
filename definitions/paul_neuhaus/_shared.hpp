// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Paul Neuhaus light plumbing — modelled after paulmann/_shared.
// Paul Neuhaus's z2m surface for the Q-line is exclusively `m.light(...)`
// over stock ZCL clusters (genOnOff, genLevelCtrl, lightingColorCtrl).
// No vendor cluster, no custom converters. Three bundles cover every
// `m.light(...)` shape seen in paul_neuhaus.ts:
//
//   Tier         | feature set                            | bindings
//   -------------+----------------------------------------+----------------
//   Light        | on/off + brightness                    | 0x0006, 0x0008  (no z2m record uses this — kept for symmetry)
//   CTLight      | + color_temp (mired)                   | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)           | + 0x0300
//
// Paul Neuhaus has no `color: true`-only bulb in z2m — every RGB SKU
// also has `colorTemp`, so the `ColorLight` (non-CT) tier is omitted.
// Plain `m.light()` (no colorTemp, no color) is also unused on the
// vendor's lighting line. Bundles ship with default endpoint 1; ports
// that override (`endpoint: () => ({default: 2})`, e.g. 100.075.74,
// NLG-RGBW_light) must declare bindings inline.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paul_neuhaus {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzPaulNeuhausLight[];
inline constexpr std::uint8_t          kFzPaulNeuhausLightCount = 2;

extern const ::zhc::TzConverter* const kTzPaulNeuhausLight[];
inline constexpr std::uint8_t          kTzPaulNeuhausLightCount = 2;

extern const ::zhc::Expose             kExposesPaulNeuhausLight[];
inline constexpr std::uint8_t          kExposesPaulNeuhausLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsPaulNeuhausLight[];
inline constexpr std::uint8_t          kBindingsPaulNeuhausLightCount = 2;

// ── White-ambiance / CCT (m.light({colorTemp})) ─────────────────────
extern const ::zhc::FzConverter* const kFzPaulNeuhausCTLight[];
inline constexpr std::uint8_t          kFzPaulNeuhausCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzPaulNeuhausCTLight[];
inline constexpr std::uint8_t          kTzPaulNeuhausCTLightCount = 3;

extern const ::zhc::Expose             kExposesPaulNeuhausCTLight[];
inline constexpr std::uint8_t          kExposesPaulNeuhausCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsPaulNeuhausCTLight[];
inline constexpr std::uint8_t          kBindingsPaulNeuhausCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzPaulNeuhausColorCTLight[];
inline constexpr std::uint8_t          kFzPaulNeuhausColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzPaulNeuhausColorCTLight[];
inline constexpr std::uint8_t          kTzPaulNeuhausColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesPaulNeuhausColorCTLight[];
inline constexpr std::uint8_t          kExposesPaulNeuhausColorCTLightCount = 5;

extern const ::zhc::BindingSpec        kBindingsPaulNeuhausColorCTLight[];
inline constexpr std::uint8_t          kBindingsPaulNeuhausColorCTLightCount = 3;

}  // namespace zhc::devices::paul_neuhaus
