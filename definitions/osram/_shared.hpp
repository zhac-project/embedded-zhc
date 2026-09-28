// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Osram (LEDVANCE) light/plug plumbing — modeled on gledopto/_shared.
// Osram's z2m surface is 99 % `ledvanceLight(...)` / `ledvanceOnOff(...)` over
// stock ZCL clusters; the LEDVANCE-specific bits (`manuSpecificOsram` 0xfc0f
// custom commands and the `pbc_level_to_action` fz) only fire on three
// vendor-specific extras (PBC, Switch Mini, Switch 4x) which keep their own
// hand-rolled ports.
//
// Five shared bundles cover the feature combinations seen in z2m's osram.ts.
// Picking a tighter bundle keeps the SPA expose list honest. For a leaner
// per-device override, promote the port to a non-generated file in
// `definitions/osram/`.
//
//   Tier        | feature set                          | bindings
//   ------------+--------------------------------------+----------------
//   OnOff       | on/off only (plug)                   | 0x0006
//   Light       | on/off + brightness                  | 0x0006, 0x0008
//   CTLight     | + color_temp (mired)                 | + 0x0300
//   ColorLight  | + color (xy + hs), no CT             | + 0x0300
//   ColorCTLight| color_temp + color (xy + hs)         | + 0x0300

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::osram {

// ── On/off only (ledvanceOnOff) — Osram smart plugs ─────────────────
extern const ::zhc::FzConverter* const kFzOsramOnOff[];
inline constexpr std::uint8_t          kFzOsramOnOffCount = 1;

extern const ::zhc::TzConverter* const kTzOsramOnOff[];
inline constexpr std::uint8_t          kTzOsramOnOffCount = 1;

extern const ::zhc::Expose             kExposesOsramOnOff[];
inline constexpr std::uint8_t          kExposesOsramOnOffCount = 1;

extern const ::zhc::BindingSpec        kBindingsOsramOnOff[];
inline constexpr std::uint8_t          kBindingsOsramOnOffCount = 1;

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
extern const ::zhc::FzConverter* const kFzOsramLight[];
inline constexpr std::uint8_t          kFzOsramLightCount = 2;

extern const ::zhc::TzConverter* const kTzOsramLight[];
inline constexpr std::uint8_t          kTzOsramLightCount = 2;

extern const ::zhc::Expose             kExposesOsramLight[];
inline constexpr std::uint8_t          kExposesOsramLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsOsramLight[];
inline constexpr std::uint8_t          kBindingsOsramLightCount = 2;

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
extern const ::zhc::FzConverter* const kFzOsramCTLight[];
inline constexpr std::uint8_t          kFzOsramCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzOsramCTLight[];
inline constexpr std::uint8_t          kTzOsramCTLightCount = 3;

extern const ::zhc::Expose             kExposesOsramCTLight[];
inline constexpr std::uint8_t          kExposesOsramCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsOsramCTLight[];
inline constexpr std::uint8_t          kBindingsOsramCTLightCount = 3;

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
extern const ::zhc::FzConverter* const kFzOsramColorLight[];
inline constexpr std::uint8_t          kFzOsramColorLightCount = 3;

extern const ::zhc::TzConverter* const kTzOsramColorLight[];
inline constexpr std::uint8_t          kTzOsramColorLightCount = 3;

extern const ::zhc::Expose             kExposesOsramColorLight[];
inline constexpr std::uint8_t          kExposesOsramColorLightCount = 6;

extern const ::zhc::BindingSpec        kBindingsOsramColorLight[];
inline constexpr std::uint8_t          kBindingsOsramColorLightCount = 3;

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
extern const ::zhc::FzConverter* const kFzOsramColorCTLight[];
inline constexpr std::uint8_t          kFzOsramColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzOsramColorCTLight[];
inline constexpr std::uint8_t          kTzOsramColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesOsramColorCTLight[];
inline constexpr std::uint8_t          kExposesOsramColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsOsramColorCTLight[];
inline constexpr std::uint8_t          kBindingsOsramColorCTLightCount = 3;

}  // namespace zhc::devices::osram
