// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Leedarson light plumbing — modelled on osram/_shared.
// Leedarson's z2m surface for lights is plain `m.light(...)` over stock
// ZCL clusters; nine of the fourteen leedarson.ts entries are
// `m.light({})`, `m.light({colorTemp})` or `m.light({colorTemp,color:true})`,
// so we flatten them to three shared bundles.
//
//   Tier         | feature set                          | bindings
//   -------------+--------------------------------------+----------------
//   Light        | on/off + brightness                  | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)                 | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)         | + 0x0300
//
// Picking the wrong bundle is harmless on-device — the extra fz/tz just
// never fire — but the exposes list shows up in the SPA, so use the
// closest match.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {

// ── Plain dimmable (m.light()) ───────────────────────────────────────
extern const ::zhc::FzConverter* const kFzLeedarsonLight[];
extern const std::uint8_t              kFzLeedarsonLightCount;

extern const ::zhc::TzConverter* const kTzLeedarsonLight[];
extern const std::uint8_t              kTzLeedarsonLightCount;

extern const ::zhc::Expose             kExposesLeedarsonLight[];
extern const std::uint8_t              kExposesLeedarsonLightCount;

extern const ::zhc::BindingSpec        kBindingsLeedarsonLight[];
extern const std::uint8_t              kBindingsLeedarsonLightCount;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzLeedarsonCTLight[];
extern const std::uint8_t              kFzLeedarsonCTLightCount;

extern const ::zhc::TzConverter* const kTzLeedarsonCTLight[];
extern const std::uint8_t              kTzLeedarsonCTLightCount;

extern const ::zhc::Expose             kExposesLeedarsonCTLight[];
extern const std::uint8_t              kExposesLeedarsonCTLightCount;

extern const ::zhc::BindingSpec        kBindingsLeedarsonCTLight[];
extern const std::uint8_t              kBindingsLeedarsonCTLightCount;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzLeedarsonColorCTLight[];
extern const std::uint8_t              kFzLeedarsonColorCTLightCount;

extern const ::zhc::TzConverter* const kTzLeedarsonColorCTLight[];
extern const std::uint8_t              kTzLeedarsonColorCTLightCount;

extern const ::zhc::Expose             kExposesLeedarsonColorCTLight[];
extern const std::uint8_t              kExposesLeedarsonColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsLeedarsonColorCTLight[];
extern const std::uint8_t              kBindingsLeedarsonColorCTLightCount;

}  // namespace zhc::devices::leedarson
