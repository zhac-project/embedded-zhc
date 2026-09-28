// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Gledopto light plumbing. Gledopto's z2m surface is exclusively
// `m.light(...)` / `gledoptoLight(...)` — there is no Gledopto-specific
// fz/tz that the generic ZCL pair `kFz{OnOff,Brightness,ColorTemperature,
// Color}` + `kTz{OnOff,Brightness,ColorTemp,Color}` can't already cover.
// We flatten the four feature combinations seen in `gledopto.ts` to four
// shared bundles so each generated/Gle_*.cpp can stay a one-liner.
//
//   Tier        | feature set                        | bindings
//   ------------+------------------------------------+----------
//   Light       | on/off + brightness                | 0x0006, 0x0008
//   CTLight     | + color_temp (mired)               | + 0x0300
//   ColorLight  | + color_x/y + hue + saturation     | + 0x0300
//   ColorCTLight| color_temp + color (xy + hs)       | + 0x0300
//
// Picking the wrong bundle is harmless on-device — the extra fz/tz just
// never fire — but the exposes list shows up in the SPA, so use the
// closest match. For a leaner expose set per device, promote the port
// to a per-device override in `definitions/gledopto/` (non-generated).

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gledopto {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzGledoptoLight[];
inline constexpr std::uint8_t          kFzGledoptoLightCount = 2;

extern const ::zhc::TzConverter* const kTzGledoptoLight[];
inline constexpr std::uint8_t          kTzGledoptoLightCount = 2;

extern const ::zhc::Expose             kExposesGledoptoLight[];
inline constexpr std::uint8_t          kExposesGledoptoLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsGledoptoLight[];
inline constexpr std::uint8_t          kBindingsGledoptoLightCount = 2;

// ── White-ambiance / CCT (gledoptoLight({colorTemp})) ───────────────
extern const ::zhc::FzConverter* const kFzGledoptoCTLight[];
inline constexpr std::uint8_t          kFzGledoptoCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzGledoptoCTLight[];
inline constexpr std::uint8_t          kTzGledoptoCTLightCount = 3;

extern const ::zhc::Expose             kExposesGledoptoCTLight[];
inline constexpr std::uint8_t          kExposesGledoptoCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsGledoptoCTLight[];
inline constexpr std::uint8_t          kBindingsGledoptoCTLightCount = 3;

// ── Colour-only (gledoptoLight({color: true})) ──────────────────────
extern const ::zhc::FzConverter* const kFzGledoptoColorLight[];
inline constexpr std::uint8_t          kFzGledoptoColorLightCount = 3;

extern const ::zhc::TzConverter* const kTzGledoptoColorLight[];
inline constexpr std::uint8_t          kTzGledoptoColorLightCount = 3;

extern const ::zhc::Expose             kExposesGledoptoColorLight[];
inline constexpr std::uint8_t          kExposesGledoptoColorLightCount = 6;

extern const ::zhc::BindingSpec        kBindingsGledoptoColorLight[];
inline constexpr std::uint8_t          kBindingsGledoptoColorLightCount = 3;

// ── Full RGBW (gledoptoLight({colorTemp, color: true})) ─────────────
extern const ::zhc::FzConverter* const kFzGledoptoColorCTLight[];
inline constexpr std::uint8_t          kFzGledoptoColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzGledoptoColorCTLight[];
inline constexpr std::uint8_t          kTzGledoptoColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesGledoptoColorCTLight[];
inline constexpr std::uint8_t          kExposesGledoptoColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsGledoptoColorCTLight[];
inline constexpr std::uint8_t          kBindingsGledoptoColorCTLightCount = 3;

}  // namespace zhc::devices::gledopto
