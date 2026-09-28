// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Ledvance light/plug plumbing — modelled on osram/_shared.
// Ledvance's z2m surface is `ledvanceLight(...)`, `ledvanceOnOff(...)`,
// `m.electricityMeter()` and `m.light(...)` over stock ZCL clusters.
//
// Six bundles cover every feature combination seen in z2m's ledvance.ts.
//
//   Tier         | feature set                               | bindings
//   -------------+-------------------------------------------+--------------------------
//   OnOff        | on/off only (plain plug, T8 ballast)      | 0x0006
//   OnOffEM      | on/off + energy + power                   | 0x0006, 0x0702, 0x0B04
//   Light        | on/off + brightness                       | 0x0006, 0x0008
//   LightEM      | on/off + brightness + energy + power      | 0x0006, 0x0008, 0x0702, 0x0B04
//   CTLight      | + color_temp (mired)                      | + 0x0300
//   ColorCTLight | + color (xy + hs)                         | + 0x0300

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ledvance {

// ── On/off only (ledvanceOnOff()) — Ledvance smart plugs / T8 tubes ──
extern const ::zhc::FzConverter* const kFzLedvanceOnOff[];
inline constexpr std::uint8_t          kFzLedvanceOnOffCount = 1;

extern const ::zhc::TzConverter* const kTzLedvanceOnOff[];
inline constexpr std::uint8_t          kTzLedvanceOnOffCount = 1;

extern const ::zhc::Expose             kExposesLedvanceOnOff[];
inline constexpr std::uint8_t          kExposesLedvanceOnOffCount = 1;

extern const ::zhc::BindingSpec        kBindingsLedvanceOnOff[];
inline constexpr std::uint8_t          kBindingsLedvanceOnOffCount = 1;

// ── On/off + electricity meter (ledvanceOnOff + m.electricityMeter) ──
extern const ::zhc::FzConverter* const kFzLedvanceOnOffEM[];
inline constexpr std::uint8_t          kFzLedvanceOnOffEMCount = 3;

extern const ::zhc::TzConverter* const kTzLedvanceOnOffEM[];
inline constexpr std::uint8_t          kTzLedvanceOnOffEMCount = 1;

extern const ::zhc::Expose             kExposesLedvanceOnOffEM[];
inline constexpr std::uint8_t          kExposesLedvanceOnOffEMCount = 5;

extern const ::zhc::BindingSpec        kBindingsLedvanceOnOffEM[];
inline constexpr std::uint8_t          kBindingsLedvanceOnOffEMCount = 3;

// ── Plain dimmable (ledvanceLight({})) ───────────────────────────────
extern const ::zhc::FzConverter* const kFzLedvanceDim[];
inline constexpr std::uint8_t          kFzLedvanceDimCount = 2;

extern const ::zhc::TzConverter* const kTzLedvanceDim[];
inline constexpr std::uint8_t          kTzLedvanceDimCount = 2;

extern const ::zhc::Expose             kExposesLedvanceDim[];
inline constexpr std::uint8_t          kExposesLedvanceDimCount = 2;

extern const ::zhc::BindingSpec        kBindingsLedvanceDim[];
inline constexpr std::uint8_t          kBindingsLedvanceDimCount = 2;

// ── Dimmable + electricity meter (m.light + m.electricityMeter) ──────
extern const ::zhc::FzConverter* const kFzLedvanceDimEM[];
inline constexpr std::uint8_t          kFzLedvanceDimEMCount = 4;

extern const ::zhc::TzConverter* const kTzLedvanceDimEM[];
inline constexpr std::uint8_t          kTzLedvanceDimEMCount = 2;

extern const ::zhc::Expose             kExposesLedvanceDimEM[];
inline constexpr std::uint8_t          kExposesLedvanceDimEMCount = 6;

extern const ::zhc::BindingSpec        kBindingsLedvanceDimEM[];
inline constexpr std::uint8_t          kBindingsLedvanceDimEMCount = 4;

// ── Tunable white (ledvanceLight({colorTemp})) ───────────────────────
// kFzLedvanceLight / kExposesLedvanceLight are the historical names every
// generated port already references — keep them as the CTLight bundle.
extern const ::zhc::FzConverter* const kFzLedvanceLight[];
inline constexpr std::uint8_t          kFzLedvanceLightCount = 3;

extern const ::zhc::TzConverter* const kTzLedvanceLight[];
inline constexpr std::uint8_t          kTzLedvanceLightCount = 3;

extern const ::zhc::Expose             kExposesLedvanceLight[];
inline constexpr std::uint8_t          kExposesLedvanceLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsLedvanceLight[];
inline constexpr std::uint8_t          kBindingsLedvanceLightCount = 3;

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ──────────────
extern const ::zhc::FzConverter* const kFzLedvanceColorCTLight[];
inline constexpr std::uint8_t          kFzLedvanceColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzLedvanceColorCTLight[];
inline constexpr std::uint8_t          kTzLedvanceColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesLedvanceColorCTLight[];
inline constexpr std::uint8_t          kExposesLedvanceColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsLedvanceColorCTLight[];
inline constexpr std::uint8_t          kBindingsLedvanceColorCTLightCount = 3;

}  // namespace zhc::devices::ledvance
