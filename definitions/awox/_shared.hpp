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
extern const std::uint8_t              kFzAwoxLightCount;

extern const ::zhc::TzConverter* const kTzAwoxLight[];
extern const std::uint8_t              kTzAwoxLightCount;

extern const ::zhc::Expose             kExposesAwoxLight[];
extern const std::uint8_t              kExposesAwoxLightCount;

extern const ::zhc::BindingSpec        kBindingsAwoxLight[];
extern const std::uint8_t              kBindingsAwoxLightCount;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzAwoxCTLight[];
extern const std::uint8_t              kFzAwoxCTLightCount;

extern const ::zhc::TzConverter* const kTzAwoxCTLight[];
extern const std::uint8_t              kTzAwoxCTLightCount;

extern const ::zhc::Expose             kExposesAwoxCTLight[];
extern const std::uint8_t              kExposesAwoxCTLightCount;

extern const ::zhc::BindingSpec        kBindingsAwoxCTLight[];
extern const std::uint8_t              kBindingsAwoxCTLightCount;

// ── Full RGBW (m.light({colorTemp, color: {modes:["xy","hs"]}})) ────
extern const ::zhc::FzConverter* const kFzAwoxColorCTLight[];
extern const std::uint8_t              kFzAwoxColorCTLightCount;

extern const ::zhc::TzConverter* const kTzAwoxColorCTLight[];
extern const std::uint8_t              kTzAwoxColorCTLightCount;

extern const ::zhc::Expose             kExposesAwoxColorCTLight[];
extern const std::uint8_t              kExposesAwoxColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsAwoxColorCTLight[];
extern const std::uint8_t              kBindingsAwoxColorCTLightCount;

}  // namespace zhc::devices::awox
