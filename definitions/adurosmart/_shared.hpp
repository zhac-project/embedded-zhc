// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared AduroSmart light/plug plumbing — modelled on ledvance/_shared.
//
// AduroSmart's z2m surface is mostly stock `m.light({...})` and
// `m.onOff()` with two metering plugs (`m.electricityMeter`) and one
// remote/siren pair that keep their own per-port wiring. The
// `81949` dimmer also carries a manuSpec attribute family in
// genBasic (0x7600 / 0x7700 / 0x7701), which is currently exposed
// best-effort as raw attribute reports — the ENUM expose is left
// open for follow-up (see ADUROSMART_PARITY.md).
//
// Six bundles cover every feature combination seen in z2m's
// adurosmart.ts (1:1 with iluminize/ledvance taxonomy):
//
//   Tier         | feature set                               | bindings
//   -------------+-------------------------------------------+----------------
//   OnOff        | m.onOff() — relay/plug                    | 0x0006
//   OnOffEM      | + electrical_measurement (electrical-only)| 0x0006 0x0B04
//   Light        | m.light() — brightness only               | 0x0006 0x0008
//   LightEM      | + electrical_measurement (electrical-only)| + 0x0B04
//   CTLight      | m.light({colorTemp})                      | + 0x0300
//   ColorCTLight | m.light({colorTemp, color})               | + 0x0300
//
// NOTE: every AduroSmart power device in z2m uses electricityMeter
// ({cluster:"electrical"}) / fz.electrical_measurement — i.e. the
// haElectricalMeasurement (0x0B04) cluster ONLY, exposing power/
// voltage/current and NO `energy`. The EM bundles therefore omit
// seMetering (0x0702) and the `energy` expose.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adurosmart {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
extern const ::zhc::FzConverter* const kFzAduOnOff[];
inline constexpr std::uint8_t          kFzAduOnOffCount = 1;

extern const ::zhc::TzConverter* const kTzAduOnOff[];
inline constexpr std::uint8_t          kTzAduOnOffCount = 1;

extern const ::zhc::Expose             kExposesAduOnOff[];
inline constexpr std::uint8_t          kExposesAduOnOffCount = 1;

extern const ::zhc::BindingSpec        kBindingsAduOnOff[];
inline constexpr std::uint8_t          kBindingsAduOnOffCount = 1;

// ── On/off + electricity meter (m.onOff() + m.electricityMeter) ─────
extern const ::zhc::FzConverter* const kFzAduOnOffEM[];
inline constexpr std::uint8_t          kFzAduOnOffEMCount = 2;

extern const ::zhc::TzConverter* const kTzAduOnOffEM[];
inline constexpr std::uint8_t          kTzAduOnOffEMCount = 1;

extern const ::zhc::Expose             kExposesAduOnOffEM[];
inline constexpr std::uint8_t          kExposesAduOnOffEMCount = 4;

extern const ::zhc::BindingSpec        kBindingsAduOnOffEM[];
inline constexpr std::uint8_t          kBindingsAduOnOffEMCount = 2;

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzAduLight[];
inline constexpr std::uint8_t          kFzAduLightCount = 2;

extern const ::zhc::TzConverter* const kTzAduLight[];
inline constexpr std::uint8_t          kTzAduLightCount = 2;

extern const ::zhc::Expose             kExposesAduLight[];
inline constexpr std::uint8_t          kExposesAduLightCount = 2;

extern const ::zhc::BindingSpec        kBindingsAduLight[];
inline constexpr std::uint8_t          kBindingsAduLightCount = 2;

// ── Dimmable + electricity meter (m.light() + m.electricityMeter) ───
extern const ::zhc::FzConverter* const kFzAduLightEM[];
inline constexpr std::uint8_t          kFzAduLightEMCount = 3;

extern const ::zhc::TzConverter* const kTzAduLightEM[];
inline constexpr std::uint8_t          kTzAduLightEMCount = 2;

extern const ::zhc::Expose             kExposesAduLightEM[];
inline constexpr std::uint8_t          kExposesAduLightEMCount = 5;

extern const ::zhc::BindingSpec        kBindingsAduLightEM[];
inline constexpr std::uint8_t          kBindingsAduLightEMCount = 3;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzAduCTLight[];
inline constexpr std::uint8_t          kFzAduCTLightCount = 3;

extern const ::zhc::TzConverter* const kTzAduCTLight[];
inline constexpr std::uint8_t          kTzAduCTLightCount = 3;

extern const ::zhc::Expose             kExposesAduCTLight[];
inline constexpr std::uint8_t          kExposesAduCTLightCount = 3;

extern const ::zhc::BindingSpec        kBindingsAduCTLight[];
inline constexpr std::uint8_t          kBindingsAduCTLightCount = 3;

// ── Full RGBCCT (m.light({colorTemp, color})) ───────────────────────
extern const ::zhc::FzConverter* const kFzAduColorCTLight[];
inline constexpr std::uint8_t          kFzAduColorCTLightCount = 4;

extern const ::zhc::TzConverter* const kTzAduColorCTLight[];
inline constexpr std::uint8_t          kTzAduColorCTLightCount = 4;

extern const ::zhc::Expose             kExposesAduColorCTLight[];
inline constexpr std::uint8_t          kExposesAduColorCTLightCount = 7;

extern const ::zhc::BindingSpec        kBindingsAduColorCTLight[];
inline constexpr std::uint8_t          kBindingsAduColorCTLightCount = 3;

}  // namespace zhc::devices::adurosmart
