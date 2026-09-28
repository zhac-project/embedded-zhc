// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared HORNBACH (FLAIR Viyu) light plumbing — modelled after
// paulmann/_shared and ecosmart/_shared. The entire hornbach.ts surface
// is `m.light(...)` over stock ZCL clusters (genOnOff, genLevelCtrl,
// lightingColorCtrl). The auto-generator dropped the 0x0300 color /
// color_temp axis on every def, wiring only on/off + brightness, so all
// 14 bulbs were missing colour-temperature (and the 6 RGBW bulbs the
// hue/sat colour axis too). Two bundles cover every shape seen:
//
//   Tier         | feature set                            | bindings
//   -------------+----------------------------------------+----------------
//   CTLight      | on/off + brightness + color_temp       | 0x0006,0x0008,0x0300
//   ColorCTLight | + color (xy + hs)                      | 0x0006,0x0008,0x0300
//
// (No plain-dim and no colour-only ports exist in hornbach.ts — every
// def has at least colorTemp.)

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hornbach {

// ── CCT (m.light({colorTemp})) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzHornbachCTLight[];
inline constexpr std::uint8_t          kFzHornbachCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzHornbachCTLight[];
inline constexpr std::uint8_t          kTzHornbachCTLightCount = 3;

extern const ::zhc::Expose             kExposesHornbachCTLight[];
inline constexpr std::uint8_t          kExposesHornbachCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsHornbachCTLight[];
inline constexpr std::uint8_t          kBindingsHornbachCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzHornbachColorCTLight[];
inline constexpr std::uint8_t          kFzHornbachColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzHornbachColorCTLight[];
inline constexpr std::uint8_t          kTzHornbachColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesHornbachColorCTLight[];
inline constexpr std::uint8_t          kExposesHornbachColorCTLightCount = 5;

extern const ::zhc::BindingSpec        kBindingsHornbachColorCTLight[];
inline constexpr std::uint8_t          kBindingsHornbachColorCTLightCount = 3;

}  // namespace zhc::devices::hornbach
