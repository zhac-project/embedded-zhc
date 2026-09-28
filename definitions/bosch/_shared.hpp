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
inline constexpr std::uint8_t          kFzBoschLightCount = 3;
extern const ::zhc::TzConverter* const kTzBoschLight[];
inline constexpr std::uint8_t          kTzBoschLightCount = 3;
extern const ::zhc::Expose             kExposesBoschLight[];
inline constexpr std::uint8_t          kExposesBoschLightCount = 3;
extern const ::zhc::BindingSpec        kBindingsBoschLight[];
inline constexpr std::uint8_t          kBindingsBoschLightCount = 3;

// ── IAS-zone battery sensor (motion / contact / smoke / water) ──────
// Bundle: kFzBattery + kFzIasZone + battery/alarm/tamper exposes.
// Used by BSEN-M, BSEN-C2, BSEN-C2D, BSEN-CV, BSD-2, BSEN-W, BSIR-EZ
// at the IAS-Zone level (their per-device manuSpec extras still need
// dedicated work — see BOSCH_PARITY.md).
extern const ::zhc::FzConverter* const kFzBoschIasBattery[];
inline constexpr std::uint8_t          kFzBoschIasBatteryCount = 2;
extern const ::zhc::Expose             kExposesBoschIasBattery[];
inline constexpr std::uint8_t          kExposesBoschIasBatteryCount = 5;
extern const ::zhc::BindingSpec        kBindingsBoschIasBattery[];
inline constexpr std::uint8_t          kBindingsBoschIasBatteryCount = 2;

// ── Typed IAS-zone sensor bundles (semantic key, not bare `alarm`) ──
// Same genPowerCfg + ssIasZone bindings as kBindingsBoschIasBattery, but
// wire the zone-type-specific converter so the runtime emits z2m's
// semantic key. Contact → `contact`, Motion → `occupancy`, Water →
// `water_leak`. Use kBindingsBoschIasBattery for .bindings.
extern const ::zhc::FzConverter* const kFzBoschContact[];
inline constexpr std::uint8_t          kFzBoschContactCount = 2;
extern const ::zhc::Expose             kExposesBoschContact[];
inline constexpr std::uint8_t          kExposesBoschContactCount = 5;
extern const ::zhc::FzConverter* const kFzBoschMotion[];
inline constexpr std::uint8_t          kFzBoschMotionCount = 2;
extern const ::zhc::Expose             kExposesBoschMotion[];
inline constexpr std::uint8_t          kExposesBoschMotionCount = 5;
extern const ::zhc::FzConverter* const kFzBoschWaterLeak[];
inline constexpr std::uint8_t          kFzBoschWaterLeakCount = 2;
extern const ::zhc::Expose             kExposesBoschWaterLeak[];
inline constexpr std::uint8_t          kExposesBoschWaterLeakCount = 5;
extern const ::zhc::FzConverter* const kFzBoschSmoke[];
inline constexpr std::uint8_t          kFzBoschSmokeCount = 2;
extern const ::zhc::Expose             kExposesBoschSmoke[];
inline constexpr std::uint8_t          kExposesBoschSmokeCount = 5;

// ── Smart-plug bundle (BSP-FZ2 / BSP-FD): on/off + electrical meter ─
extern const ::zhc::FzConverter* const kFzBoschPlug[];
inline constexpr std::uint8_t          kFzBoschPlugCount = 2;
extern const ::zhc::TzConverter* const kTzBoschPlug[];
inline constexpr std::uint8_t          kTzBoschPlugCount = 1;
extern const ::zhc::Expose             kExposesBoschPlug[];
inline constexpr std::uint8_t          kExposesBoschPlugCount = 3;
extern const ::zhc::BindingSpec        kBindingsBoschPlug[];
inline constexpr std::uint8_t          kBindingsBoschPlugCount = 3;

// ── Bosch radiator-thermostat (BTH-RA) bundle ───────────────────────
// Generic hvacThermostat + battery report path, plus a minimal expose
// list. Manu-specific TZ converters below cover the bulk of the
// special attributes the device understands.
extern const ::zhc::FzConverter* const kFzBoschTrv[];
inline constexpr std::uint8_t          kFzBoschTrvCount = 2;
extern const ::zhc::TzConverter* const kTzBoschTrv[];
inline constexpr std::uint8_t          kTzBoschTrvCount = 10;
extern const ::zhc::Expose             kExposesBoschTrv[];
inline constexpr std::uint8_t          kExposesBoschTrvCount = 16;
extern const ::zhc::BindingSpec        kBindingsBoschTrv[];
inline constexpr std::uint8_t          kBindingsBoschTrvCount = 3;

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


// ── BTH-RM230Z (Room thermostat II 230V) bundle ─────────────────────
// The shared TRV bundle plus what the mains thermostat has that the TRVs do
// not: humidity (msRelativeHumidity) and the humidity-alarm LED toggle
// (0x4023, mfg 0x1209; only 0x07 on / 0x06 off are observed values). No
// battery row — it is a 230 V device.
extern const ::zhc::FzConverter kFzBoschHumidityAlarmLed;
extern const ::zhc::TzConverter kTzBoschHumidityAlarmLed;
extern const ::zhc::FzConverter* const kFzBoschRm230z[];
inline constexpr std::uint8_t          kFzBoschRm230zCount = 3;
extern const ::zhc::TzConverter* const kTzBoschRm230z[];
inline constexpr std::uint8_t          kTzBoschRm230zCount = 11;
extern const ::zhc::Expose             kExposesBoschRm230z[];
inline constexpr std::uint8_t          kExposesBoschRm230zCount = 15;
extern const ::zhc::BindingSpec        kBindingsBoschRm230z[];
inline constexpr std::uint8_t          kBindingsBoschRm230zCount = 3;

}  // namespace zhc::devices::bosch
