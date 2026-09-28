// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Sylvania light/plug plumbing — modelled on osram/_shared.
// Sylvania's z2m surface is `ledvanceLight(...)` (`lib/ledvance.ts` —
// same family as osram + ledvance) and `ledvanceOnOff(...)` over stock
// ZCL clusters. Five bundles cover every feature combination seen in
// z2m's sylvania.ts.
//
//   Tier         | feature set                            | bindings
//   -------------+----------------------------------------+----------------------
//   OnOff        | on/off only (ledvanceOnOff plug)       | 0x0006
//   Dim          | + brightness (ledvanceLight({}))       | + 0x0008
//   CTLight      | + color_temp (mired)                   | + 0x0300
//   ColorLight   | + color (xy + hs), no CT               | (Dim + 0x0300)
//   ColorCTLight | + color_temp + color (xy + hs)         | (Dim + 0x0300)
//
// The legacy `kFzSylvaniaLight` / `kExposesSylvaniaLight` /
// `kBindingsSylvaniaLight` symbol names are kept as aliases for
// CTLight (every pre-sweep generated port already references them).

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sylvania {

// ── On/off only (ledvanceOnOff) — Sylvania smart plugs ──────────────
extern const ::zhc::FzConverter* const kFzSylvaniaOnOff[];
inline constexpr std::uint8_t          kFzSylvaniaOnOffCount = 1;

extern const ::zhc::TzConverter* const kTzSylvaniaOnOff[];
inline constexpr std::uint8_t          kTzSylvaniaOnOffCount = 1;

extern const ::zhc::Expose             kExposesSylvaniaOnOff[];
inline constexpr std::uint8_t          kExposesSylvaniaOnOffCount = 1;

extern const ::zhc::BindingSpec        kBindingsSylvaniaOnOff[];
inline constexpr std::uint8_t          kBindingsSylvaniaOnOffCount = 1;

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
extern const ::zhc::FzConverter* const kFzSylvaniaDim[];
inline constexpr std::uint8_t          kFzSylvaniaDimCount = 2;

extern const ::zhc::TzConverter* const kTzSylvaniaDim[];
inline constexpr std::uint8_t          kTzSylvaniaDimCount = 2;

extern const ::zhc::Expose             kExposesSylvaniaDim[];
inline constexpr std::uint8_t          kExposesSylvaniaDimCount = 2;

extern const ::zhc::BindingSpec        kBindingsSylvaniaDim[];
inline constexpr std::uint8_t          kBindingsSylvaniaDimCount = 2;

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
// kFzSylvaniaLight / kExposesSylvaniaLight are the historical names every
// generated port already references — keep them as the CTLight bundle.
extern const ::zhc::FzConverter* const kFzSylvaniaLight[];
inline constexpr std::uint8_t          kFzSylvaniaLightCount = 3;

extern const ::zhc::TzConverter* const kTzSylvaniaLight[];
inline constexpr std::uint8_t          kTzSylvaniaLightCount = 3;

extern const ::zhc::Expose             kExposesSylvaniaLight[];
inline constexpr std::uint8_t          kExposesSylvaniaLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsSylvaniaLight[];
inline constexpr std::uint8_t          kBindingsSylvaniaLightCount = 3;

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
extern const ::zhc::FzConverter* const kFzSylvaniaColorLight[];
inline constexpr std::uint8_t          kFzSylvaniaColorLightCount = 3;

extern const ::zhc::TzConverter* const kTzSylvaniaColorLight[];
inline constexpr std::uint8_t          kTzSylvaniaColorLightCount = 3;

extern const ::zhc::Expose             kExposesSylvaniaColorLight[];
inline constexpr std::uint8_t          kExposesSylvaniaColorLightCount = 6;

extern const ::zhc::BindingSpec        kBindingsSylvaniaColorLight[];
inline constexpr std::uint8_t          kBindingsSylvaniaColorLightCount = 3;

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
extern const ::zhc::FzConverter* const kFzSylvaniaColorCTLight[];
inline constexpr std::uint8_t          kFzSylvaniaColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzSylvaniaColorCTLight[];
inline constexpr std::uint8_t          kTzSylvaniaColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesSylvaniaColorCTLight[];
inline constexpr std::uint8_t          kExposesSylvaniaColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsSylvaniaColorCTLight[];
inline constexpr std::uint8_t          kBindingsSylvaniaColorCTLightCount = 3;

}  // namespace zhc::devices::sylvania
