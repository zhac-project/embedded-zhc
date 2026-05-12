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
extern const std::uint8_t              kFzOsramOnOffCount;

extern const ::zhc::TzConverter* const kTzOsramOnOff[];
extern const std::uint8_t              kTzOsramOnOffCount;

extern const ::zhc::Expose             kExposesOsramOnOff[];
extern const std::uint8_t              kExposesOsramOnOffCount;

extern const ::zhc::BindingSpec        kBindingsOsramOnOff[];
extern const std::uint8_t              kBindingsOsramOnOffCount;

// ── Plain dimmable (ledvanceLight({})) ──────────────────────────────
extern const ::zhc::FzConverter* const kFzOsramLight[];
extern const std::uint8_t              kFzOsramLightCount;

extern const ::zhc::TzConverter* const kTzOsramLight[];
extern const std::uint8_t              kTzOsramLightCount;

extern const ::zhc::Expose             kExposesOsramLight[];
extern const std::uint8_t              kExposesOsramLightCount;

extern const ::zhc::BindingSpec        kBindingsOsramLight[];
extern const std::uint8_t              kBindingsOsramLightCount;

// ── Tunable white (ledvanceLight({colorTemp})) ──────────────────────
extern const ::zhc::FzConverter* const kFzOsramCTLight[];
extern const std::uint8_t              kFzOsramCTLightCount;

extern const ::zhc::TzConverter* const kTzOsramCTLight[];
extern const std::uint8_t              kTzOsramCTLightCount;

extern const ::zhc::Expose             kExposesOsramCTLight[];
extern const std::uint8_t              kExposesOsramCTLightCount;

extern const ::zhc::BindingSpec        kBindingsOsramCTLight[];
extern const std::uint8_t              kBindingsOsramCTLightCount;

// ── Colour-only (ledvanceLight({color: true})) ──────────────────────
extern const ::zhc::FzConverter* const kFzOsramColorLight[];
extern const std::uint8_t              kFzOsramColorLightCount;

extern const ::zhc::TzConverter* const kTzOsramColorLight[];
extern const std::uint8_t              kTzOsramColorLightCount;

extern const ::zhc::Expose             kExposesOsramColorLight[];
extern const std::uint8_t              kExposesOsramColorLightCount;

extern const ::zhc::BindingSpec        kBindingsOsramColorLight[];
extern const std::uint8_t              kBindingsOsramColorLightCount;

// ── Full RGBW (ledvanceLight({colorTemp, color: true})) ─────────────
extern const ::zhc::FzConverter* const kFzOsramColorCTLight[];
extern const std::uint8_t              kFzOsramColorCTLightCount;

extern const ::zhc::TzConverter* const kTzOsramColorCTLight[];
extern const std::uint8_t              kTzOsramColorCTLightCount;

extern const ::zhc::Expose             kExposesOsramColorCTLight[];
extern const std::uint8_t              kExposesOsramColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsOsramColorCTLight[];
extern const std::uint8_t              kBindingsOsramColorCTLightCount;

}  // namespace zhc::devices::osram
