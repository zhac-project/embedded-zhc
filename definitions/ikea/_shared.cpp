// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/ikea/_shared.hpp"
namespace zhc::devices::ikea {
const ::zhc::FzConverter* const kFzIkeaLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzIkeaLightCount == std::size(kFzIkeaLight));
const ::zhc::TzConverter* const kTzIkeaLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzIkeaLightCount == std::size(kTzIkeaLight));
const ::zhc::Expose kExposesIkeaLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesIkeaLightCount == std::size(kExposesIkeaLight));
const ::zhc::BindingSpec kBindingsIkeaLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsIkeaLightCount == std::size(kBindingsIkeaLight));

// ── Color/CT bulbs (`ikeaLight({color: true})`) ─────────────────────
//
// Adds the generic ColorControl decoder (`kFzColor` — hue / saturation
// / xy / mode / enhanced_hue) and the wildcard color setter
// (`kTzColor` claims `color_x` / `color_y` / `hue` / `saturation`).
// Bindings reuse `kBindingsIkeaLight` since IKEA color bulbs already
// bind 0x0300 there — only the converter arrays + exposes change.
const ::zhc::FzConverter* const kFzIkeaColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzIkeaColorLightCount == std::size(kFzIkeaColorLight));
const ::zhc::TzConverter* const kTzIkeaColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzIkeaColorLightCount == std::size(kTzIkeaColorLight));
const ::zhc::Expose kExposesIkeaColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesIkeaColorLightCount == std::size(kExposesIkeaColorLight));

// ── kReportsIkeaLight — tunable-white IKEA bulbs ────────────────────
//
// Mirrors z2m's `m.light()` defaults. Field order:
//   { endpoint, cluster_id, attr_id, attr_type, min_s, max_s, change, mfg }
// attr_type byte: 0x10=bool, 0x20=u8, 0x21=u16, 0x30=enum8.
const ::zhc::ReportingSpec kReportsIkeaLight[] = {
    // genOnOff       onOff             bool   0..3600s
    { 1, 0x0006, 0x0000, 0x10, 0,    3600, 1, 0 },
    // genLevelCtrl   currentLevel      u8     5..3600s
    { 1, 0x0008, 0x0000, 0x20, 5,    3600, 1, 0 },
    // genColorCtrl   colorTemperature  u16    5..3600s, rc=1 mired
    { 1, 0x0300, 0x0007, 0x21, 5,    3600, 1, 0 },
    // genColorCtrl   colorMode         enum8  0..3600s
    { 1, 0x0300, 0x0008, 0x30, 0,    3600, 1, 0 },
};
static_assert(kReportsIkeaLightCount == std::size(kReportsIkeaLight));

// ── kReportsIkeaColorLight — full-colour IKEA bulbs ────────────────
//
// Adds currentX/currentY for full-colour bulbs (z2m's
// `m.colorCtrl({color: true})` adds these to the configureReporting set).
const ::zhc::ReportingSpec kReportsIkeaColorLight[] = {
    { 1, 0x0006, 0x0000, 0x10, 0,    3600, 1, 0 },
    { 1, 0x0008, 0x0000, 0x20, 5,    3600, 1, 0 },
    { 1, 0x0300, 0x0007, 0x21, 5,    3600, 1, 0 },
    { 1, 0x0300, 0x0008, 0x30, 0,    3600, 1, 0 },
    // genColorCtrl   currentX          u16    5..3600s
    { 1, 0x0300, 0x0003, 0x21, 5,    3600, 1, 0 },
    // genColorCtrl   currentY          u16    5..3600s
    { 1, 0x0300, 0x0004, 0x21, 5,    3600, 1, 0 },
};
static_assert(kReportsIkeaColorLightCount == std::size(kReportsIkeaColorLight));
}
