// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared bundles for Dresden Elektronik (and Phoscon-branded) lights.
// Modeled on philips/_shared and osram/_shared. The 9-device z2m
// surface only needs two light bundles:
//
//   Tier         | feature set                                    | bindings
//   -------------+------------------------------------------------+----------------
//   CTLight      | on/off + brightness + color_temp (153-500 mireds) | 0x0006, 0x0008, 0x0300
//   ColorCTLight | on/off + brightness + color_temp + color (xy+hs) | 0x0006, 0x0008, 0x0300
//
// FLS-CT uses CTLight. FLS-PP, FLS-H, Hive use ColorCTLight (Hive
// additionally adds the generic battery channel in its port file).
//
// Plain m.light() (Kobold, FLS-A, FLS-M) keeps using `_generic/`
// converters directly — no bundle warranted for two-converter shape.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dresden_elektronik {

// ── m.light({colorTemp: range[153,500]}) — color-temp only ──────────
extern const ::zhc::FzConverter* const kFzDresdenCTLight[];
extern const std::uint8_t              kFzDresdenCTLightCount;
extern const ::zhc::TzConverter* const kTzDresdenCTLight[];
extern const std::uint8_t              kTzDresdenCTLightCount;
extern const ::zhc::Expose             kExposesDresdenCTLight[];
extern const std::uint8_t              kExposesDresdenCTLightCount;
extern const ::zhc::BindingSpec        kBindingsDresdenCTLight[];
extern const std::uint8_t              kBindingsDresdenCTLightCount;

// ── m.light({colorTemp:…, color:true}) — color-temp + color ─────────
extern const ::zhc::FzConverter* const kFzDresdenColorCTLight[];
extern const std::uint8_t              kFzDresdenColorCTLightCount;
extern const ::zhc::TzConverter* const kTzDresdenColorCTLight[];
extern const std::uint8_t              kTzDresdenColorCTLightCount;
extern const ::zhc::Expose             kExposesDresdenColorCTLight[];
extern const std::uint8_t              kExposesDresdenColorCTLightCount;
extern const ::zhc::BindingSpec        kBindingsDresdenColorCTLight[];
extern const std::uint8_t              kBindingsDresdenColorCTLightCount;

}  // namespace zhc::devices::dresden_elektronik
