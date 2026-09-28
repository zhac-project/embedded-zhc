// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::ikea {
extern const ::zhc::FzConverter* const kFzIkeaLight[];
inline constexpr std::uint8_t          kFzIkeaLightCount = 3;
extern const ::zhc::TzConverter* const kTzIkeaLight[];
inline constexpr std::uint8_t          kTzIkeaLightCount = 3;
extern const ::zhc::Expose             kExposesIkeaLight[];
inline constexpr std::uint8_t          kExposesIkeaLightCount = 3;
extern const ::zhc::BindingSpec        kBindingsIkeaLight[];
inline constexpr std::uint8_t          kBindingsIkeaLightCount = 3;

// `ikeaLight({color: true})` variants — adds kFzColor + kTzColor +
// color_x/color_y/hue/saturation exposes. Bindings reuse the OnOff +
// LevelCtrl + ColorCtrl set declared above.
extern const ::zhc::FzConverter* const kFzIkeaColorLight[];
inline constexpr std::uint8_t          kFzIkeaColorLightCount = 4;
extern const ::zhc::TzConverter* const kTzIkeaColorLight[];
inline constexpr std::uint8_t          kTzIkeaColorLightCount = 4;
extern const ::zhc::Expose             kExposesIkeaColorLight[];
inline constexpr std::uint8_t          kExposesIkeaColorLightCount = 7;

// ── Default `configureReporting` sets for IKEA bulbs ────────────────
//
// Mirrors z2m's `m.light()` + colour modernExtend defaults. Without
// these, the device emits no spontaneous reports until something
// physically toggles it — the SPA States tab sits empty. Run by the
// declarative reports walker in `runtime/dispatch.cpp::run_configure`.
extern const ::zhc::ReportingSpec      kReportsIkeaLight[];
inline constexpr std::uint8_t          kReportsIkeaLightCount = 4;
extern const ::zhc::ReportingSpec      kReportsIkeaColorLight[];
inline constexpr std::uint8_t          kReportsIkeaColorLightCount = 6;
}
