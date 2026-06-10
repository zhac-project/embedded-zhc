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
extern const std::uint8_t              kFzAduOnOffCount;

extern const ::zhc::TzConverter* const kTzAduOnOff[];
extern const std::uint8_t              kTzAduOnOffCount;

extern const ::zhc::Expose             kExposesAduOnOff[];
extern const std::uint8_t              kExposesAduOnOffCount;

extern const ::zhc::BindingSpec        kBindingsAduOnOff[];
extern const std::uint8_t              kBindingsAduOnOffCount;

// ── On/off + electricity meter (m.onOff() + m.electricityMeter) ─────
extern const ::zhc::FzConverter* const kFzAduOnOffEM[];
extern const std::uint8_t              kFzAduOnOffEMCount;

extern const ::zhc::TzConverter* const kTzAduOnOffEM[];
extern const std::uint8_t              kTzAduOnOffEMCount;

extern const ::zhc::Expose             kExposesAduOnOffEM[];
extern const std::uint8_t              kExposesAduOnOffEMCount;

extern const ::zhc::BindingSpec        kBindingsAduOnOffEM[];
extern const std::uint8_t              kBindingsAduOnOffEMCount;

// ── Plain dimmable (m.light()) ──────────────────────────────────────
extern const ::zhc::FzConverter* const kFzAduLight[];
extern const std::uint8_t              kFzAduLightCount;

extern const ::zhc::TzConverter* const kTzAduLight[];
extern const std::uint8_t              kTzAduLightCount;

extern const ::zhc::Expose             kExposesAduLight[];
extern const std::uint8_t              kExposesAduLightCount;

extern const ::zhc::BindingSpec        kBindingsAduLight[];
extern const std::uint8_t              kBindingsAduLightCount;

// ── Dimmable + electricity meter (m.light() + m.electricityMeter) ───
extern const ::zhc::FzConverter* const kFzAduLightEM[];
extern const std::uint8_t              kFzAduLightEMCount;

extern const ::zhc::TzConverter* const kTzAduLightEM[];
extern const std::uint8_t              kTzAduLightEMCount;

extern const ::zhc::Expose             kExposesAduLightEM[];
extern const std::uint8_t              kExposesAduLightEMCount;

extern const ::zhc::BindingSpec        kBindingsAduLightEM[];
extern const std::uint8_t              kBindingsAduLightEMCount;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzAduCTLight[];
extern const std::uint8_t              kFzAduCTLightCount;

extern const ::zhc::TzConverter* const kTzAduCTLight[];
extern const std::uint8_t              kTzAduCTLightCount;

extern const ::zhc::Expose             kExposesAduCTLight[];
extern const std::uint8_t              kExposesAduCTLightCount;

extern const ::zhc::BindingSpec        kBindingsAduCTLight[];
extern const std::uint8_t              kBindingsAduCTLightCount;

// ── Full RGBCCT (m.light({colorTemp, color})) ───────────────────────
extern const ::zhc::FzConverter* const kFzAduColorCTLight[];
extern const std::uint8_t              kFzAduColorCTLightCount;

extern const ::zhc::TzConverter* const kTzAduColorCTLight[];
extern const std::uint8_t              kTzAduColorCTLightCount;

extern const ::zhc::Expose             kExposesAduColorCTLight[];
extern const std::uint8_t              kExposesAduColorCTLightCount;

extern const ::zhc::BindingSpec        kBindingsAduColorCTLight[];
extern const std::uint8_t              kBindingsAduColorCTLightCount;

}  // namespace zhc::devices::adurosmart
