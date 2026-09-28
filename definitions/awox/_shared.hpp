// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared AwoX light plumbing — modeled on osram/_shared.
// AwoX's z2m surface is plain ZCL: every bulb in awox.ts is `m.light(...)`
// over standard clusters. The vendor-specific bits (`awox_color_ctrl`,
// `awox_level_ctrl`, `awox_scenes_raw`, `awox_remote_actions`) only fire on
// the ERCU/EPIR remotes and EBF_RGB_Zm command surface, which keep their own
// per-port files.
//
// Three shared bundles cover the bulb feature combinations seen in awox.ts:
//
//   Tier        | feature set                          | bindings
//   ------------+--------------------------------------+----------------
//   Light       | on/off + brightness                  | 0x0006, 0x0008
//   CTLight     | + color_temp (mired)                 | + 0x0300
//   ColorCTLight| color_temp + color (xy + hs)         | + 0x0300

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::awox {

// ── Plain dimmable (m.light({})) ────────────────────────────────────
extern const ::zhc::FzConverter* const kFzAwoxLight[];
inline constexpr std::uint8_t          kFzAwoxLightCount = 2;

extern const ::zhc::TzConverter* const kTzAwoxLight[];
inline constexpr std::uint8_t          kTzAwoxLightCount = 2;

extern const ::zhc::Expose             kExposesAwoxLight[];
inline constexpr std::uint8_t          kExposesAwoxLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsAwoxLight[];
inline constexpr std::uint8_t          kBindingsAwoxLightCount = 2;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzAwoxCTLight[];
inline constexpr std::uint8_t          kFzAwoxCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzAwoxCTLight[];
inline constexpr std::uint8_t          kTzAwoxCTLightCount = 3;

extern const ::zhc::Expose             kExposesAwoxCTLight[];
inline constexpr std::uint8_t          kExposesAwoxCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsAwoxCTLight[];
inline constexpr std::uint8_t          kBindingsAwoxCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color: {modes:["xy","hs"]}})) ────
extern const ::zhc::FzConverter* const kFzAwoxColorCTLight[];
inline constexpr std::uint8_t          kFzAwoxColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzAwoxColorCTLight[];
inline constexpr std::uint8_t          kTzAwoxColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesAwoxColorCTLight[];
inline constexpr std::uint8_t          kExposesAwoxColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsAwoxColorCTLight[];
inline constexpr std::uint8_t          kBindingsAwoxColorCTLightCount = 3;

}  // namespace zhc::devices::awox
