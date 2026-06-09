// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bosch {

// ── Light-class bundle (kept for any future Bosch-branded light ─────
// device — currently no model in bosch.ts uses this). Existing ports
// that incorrectly referenced these have been re-targeted, but the
// symbols remain so Bos_*.cpp files which still link against them
// continue to build during the staged port sweep.
extern const ::zhc::FzConverter* const kFzBoschLight[];
extern const std::uint8_t              kFzBoschLightCount;
extern const ::zhc::TzConverter* const kTzBoschLight[];
extern const std::uint8_t              kTzBoschLightCount;
extern const ::zhc::Expose             kExposesBoschLight[];
extern const std::uint8_t              kExposesBoschLightCount;
extern const ::zhc::BindingSpec        kBindingsBoschLight[];
extern const std::uint8_t              kBindingsBoschLightCount;

// ── IAS-zone battery sensor (motion / contact / smoke / water) ──────
// Bundle: kFzBattery + kFzIasZone + battery/alarm/tamper exposes.
// Used by BSEN-M, BSEN-C2, BSEN-C2D, BSEN-CV, BSD-2, BSEN-W, BSIR-EZ
// at the IAS-Zone level (their per-device manuSpec extras still need
// dedicated work — see BOSCH_PARITY.md).
extern const ::zhc::FzConverter* const kFzBoschIasBattery[];
extern const std::uint8_t              kFzBoschIasBatteryCount;
extern const ::zhc::Expose             kExposesBoschIasBattery[];
extern const std::uint8_t              kExposesBoschIasBatteryCount;
extern const ::zhc::BindingSpec        kBindingsBoschIasBattery[];
extern const std::uint8_t              kBindingsBoschIasBatteryCount;

// ── Typed IAS-zone sensor bundles (semantic key, not bare `alarm`) ──
// Same genPowerCfg + ssIasZone bindings as kBindingsBoschIasBattery, but
// wire the zone-type-specific converter so the runtime emits z2m's
// semantic key. Contact → `contact`, Motion → `occupancy`, Water →
// `water_leak`. Use kBindingsBoschIasBattery for .bindings.
extern const ::zhc::FzConverter* const kFzBoschContact[];
extern const std::uint8_t              kFzBoschContactCount;
extern const ::zhc::Expose             kExposesBoschContact[];
extern const std::uint8_t              kExposesBoschContactCount;
extern const ::zhc::FzConverter* const kFzBoschMotion[];
extern const std::uint8_t              kFzBoschMotionCount;
extern const ::zhc::Expose             kExposesBoschMotion[];
extern const std::uint8_t              kExposesBoschMotionCount;
extern const ::zhc::FzConverter* const kFzBoschWaterLeak[];
extern const std::uint8_t              kFzBoschWaterLeakCount;
extern const ::zhc::Expose             kExposesBoschWaterLeak[];
extern const std::uint8_t              kExposesBoschWaterLeakCount;
extern const ::zhc::FzConverter* const kFzBoschSmoke[];
extern const std::uint8_t              kFzBoschSmokeCount;
extern const ::zhc::Expose             kExposesBoschSmoke[];
extern const std::uint8_t              kExposesBoschSmokeCount;

// ── Smart-plug bundle (BSP-FZ2 / BSP-FD): on/off + electrical meter ─
extern const ::zhc::FzConverter* const kFzBoschPlug[];
extern const std::uint8_t              kFzBoschPlugCount;
extern const ::zhc::TzConverter* const kTzBoschPlug[];
extern const std::uint8_t              kTzBoschPlugCount;
extern const ::zhc::Expose             kExposesBoschPlug[];
extern const std::uint8_t              kExposesBoschPlugCount;
extern const ::zhc::BindingSpec        kBindingsBoschPlug[];
extern const std::uint8_t              kBindingsBoschPlugCount;

// ── Bosch radiator-thermostat (BTH-RA) bundle ───────────────────────
// Generic hvacThermostat + battery report path, plus a minimal expose
// list. Manu-specific TZ converters below cover the bulk of the
// special attributes the device understands.
extern const ::zhc::FzConverter* const kFzBoschTrv[];
extern const std::uint8_t              kFzBoschTrvCount;
extern const ::zhc::TzConverter* const kTzBoschTrv[];
extern const std::uint8_t              kTzBoschTrvCount;
extern const ::zhc::Expose             kExposesBoschTrv[];
extern const std::uint8_t              kExposesBoschTrvCount;
extern const ::zhc::BindingSpec        kBindingsBoschTrv[];
extern const std::uint8_t              kBindingsBoschTrvCount;

// ── Manu-specific (mfgcode 0x1209 = ROBERT_BOSCH_GMBH) writes ───────
// All target hvacThermostat (0x0201) or hvacUserInterfaceCfg (0x0204)
// with fc=0x14. Mapping per `lib/bosch.ts → boschThermostatExtend.*`.
//
// hvacThermostat manuSpec attribute IDs:
//   operatingMode          0x4007 ENUM8
//   heatingDemand          0x4020 ENUM8 (= pi_heating_demand surrogate)
//   valveAdaptStatus       0x4022 ENUM8
//   remoteTemperature      0x4040 INT16  (×100 °C)
//   windowOpenMode         0x4042 ENUM8
//   boostHeating           0x4043 ENUM8
//   cableSensorTemperature 0x4052 INT16
//   valveType              0x4060 ENUM8
//   cableSensorMode        0x4062 ENUM8
//   heaterType             0x4063 ENUM8
//   errorState             0x5000 BITMAP8
//   automaticValveAdapt    0x5010 ENUM8
//
// hvacUserInterfaceCfg manuSpec attribute IDs:
//   displayOrientation     0x400B UINT8
//   activityLed            0x4033 ENUM8
//   displayedTemperature   0x4039 ENUM8
//   displaySwitchOnDuration0x403A ENUM8
//   displayBrightness      0x403B ENUM8
extern const ::zhc::TzConverter kTzBoschOperatingMode;
extern const ::zhc::TzConverter kTzBoschBoostHeating;
extern const ::zhc::TzConverter kTzBoschWindowOpenMode;
extern const ::zhc::TzConverter kTzBoschRemoteTemperature;
extern const ::zhc::TzConverter kTzBoschValveAdaptStatus;
extern const ::zhc::TzConverter kTzBoschHeaterType;
extern const ::zhc::TzConverter kTzBoschValveType;
extern const ::zhc::TzConverter kTzBoschChildLockUi;       // hvacUserInterfaceCfg keypadLockout
extern const ::zhc::TzConverter kTzBoschDisplayBrightness;
extern const ::zhc::TzConverter kTzBoschDisplayOrientation;
extern const ::zhc::TzConverter kTzBoschDisplayedTemperature;
extern const ::zhc::TzConverter kTzBoschDisplaySwitchDur;
extern const ::zhc::TzConverter kTzBoschActivityLed;

// Twinguard sensitivity write — twinguardSmokeDetector (0xE000) attr
// 0x4003 UINT16 with mfgcode 0x1209.
extern const ::zhc::TzConverter kTzBoschTwinguardSensitivity;

}  // namespace zhc::devices::bosch
