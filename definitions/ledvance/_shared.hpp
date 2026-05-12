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
extern const std::uint8_t              kFzLedvanceOnOffCount;

extern const ::zhc::TzConverter* const kTzLedvanceOnOff[];
extern const std::uint8_t              kTzLedvanceOnOffCount;

extern const ::zhc::Expose             kExposesLedvanceOnOff[];
extern const std::uint8_t              kExposesLedvanceOnOffCount;

extern const ::zhc::BindingSpec        kBindingsLedvanceOnOff[];
extern const std::uint8_t              kBindingsLedvanceOnOffCount;

// ── On/off + electricity meter (ledvanceOnOff + m.electricityMeter) ──
extern const ::zhc::FzConverter* const kFzLedvanceOnOffEM[];
extern const std::uint8_t              kFzLedvanceOnOffEMCount;

extern const ::zhc::TzConverter* const kTzLedvanceOnOffEM[];
extern const std::uint8_t              kTzLedvanceOnOffEMCount;

extern const ::zhc::Expose             kExposesLedvanceOnOffEM[];
extern const std::uint8_t              kExposesLedvanceOnOffEMCount;

extern const ::zhc::BindingSpec        kBindingsLedvanceOnOffEM[];
extern const std::uint8_t              kBindingsLedvanceOnOffEMCount;

// ── Plain dimmable (ledvanceLight({})) ───────────────────────────────
extern const ::zhc::FzConverter* const kFzLedvanceDim[];
extern const std::uint8_t              kFzLedvanceDimCount;

extern const ::zhc::TzConverter* const kTzLedvanceDim[];
extern const std::uint8_t              kTzLedvanceDimCount;

extern const ::zhc::Expose             kExposesLedvanceDim[];
extern const std::uint8_t              kExposesLedvanceDimCount;

extern const ::zhc::BindingSpec        kBindingsLedvanceDim[];
extern const std::uint8_t              kBindingsLedvanceDimCount;

// ── Dimmable + electricity meter (m.light + m.electricityMeter) ──────
extern const ::zhc::FzConverter* const kFzLedvanceDimEM[];
extern const std::uint8_t              kFzLedvanceDimEMCount;

extern const ::zhc::TzConverter* const kTzLedvanceDimEM[];
extern const std::uint8_t              kTzLedvanceDimEMCount;

extern const ::zhc::Expose             kExposesLedvanceDimEM[];
extern const std::uint8_t              kExposesLedvanceDimEMCount;

extern const ::zhc::BindingSpec        kBindingsLedvanceDimEM[];
extern const std::uint8_t              kBindingsLedvanceDimEMCount;

// ── Tunable white (ledvanceLight({colorTemp})) ───────────────────────
// kFzLedvanceLight / kExposesLedvanceLight are the historical names every
// generated port already references — keep them as the CTLight bundle.
extern const ::zhc::FzConverter* const kFzLedvanceLight[];
extern const std::uint8_t              kFzLedvanceLightCount;

extern const ::zhc::TzConverter* const kTzLedvanceLight[];
extern const std::uint8_t              kTzLedvanceLightCount;

extern const ::zhc::Expose             kExposesLedvanceLight[];
extern const std::uint8_t              kExposesLedvanceLightCount;

extern const ::zhc::BindingSpec        kBindingsLedvanceLight[];
extern const std::uint8_t              kBindingsLedvanceLightCount;

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ──────────────
extern const ::zhc::FzConverter* const kFzLedvanceColorCTLight[];
extern const std::uint8_t              kFzLedvanceColorCTLightCount;

extern const ::zhc::TzConverter* const kTzLedvanceColorCTLight[];
extern const std::uint8_t              kTzLedvanceColorCTLightCount;

extern const ::zhc::Expose             kExposesLedvanceColorCTLight[];
extern const std::uint8_t              kExposesLedvanceColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsLedvanceColorCTLight[];
extern const std::uint8_t              kBindingsLedvanceColorCTLightCount;

}  // namespace zhc::devices::ledvance
