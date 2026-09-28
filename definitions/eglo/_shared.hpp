// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared light bundles for the EGLO connect bulb family. EGLO is an AwoX
// rebadge; z2m models every bulb with `m.light(...)` in one of two
// feature combinations:
//   * tunable white   — m.light({colorTemp})
//   * full RGBW        — m.light({colorTemp, color:{modes:["xy","hs"]}})
// The generator lowered all of them onto on/off + brightness only,
// dropping the colorTemp (and, for RGBW, the colour) channels. These
// bundles restore the missing fz/tz converters + exposes. Modelled on
// awox/_shared.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::eglo {

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzEgloCTLight[];
inline constexpr std::uint8_t kFzEgloCTLightCount = 3;
extern const ::zhc::TzConverter* const kTzEgloCTLight[];
inline constexpr std::uint8_t kTzEgloCTLightCount = 3;
extern const ::zhc::Expose kExposesEgloCTLight[];
inline constexpr std::uint8_t kExposesEgloCTLightCount = 3;
extern const ::zhc::BindingSpec kBindingsEgloCTLight[];
inline constexpr std::uint8_t kBindingsEgloCTLightCount = 3;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzEgloColorCTLight[];
inline constexpr std::uint8_t kFzEgloColorCTLightCount = 4;
extern const ::zhc::TzConverter* const kTzEgloColorCTLight[];
inline constexpr std::uint8_t kTzEgloColorCTLightCount = 4;
extern const ::zhc::Expose kExposesEgloColorCTLight[];
inline constexpr std::uint8_t kExposesEgloColorCTLightCount = 7;
extern const ::zhc::BindingSpec kBindingsEgloColorCTLight[];
inline constexpr std::uint8_t kBindingsEgloColorCTLightCount = 3;

}  // namespace zhc::devices::eglo
