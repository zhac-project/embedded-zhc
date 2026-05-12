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
extern const std::uint8_t              kFzMullerLichtOnOffCount;

extern const ::zhc::TzConverter* const kTzMullerLichtOnOff[];
extern const std::uint8_t              kTzMullerLichtOnOffCount;

extern const ::zhc::Expose             kExposesMullerLichtOnOff[];
extern const std::uint8_t              kExposesMullerLichtOnOffCount;

extern const ::zhc::BindingSpec        kBindingsMullerLichtOnOff[];
extern const std::uint8_t              kBindingsMullerLichtOnOffCount;

// ── Dimmable (m.light({})) ──────────────────────────────────────────
extern const ::zhc::FzConverter* const kFzMullerLichtLight[];
extern const std::uint8_t              kFzMullerLichtLightCount;

extern const ::zhc::TzConverter* const kTzMullerLichtLight[];
extern const std::uint8_t              kTzMullerLichtLightCount;

extern const ::zhc::Expose             kExposesMullerLichtLight[];
extern const std::uint8_t              kExposesMullerLichtLightCount;

extern const ::zhc::BindingSpec        kBindingsMullerLichtLight[];
extern const std::uint8_t              kBindingsMullerLichtLightCount;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzMullerLichtCTLight[];
extern const std::uint8_t              kFzMullerLichtCTLightCount;

extern const ::zhc::TzConverter* const kTzMullerLichtCTLight[];
extern const std::uint8_t              kTzMullerLichtCTLightCount;

extern const ::zhc::Expose             kExposesMullerLichtCTLight[];
extern const std::uint8_t              kExposesMullerLichtCTLightCount;

extern const ::zhc::BindingSpec        kBindingsMullerLichtCTLight[];
extern const std::uint8_t              kBindingsMullerLichtCTLightCount;

// ── Full RGBW (m.light({colorTemp, color: true})) ───────────────────
extern const ::zhc::FzConverter* const kFzMullerLichtColorCTLight[];
extern const std::uint8_t              kFzMullerLichtColorCTLightCount;

extern const ::zhc::TzConverter* const kTzMullerLichtColorCTLight[];
extern const std::uint8_t              kTzMullerLichtColorCTLightCount;

extern const ::zhc::Expose             kExposesMullerLichtColorCTLight[];
extern const std::uint8_t              kExposesMullerLichtColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsMullerLichtColorCTLight[];
extern const std::uint8_t              kBindingsMullerLichtColorCTLightCount;

}  // namespace zhc::devices::muller_licht
