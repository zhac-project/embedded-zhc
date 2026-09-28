// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Ecosmart light plumbing. Ecosmart's z2m surface in `ecosmart.ts`
// is exclusively `m.light(...)` — there is no Ecosmart-specific fz/tz that
// the generic ZCL pair `kFz{OnOff,Brightness,ColorTemperature,Color}` /
// `kTz{OnOff,Brightness,ColorTemp,Color}` can't already cover.
//
// We flatten the three feature combinations seen in z2m to three shared
// bundles so each generated/Eco_*.cpp can stay a one-liner.
//
//   Tier         | feature set                        | bindings
//   -------------+------------------------------------+----------
//   Light        | on/off + brightness                | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)               | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)       | + 0x0300
//
// Ecosmart has no `color: true`-only bulb in z2m (the only RGB SKU,
// D1821, sets `colorTemp: {range: undefined}, color: true`, so it
// belongs in the ColorCTLight tier). Picking the wrong bundle is
// harmless on-device — the extra fz/tz simply never fire — but the
// exposes list shows up in the SPA, so use the closest match.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecosmart {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzEcosmartLight[];
inline constexpr std::uint8_t          kFzEcosmartLightCount = 2;

extern const ::zhc::TzConverter* const kTzEcosmartLight[];
inline constexpr std::uint8_t          kTzEcosmartLightCount = 2;

extern const ::zhc::Expose             kExposesEcosmartLight[];
inline constexpr std::uint8_t          kExposesEcosmartLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsEcosmartLight[];
inline constexpr std::uint8_t          kBindingsEcosmartLightCount = 2;

// ── White-ambiance / CCT (m.light({colorTemp: ...})) ────────────────
extern const ::zhc::FzConverter* const kFzEcosmartCTLight[];
inline constexpr std::uint8_t          kFzEcosmartCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzEcosmartCTLight[];
inline constexpr std::uint8_t          kTzEcosmartCTLightCount = 3;

extern const ::zhc::Expose             kExposesEcosmartCTLight[];
inline constexpr std::uint8_t          kExposesEcosmartCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsEcosmartCTLight[];
inline constexpr std::uint8_t          kBindingsEcosmartCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzEcosmartColorCTLight[];
inline constexpr std::uint8_t          kFzEcosmartColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzEcosmartColorCTLight[];
inline constexpr std::uint8_t          kTzEcosmartColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesEcosmartColorCTLight[];
inline constexpr std::uint8_t          kExposesEcosmartColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsEcosmartColorCTLight[];
inline constexpr std::uint8_t          kBindingsEcosmartColorCTLightCount = 3;

}  // namespace zhc::devices::ecosmart
