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
extern const std::uint8_t              kFzSylvaniaOnOffCount;

extern const ::zhc::TzConverter* const kTzSylvaniaOnOff[];
extern const std::uint8_t              kTzSylvaniaOnOffCount;

extern const ::zhc::Expose             kExposesSylvaniaOnOff[];
extern const std::uint8_t              kExposesSylvaniaOnOffCount;

extern const ::zhc::BindingSpec        kBindingsSylvaniaOnOff[];
extern const std::uint8_t              kBindingsSylvaniaOnOffCount;

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
extern const ::zhc::FzConverter* const kFzSylvaniaDim[];
extern const std::uint8_t              kFzSylvaniaDimCount;

extern const ::zhc::TzConverter* const kTzSylvaniaDim[];
extern const std::uint8_t              kTzSylvaniaDimCount;

extern const ::zhc::Expose             kExposesSylvaniaDim[];
extern const std::uint8_t              kExposesSylvaniaDimCount;

extern const ::zhc::BindingSpec        kBindingsSylvaniaDim[];
extern const std::uint8_t              kBindingsSylvaniaDimCount;

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
// kFzSylvaniaLight / kExposesSylvaniaLight are the historical names every
// generated port already references — keep them as the CTLight bundle.
extern const ::zhc::FzConverter* const kFzSylvaniaLight[];
extern const std::uint8_t              kFzSylvaniaLightCount;

extern const ::zhc::TzConverter* const kTzSylvaniaLight[];
extern const std::uint8_t              kTzSylvaniaLightCount;

extern const ::zhc::Expose             kExposesSylvaniaLight[];
extern const std::uint8_t              kExposesSylvaniaLightCount;

extern const ::zhc::BindingSpec        kBindingsSylvaniaLight[];
extern const std::uint8_t              kBindingsSylvaniaLightCount;

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
extern const ::zhc::FzConverter* const kFzSylvaniaColorLight[];
extern const std::uint8_t              kFzSylvaniaColorLightCount;

extern const ::zhc::TzConverter* const kTzSylvaniaColorLight[];
extern const std::uint8_t              kTzSylvaniaColorLightCount;

extern const ::zhc::Expose             kExposesSylvaniaColorLight[];
extern const std::uint8_t              kExposesSylvaniaColorLightCount;

extern const ::zhc::BindingSpec        kBindingsSylvaniaColorLight[];
extern const std::uint8_t              kBindingsSylvaniaColorLightCount;

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
extern const ::zhc::FzConverter* const kFzSylvaniaColorCTLight[];
extern const std::uint8_t              kFzSylvaniaColorCTLightCount;

extern const ::zhc::TzConverter* const kTzSylvaniaColorCTLight[];
extern const std::uint8_t              kTzSylvaniaColorCTLightCount;

extern const ::zhc::Expose             kExposesSylvaniaColorCTLight[];
extern const std::uint8_t              kExposesSylvaniaColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsSylvaniaColorCTLight[];
extern const std::uint8_t              kBindingsSylvaniaColorCTLightCount;

}  // namespace zhc::devices::sylvania
