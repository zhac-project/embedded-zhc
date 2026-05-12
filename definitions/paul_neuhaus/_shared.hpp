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
extern const std::uint8_t              kFzPaulNeuhausLightCount;

extern const ::zhc::TzConverter* const kTzPaulNeuhausLight[];
extern const std::uint8_t              kTzPaulNeuhausLightCount;

extern const ::zhc::Expose             kExposesPaulNeuhausLight[];
extern const std::uint8_t              kExposesPaulNeuhausLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulNeuhausLight[];
extern const std::uint8_t              kBindingsPaulNeuhausLightCount;

// ── White-ambiance / CCT (m.light({colorTemp})) ─────────────────────
extern const ::zhc::FzConverter* const kFzPaulNeuhausCTLight[];
extern const std::uint8_t              kFzPaulNeuhausCTLightCount;

extern const ::zhc::TzConverter* const kTzPaulNeuhausCTLight[];
extern const std::uint8_t              kTzPaulNeuhausCTLightCount;

extern const ::zhc::Expose             kExposesPaulNeuhausCTLight[];
extern const std::uint8_t              kExposesPaulNeuhausCTLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulNeuhausCTLight[];
extern const std::uint8_t              kBindingsPaulNeuhausCTLightCount;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzPaulNeuhausColorCTLight[];
extern const std::uint8_t              kFzPaulNeuhausColorCTLightCount;

extern const ::zhc::TzConverter* const kTzPaulNeuhausColorCTLight[];
extern const std::uint8_t              kTzPaulNeuhausColorCTLightCount;

extern const ::zhc::Expose             kExposesPaulNeuhausColorCTLight[];
extern const std::uint8_t              kExposesPaulNeuhausColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulNeuhausColorCTLight[];
extern const std::uint8_t              kBindingsPaulNeuhausColorCTLightCount;

}  // namespace zhc::devices::paul_neuhaus
