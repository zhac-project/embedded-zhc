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
inline constexpr std::uint8_t          kFzSengledLightCount = 2;
extern const ::zhc::TzConverter* const kTzSengledLight[];
inline constexpr std::uint8_t          kTzSengledLightCount = 2;
extern const ::zhc::Expose             kExposesSengledLight[];
inline constexpr std::uint8_t          kExposesSengledLightCount = 2;
extern const ::zhc::BindingSpec        kBindingsSengledLight[];
inline constexpr std::uint8_t          kBindingsSengledLightCount = 2;

// ── Tunable white (sengledLight({colorTemp: {range: ...}})) ────────
extern const ::zhc::FzConverter* const kFzSengledCTLight[];
inline constexpr std::uint8_t          kFzSengledCTLightCount = 3;
extern const ::zhc::TzConverter* const kTzSengledCTLight[];
inline constexpr std::uint8_t          kTzSengledCTLightCount = 3;
extern const ::zhc::Expose             kExposesSengledCTLight[];
inline constexpr std::uint8_t          kExposesSengledCTLightCount = 3;
extern const ::zhc::BindingSpec        kBindingsSengledCTLight[];
inline constexpr std::uint8_t          kBindingsSengledCTLightCount = 3;

// ── Colour-only (sengledLight({colorTemp: undefined, color: {modes:["xy"]}})) ──
extern const ::zhc::FzConverter* const kFzSengledColorLight[];
inline constexpr std::uint8_t          kFzSengledColorLightCount = 3;
extern const ::zhc::TzConverter* const kTzSengledColorLight[];
inline constexpr std::uint8_t          kTzSengledColorLightCount = 3;
extern const ::zhc::Expose             kExposesSengledColorLight[];
inline constexpr std::uint8_t          kExposesSengledColorLightCount = 6;
extern const ::zhc::BindingSpec        kBindingsSengledColorLight[];
inline constexpr std::uint8_t          kBindingsSengledColorLightCount = 3;

// ── Full RGBW (sengledLight({colorTemp: {...}, color: {modes:["xy"]}})) ──
extern const ::zhc::FzConverter* const kFzSengledColorCTLight[];
inline constexpr std::uint8_t          kFzSengledColorCTLightCount = 4;
extern const ::zhc::TzConverter* const kTzSengledColorCTLight[];
inline constexpr std::uint8_t          kTzSengledColorCTLightCount = 4;
extern const ::zhc::Expose             kExposesSengledColorCTLight[];
inline constexpr std::uint8_t          kExposesSengledColorCTLightCount = 7;
extern const ::zhc::BindingSpec        kBindingsSengledColorCTLight[];
inline constexpr std::uint8_t          kBindingsSengledColorCTLightCount = 3;

}  // namespace zhc::devices::sengled
