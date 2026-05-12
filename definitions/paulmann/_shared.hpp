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
extern const std::uint8_t              kFzPaulmannLightCount;

extern const ::zhc::TzConverter* const kTzPaulmannLight[];
extern const std::uint8_t              kTzPaulmannLightCount;

extern const ::zhc::Expose             kExposesPaulmannLight[];
extern const std::uint8_t              kExposesPaulmannLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulmannLight[];
extern const std::uint8_t              kBindingsPaulmannLightCount;

// ── White-ambiance / CCT (m.light({colorTemp})) ─────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannCTLight[];
extern const std::uint8_t              kFzPaulmannCTLightCount;

extern const ::zhc::TzConverter* const kTzPaulmannCTLight[];
extern const std::uint8_t              kTzPaulmannCTLightCount;

extern const ::zhc::Expose             kExposesPaulmannCTLight[];
extern const std::uint8_t              kExposesPaulmannCTLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulmannCTLight[];
extern const std::uint8_t              kBindingsPaulmannCTLightCount;

// ── Colour-only (m.light({color})) ──────────────────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannColorLight[];
extern const std::uint8_t              kFzPaulmannColorLightCount;

extern const ::zhc::TzConverter* const kTzPaulmannColorLight[];
extern const std::uint8_t              kTzPaulmannColorLightCount;

extern const ::zhc::Expose             kExposesPaulmannColorLight[];
extern const std::uint8_t              kExposesPaulmannColorLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulmannColorLight[];
extern const std::uint8_t              kBindingsPaulmannColorLightCount;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzPaulmannColorCTLight[];
extern const std::uint8_t              kFzPaulmannColorCTLightCount;

extern const ::zhc::TzConverter* const kTzPaulmannColorCTLight[];
extern const std::uint8_t              kTzPaulmannColorCTLightCount;

extern const ::zhc::Expose             kExposesPaulmannColorCTLight[];
extern const std::uint8_t              kExposesPaulmannColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsPaulmannColorCTLight[];
extern const std::uint8_t              kBindingsPaulmannColorCTLightCount;

}  // namespace zhc::devices::paulmann
