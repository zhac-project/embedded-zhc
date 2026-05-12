// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::ikea {
extern const ::zhc::FzConverter* const kFzIkeaLight[];
extern const std::uint8_t              kFzIkeaLightCount;
extern const ::zhc::TzConverter* const kTzIkeaLight[];
extern const std::uint8_t              kTzIkeaLightCount;
extern const ::zhc::Expose             kExposesIkeaLight[];
extern const std::uint8_t              kExposesIkeaLightCount;
extern const ::zhc::BindingSpec        kBindingsIkeaLight[];
extern const std::uint8_t              kBindingsIkeaLightCount;

// `ikeaLight({color: true})` variants — adds kFzColor + kTzColor +
// color_x/color_y/hue/saturation exposes. Bindings reuse the OnOff +
// LevelCtrl + ColorCtrl set declared above.
extern const ::zhc::FzConverter* const kFzIkeaColorLight[];
extern const std::uint8_t              kFzIkeaColorLightCount;
extern const ::zhc::TzConverter* const kTzIkeaColorLight[];
extern const std::uint8_t              kTzIkeaColorLightCount;
extern const ::zhc::Expose             kExposesIkeaColorLight[];
extern const std::uint8_t              kExposesIkeaColorLightCount;

// ── Default `configureReporting` sets for IKEA bulbs ────────────────
//
// Mirrors z2m's `m.light()` + colour modernExtend defaults. Without
// these, the device emits no spontaneous reports until something
// physically toggles it — the SPA States tab sits empty. Run by the
// declarative reports walker in `runtime/dispatch.cpp::run_configure`.
extern const ::zhc::ReportingSpec      kReportsIkeaLight[];
extern const std::uint8_t              kReportsIkeaLightCount;
extern const ::zhc::ReportingSpec      kReportsIkeaColorLight[];
extern const std::uint8_t              kReportsIkeaColorLightCount;
}
