// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Sengled light bundles — modeled on osram/_shared.
//
// `sengledLight()` in z2m wraps `m.light({effect:false, powerOnBehavior:false, ...})`.
// `m.light()` itself defaults to *no* color and *no* colorTemp — only on/off + brightness.
// Color/colorTemp are opt-in via args, so the four bundles below cover every
// combination seen in z2m's sengled.ts.
//
//   Tier         | feature set                          | bindings
//   -------------+--------------------------------------+----------------
//   Light        | on/off + brightness                  | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)                 | + 0x0300
//   ColorLight   | + color (xy + hs), no CT             | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)         | + 0x0300
//
// NOTE: prior to 2026-04, a single `kFzSengledLight` bundle (Light + CT)
// was applied to every model that mentioned `sengledLight(...)` in z2m.
// That mapping was wrong for both bare `sengledLight()` calls (no CT) and
// for `color: {modes:["xy"]}` calls (no color/hue/saturation exposes).
// See docs/SENGLED_PARITY.md.

#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sengled {

// ── Plain dimmable (sengledLight() / sengledLight({color: false})) ──
extern const ::zhc::FzConverter* const kFzSengledLight[];
extern const std::uint8_t              kFzSengledLightCount;
extern const ::zhc::TzConverter* const kTzSengledLight[];
extern const std::uint8_t              kTzSengledLightCount;
extern const ::zhc::Expose             kExposesSengledLight[];
extern const std::uint8_t              kExposesSengledLightCount;
extern const ::zhc::BindingSpec        kBindingsSengledLight[];
extern const std::uint8_t              kBindingsSengledLightCount;

// ── Tunable white (sengledLight({colorTemp: {range: ...}})) ────────
extern const ::zhc::FzConverter* const kFzSengledCTLight[];
extern const std::uint8_t              kFzSengledCTLightCount;
extern const ::zhc::TzConverter* const kTzSengledCTLight[];
extern const std::uint8_t              kTzSengledCTLightCount;
extern const ::zhc::Expose             kExposesSengledCTLight[];
extern const std::uint8_t              kExposesSengledCTLightCount;
extern const ::zhc::BindingSpec        kBindingsSengledCTLight[];
extern const std::uint8_t              kBindingsSengledCTLightCount;

// ── Colour-only (sengledLight({colorTemp: undefined, color: {modes:["xy"]}})) ──
extern const ::zhc::FzConverter* const kFzSengledColorLight[];
extern const std::uint8_t              kFzSengledColorLightCount;
extern const ::zhc::TzConverter* const kTzSengledColorLight[];
extern const std::uint8_t              kTzSengledColorLightCount;
extern const ::zhc::Expose             kExposesSengledColorLight[];
extern const std::uint8_t              kExposesSengledColorLightCount;
extern const ::zhc::BindingSpec        kBindingsSengledColorLight[];
extern const std::uint8_t              kBindingsSengledColorLightCount;

// ── Full RGBW (sengledLight({colorTemp: {...}, color: {modes:["xy"]}})) ──
extern const ::zhc::FzConverter* const kFzSengledColorCTLight[];
extern const std::uint8_t              kFzSengledColorCTLightCount;
extern const ::zhc::TzConverter* const kTzSengledColorCTLight[];
extern const std::uint8_t              kTzSengledColorCTLightCount;
extern const ::zhc::Expose             kExposesSengledColorCTLight[];
extern const std::uint8_t              kExposesSengledColorCTLightCount;
extern const ::zhc::BindingSpec        kBindingsSengledColorCTLight[];
extern const std::uint8_t              kBindingsSengledColorCTLightCount;

}  // namespace zhc::devices::sengled
