// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Iluminize plumbing — modelled on osram/_shared.
//
// Iluminize's z2m surface is almost entirely stock `m.light({...})` and
// `m.onOff()` over generic clusters. The only vendor-specific extras
// (`sunricher.extend.externalSwitchType` / `minimumPWM` on 5715/5717,
// the `command_*` action remotes, and the cover at 5128.10) keep
// hand-rolled ports.
//
// Five shared bundles cover the m.light()/m.onOff() permutations seen
// in iluminize.ts. Choose the tightest bundle so the SPA expose list
// stays honest. Promote a port to a non-generated file for per-device
// overrides.
//
//   Tier         | feature set                          | bindings
//   -------------+--------------------------------------+----------------
//   OnOff        | m.onOff() — switches/actuators        | 0x0006
//   Light        | m.light({}) — plain dimmable          | 0x0006, 0x0008
//   CTLight      | + colorTemp (mired)                   | + 0x0300
//   ColorLight   | + color (xy + hs), no CT              | + 0x0300
//   ColorCTLight | + colorTemp + color (xy + hs)         | + 0x0300

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {

// ── On/off only (m.onOff()) — switches/actuators ────────────────────
extern const ::zhc::FzConverter* const kFzIluOnOff[];
inline constexpr std::uint8_t          kFzIluOnOffCount = 1;

extern const ::zhc::TzConverter* const kTzIluOnOff[];
inline constexpr std::uint8_t          kTzIluOnOffCount = 1;

extern const ::zhc::Expose             kExposesIluOnOff[];
inline constexpr std::uint8_t          kExposesIluOnOffCount = 1;

extern const ::zhc::BindingSpec        kBindingsIluOnOff[];
inline constexpr std::uint8_t          kBindingsIluOnOffCount = 1;

// ── Plain dimmable (m.light({})) ────────────────────────────────────
extern const ::zhc::FzConverter* const kFzIluLight[];
inline constexpr std::uint8_t          kFzIluLightCount = 2;

extern const ::zhc::TzConverter* const kTzIluLight[];
inline constexpr std::uint8_t          kTzIluLightCount = 2;

extern const ::zhc::Expose             kExposesIluLight[];
inline constexpr std::uint8_t          kExposesIluLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsIluLight[];
inline constexpr std::uint8_t          kBindingsIluLightCount = 2;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzIluCTLight[];
inline constexpr std::uint8_t          kFzIluCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzIluCTLight[];
inline constexpr std::uint8_t          kTzIluCTLightCount = 3;

extern const ::zhc::Expose             kExposesIluCTLight[];
inline constexpr std::uint8_t          kExposesIluCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsIluCTLight[];
inline constexpr std::uint8_t          kBindingsIluCTLightCount = 3;

// ── Colour-only (m.light({color: true})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzIluColorLight[];
inline constexpr std::uint8_t          kFzIluColorLightCount = 3;

extern const ::zhc::TzConverter* const kTzIluColorLight[];
inline constexpr std::uint8_t          kTzIluColorLightCount = 3;

extern const ::zhc::Expose             kExposesIluColorLight[];
inline constexpr std::uint8_t          kExposesIluColorLightCount = 6;

extern const ::zhc::BindingSpec        kBindingsIluColorLight[];
inline constexpr std::uint8_t          kBindingsIluColorLightCount = 3;

// ── Full RGBCCT (m.light({colorTemp, color: true})) ─────────────────
extern const ::zhc::FzConverter* const kFzIluColorCTLight[];
inline constexpr std::uint8_t          kFzIluColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzIluColorCTLight[];
inline constexpr std::uint8_t          kTzIluColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesIluColorCTLight[];
inline constexpr std::uint8_t          kExposesIluColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsIluColorCTLight[];
inline constexpr std::uint8_t          kBindingsIluColorCTLightCount = 3;

// ── Cover-via-brightness (5128.10 roller-shutter relay) ─────────────
//
// z2m's 5128.10 reports position over `genLevelCtrl.currentLevel`
// (cover_position_via_brightness) and OPEN/CLOSE over `genOnOff.onOff`
// (cover_state_via_onoff); the generic `kFzCoverPosition` only decodes
// the `closuresWindowCovering` lift attribute, so a real device's
// reports decode to nothing. These two add the missing legs.

// genLevelCtrl currentLevel attr 0x0000 (u8 0..255) → "position"
// (Uint 0..100) + "state" (StringRef "OPEN"/"CLOSE", >0 ⇒ OPEN).
// Mirrors z2m fz.cover_position_via_brightness.
extern const ::zhc::FzConverter kFzIluCoverViaBrightness;

// genOnOff onOff attr 0x0000 → "state" ("OPEN" when 1 else "CLOSE").
// Mirrors z2m fz.cover_state_via_onoff.
extern const ::zhc::FzConverter kFzIluCoverStateViaOnOff;

// Accepts "position" (Uint 0..100) → genLevelCtrl moveToLevelWithOnOff
// (cmd 0x04). Mirrors z2m tz.cover_via_brightness (position leg).
extern const ::zhc::TzConverter kTzIluCoverViaBrightness;

}  // namespace zhc::devices::iluminize
