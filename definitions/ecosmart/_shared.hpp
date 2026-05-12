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
extern const std::uint8_t              kFzEcosmartLightCount;

extern const ::zhc::TzConverter* const kTzEcosmartLight[];
extern const std::uint8_t              kTzEcosmartLightCount;

extern const ::zhc::Expose             kExposesEcosmartLight[];
extern const std::uint8_t              kExposesEcosmartLightCount;

extern const ::zhc::BindingSpec        kBindingsEcosmartLight[];
extern const std::uint8_t              kBindingsEcosmartLightCount;

// ── White-ambiance / CCT (m.light({colorTemp: ...})) ────────────────
extern const ::zhc::FzConverter* const kFzEcosmartCTLight[];
extern const std::uint8_t              kFzEcosmartCTLightCount;

extern const ::zhc::TzConverter* const kTzEcosmartCTLight[];
extern const std::uint8_t              kTzEcosmartCTLightCount;

extern const ::zhc::Expose             kExposesEcosmartCTLight[];
extern const std::uint8_t              kExposesEcosmartCTLightCount;

extern const ::zhc::BindingSpec        kBindingsEcosmartCTLight[];
extern const std::uint8_t              kBindingsEcosmartCTLightCount;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzEcosmartColorCTLight[];
extern const std::uint8_t              kFzEcosmartColorCTLightCount;

extern const ::zhc::TzConverter* const kTzEcosmartColorCTLight[];
extern const std::uint8_t              kTzEcosmartColorCTLightCount;

extern const ::zhc::Expose             kExposesEcosmartColorCTLight[];
extern const std::uint8_t              kExposesEcosmartColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsEcosmartColorCTLight[];
extern const std::uint8_t              kBindingsEcosmartColorCTLightCount;

}  // namespace zhc::devices::ecosmart
