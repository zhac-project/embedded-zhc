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
extern const std::uint8_t              kFzGledoptoLightCount;

extern const ::zhc::TzConverter* const kTzGledoptoLight[];
extern const std::uint8_t              kTzGledoptoLightCount;

extern const ::zhc::Expose             kExposesGledoptoLight[];
extern const std::uint8_t              kExposesGledoptoLightCount;

extern const ::zhc::BindingSpec        kBindingsGledoptoLight[];
extern const std::uint8_t              kBindingsGledoptoLightCount;

// ── White-ambiance / CCT (gledoptoLight({colorTemp})) ───────────────
extern const ::zhc::FzConverter* const kFzGledoptoCTLight[];
extern const std::uint8_t              kFzGledoptoCTLightCount;

extern const ::zhc::TzConverter* const kTzGledoptoCTLight[];
extern const std::uint8_t              kTzGledoptoCTLightCount;

extern const ::zhc::Expose             kExposesGledoptoCTLight[];
extern const std::uint8_t              kExposesGledoptoCTLightCount;

extern const ::zhc::BindingSpec        kBindingsGledoptoCTLight[];
extern const std::uint8_t              kBindingsGledoptoCTLightCount;

// ── Colour-only (gledoptoLight({color: true})) ──────────────────────
extern const ::zhc::FzConverter* const kFzGledoptoColorLight[];
extern const std::uint8_t              kFzGledoptoColorLightCount;

extern const ::zhc::TzConverter* const kTzGledoptoColorLight[];
extern const std::uint8_t              kTzGledoptoColorLightCount;

extern const ::zhc::Expose             kExposesGledoptoColorLight[];
extern const std::uint8_t              kExposesGledoptoColorLightCount;

extern const ::zhc::BindingSpec        kBindingsGledoptoColorLight[];
extern const std::uint8_t              kBindingsGledoptoColorLightCount;

// ── Full RGBW (gledoptoLight({colorTemp, color: true})) ─────────────
extern const ::zhc::FzConverter* const kFzGledoptoColorCTLight[];
extern const std::uint8_t              kFzGledoptoColorCTLightCount;

extern const ::zhc::TzConverter* const kTzGledoptoColorCTLight[];
extern const std::uint8_t              kTzGledoptoColorCTLightCount;

extern const ::zhc::Expose             kExposesGledoptoColorCTLight[];
extern const std::uint8_t              kExposesGledoptoColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsGledoptoColorCTLight[];
extern const std::uint8_t              kBindingsGledoptoColorCTLightCount;

}  // namespace zhc::devices::gledopto
