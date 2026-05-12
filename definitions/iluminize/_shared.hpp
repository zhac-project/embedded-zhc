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
extern const std::uint8_t              kFzIluOnOffCount;

extern const ::zhc::TzConverter* const kTzIluOnOff[];
extern const std::uint8_t              kTzIluOnOffCount;

extern const ::zhc::Expose             kExposesIluOnOff[];
extern const std::uint8_t              kExposesIluOnOffCount;

extern const ::zhc::BindingSpec        kBindingsIluOnOff[];
extern const std::uint8_t              kBindingsIluOnOffCount;

// ── Plain dimmable (m.light({})) ────────────────────────────────────
extern const ::zhc::FzConverter* const kFzIluLight[];
extern const std::uint8_t              kFzIluLightCount;

extern const ::zhc::TzConverter* const kTzIluLight[];
extern const std::uint8_t              kTzIluLightCount;

extern const ::zhc::Expose             kExposesIluLight[];
extern const std::uint8_t              kExposesIluLightCount;

extern const ::zhc::BindingSpec        kBindingsIluLight[];
extern const std::uint8_t              kBindingsIluLightCount;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzIluCTLight[];
extern const std::uint8_t              kFzIluCTLightCount;

extern const ::zhc::TzConverter* const kTzIluCTLight[];
extern const std::uint8_t              kTzIluCTLightCount;

extern const ::zhc::Expose             kExposesIluCTLight[];
extern const std::uint8_t              kExposesIluCTLightCount;

extern const ::zhc::BindingSpec        kBindingsIluCTLight[];
extern const std::uint8_t              kBindingsIluCTLightCount;

// ── Colour-only (m.light({color: true})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzIluColorLight[];
extern const std::uint8_t              kFzIluColorLightCount;

extern const ::zhc::TzConverter* const kTzIluColorLight[];
extern const std::uint8_t              kTzIluColorLightCount;

extern const ::zhc::Expose             kExposesIluColorLight[];
extern const std::uint8_t              kExposesIluColorLightCount;

extern const ::zhc::BindingSpec        kBindingsIluColorLight[];
extern const std::uint8_t              kBindingsIluColorLightCount;

// ── Full RGBCCT (m.light({colorTemp, color: true})) ─────────────────
extern const ::zhc::FzConverter* const kFzIluColorCTLight[];
extern const std::uint8_t              kFzIluColorCTLightCount;

extern const ::zhc::TzConverter* const kTzIluColorCTLight[];
extern const std::uint8_t              kTzIluColorCTLightCount;

extern const ::zhc::Expose             kExposesIluColorCTLight[];
extern const std::uint8_t              kExposesIluColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsIluColorCTLight[];
extern const std::uint8_t              kBindingsIluColorCTLightCount;

}  // namespace zhc::devices::iluminize
