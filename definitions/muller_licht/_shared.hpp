// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Müller-Licht (tint) light/plug plumbing — modelled on osram/_shared.
// The vast majority of muller_licht.ts is `mullerLichtLight(args)` (which is
// `m.light(args)` plus an extra tz.tint_scene write — z2m only — and the
// rest is a sprinkle of `m.onOff()` plugs and three battery remotes (the
// remotes have their own hand-rolled ports).
//
// Four bundles cover every feature combination the lights+plugs cover.
// (Pure colour-without-CT — the `ColorLight` tier from osram/_shared —
// has no consumer in muller_licht.ts: every colour-capable tint model
// also exposes colour temperature.)
//
//   Tier         | feature set                          | bindings
//   -------------+--------------------------------------+----------------
//   OnOff        | on/off only (plug)                   | 0x0006
//   Light        | on/off + brightness                  | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)                 | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)         | + 0x0300
//
// `tint_scene` (a z2m-side toZigbee write that loops `genIdentify` /
// `genGroups`) is not yet ported — the SPA can drive scenes directly via
// the genScenes cluster or by sending the recall command, so the missing
// helper does not block scene recall. Tracked in
// `docs/MULLER_LICHT_PARITY.md`.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::muller_licht {

// ── On/off only (m.onOff()) — Tint power strips / sockets ───────────
extern const ::zhc::FzConverter* const kFzMullerLichtOnOff[];
inline constexpr std::uint8_t          kFzMullerLichtOnOffCount = 1;

extern const ::zhc::TzConverter* const kTzMullerLichtOnOff[];
inline constexpr std::uint8_t          kTzMullerLichtOnOffCount = 1;

extern const ::zhc::Expose             kExposesMullerLichtOnOff[];
inline constexpr std::uint8_t          kExposesMullerLichtOnOffCount = 1;

extern const ::zhc::BindingSpec        kBindingsMullerLichtOnOff[];
inline constexpr std::uint8_t          kBindingsMullerLichtOnOffCount = 1;

// ── Dimmable (m.light({})) ──────────────────────────────────────────
extern const ::zhc::FzConverter* const kFzMullerLichtLight[];
inline constexpr std::uint8_t          kFzMullerLichtLightCount = 2;

extern const ::zhc::TzConverter* const kTzMullerLichtLight[];
inline constexpr std::uint8_t          kTzMullerLichtLightCount = 2;

extern const ::zhc::Expose             kExposesMullerLichtLight[];
inline constexpr std::uint8_t          kExposesMullerLichtLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsMullerLichtLight[];
inline constexpr std::uint8_t          kBindingsMullerLichtLightCount = 2;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzMullerLichtCTLight[];
inline constexpr std::uint8_t          kFzMullerLichtCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzMullerLichtCTLight[];
inline constexpr std::uint8_t          kTzMullerLichtCTLightCount = 3;

extern const ::zhc::Expose             kExposesMullerLichtCTLight[];
inline constexpr std::uint8_t          kExposesMullerLichtCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsMullerLichtCTLight[];
inline constexpr std::uint8_t          kBindingsMullerLichtCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzMullerLichtColorCTLight[];
inline constexpr std::uint8_t          kFzMullerLichtColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzMullerLichtColorCTLight[];
inline constexpr std::uint8_t          kTzMullerLichtColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesMullerLichtColorCTLight[];
inline constexpr std::uint8_t          kExposesMullerLichtColorCTLightCount = 5;

extern const ::zhc::BindingSpec        kBindingsMullerLichtColorCTLight[];
inline constexpr std::uint8_t          kBindingsMullerLichtColorCTLightCount = 3;

}  // namespace zhc::devices::muller_licht
