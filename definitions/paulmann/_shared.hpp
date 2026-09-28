// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Paulmann light plumbing — modelled after innr/_shared and
// osram/_shared. Paulmann's z2m surface for lighting is exclusively
// `m.light(...)` over stock ZCL clusters (genOnOff, genLevelCtrl,
// lightingColorCtrl). No vendor cluster, no custom converters apart
// from the 501.41 colour-temp-stop quirk which keeps its own hand-
// rolled port. Five bundles cover every `m.light(...)` shape seen in
// paulmann.ts.
//
//   Tier         | feature set                            | bindings
//   -------------+----------------------------------------+----------------
//   Light        | on/off + brightness                    | 0x0006, 0x0008
//   CTLight      | + color_temp (mired)                   | + 0x0300
//   ColorLight   | + color (xy + hs), no CT               | + 0x0300
//   ColorCTLight | color_temp + color (xy + hs)           | + 0x0300
//
// On/off-only ports (relays, plugs, solar number light, 501.39 Tuya
// universal switch) keep their inline `kFzOnOff` / `kTzOnOff` arrays —
// the bundle would not save lines, and their expose lists differ.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannLight[];
inline constexpr std::uint8_t          kFzPaulmannLightCount = 2;

extern const ::zhc::TzConverter* const kTzPaulmannLight[];
inline constexpr std::uint8_t          kTzPaulmannLightCount = 2;

extern const ::zhc::Expose             kExposesPaulmannLight[];
inline constexpr std::uint8_t          kExposesPaulmannLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsPaulmannLight[];
inline constexpr std::uint8_t          kBindingsPaulmannLightCount = 2;

// ── White-ambiance / CCT (m.light({colorTemp})) ─────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannCTLight[];
inline constexpr std::uint8_t          kFzPaulmannCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzPaulmannCTLight[];
inline constexpr std::uint8_t          kTzPaulmannCTLightCount = 3;

extern const ::zhc::Expose             kExposesPaulmannCTLight[];
inline constexpr std::uint8_t          kExposesPaulmannCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsPaulmannCTLight[];
inline constexpr std::uint8_t          kBindingsPaulmannCTLightCount = 3;

// ── Colour-only (m.light({color})) ──────────────────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannColorLight[];
inline constexpr std::uint8_t          kFzPaulmannColorLightCount = 3;

extern const ::zhc::TzConverter* const kTzPaulmannColorLight[];
inline constexpr std::uint8_t          kTzPaulmannColorLightCount = 3;

extern const ::zhc::Expose             kExposesPaulmannColorLight[];
inline constexpr std::uint8_t          kExposesPaulmannColorLightCount = 4;

extern const ::zhc::BindingSpec        kBindingsPaulmannColorLight[];
inline constexpr std::uint8_t          kBindingsPaulmannColorLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannColorCTLight[];
inline constexpr std::uint8_t          kFzPaulmannColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzPaulmannColorCTLight[];
inline constexpr std::uint8_t          kTzPaulmannColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesPaulmannColorCTLight[];
inline constexpr std::uint8_t          kExposesPaulmannColorCTLightCount = 5;

extern const ::zhc::BindingSpec        kBindingsPaulmannColorCTLight[];
inline constexpr std::uint8_t          kBindingsPaulmannColorCTLightCount = 3;

}  // namespace zhc::devices::paulmann
