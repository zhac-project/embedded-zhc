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
extern const std::uint8_t kFzEgloCTLightCount;
extern const ::zhc::TzConverter* const kTzEgloCTLight[];
extern const std::uint8_t kTzEgloCTLightCount;
extern const ::zhc::Expose kExposesEgloCTLight[];
extern const std::uint8_t kExposesEgloCTLightCount;
extern const ::zhc::BindingSpec kBindingsEgloCTLight[];
extern const std::uint8_t kBindingsEgloCTLightCount;

// ── Full RGBW (m.light({colorTemp, color})) ─────────────────────────
extern const ::zhc::FzConverter* const kFzEgloColorCTLight[];
extern const std::uint8_t kFzEgloColorCTLightCount;
extern const ::zhc::TzConverter* const kTzEgloColorCTLight[];
extern const std::uint8_t kTzEgloColorCTLightCount;
extern const ::zhc::Expose kExposesEgloColorCTLight[];
extern const std::uint8_t kExposesEgloColorCTLightCount;
extern const ::zhc::BindingSpec kBindingsEgloColorCTLight[];
extern const std::uint8_t kBindingsEgloColorCTLightCount;

}  // namespace zhc::devices::eglo
