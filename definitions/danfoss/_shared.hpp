// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Danfoss vendor-shared converters. Wraps the manuSpecific attributes
// added by `danfossExtend.addDanfoss*Cluster()` in z2m's
// `src/devices/danfoss.ts`. Manufacturer code is `0x1246`
// (`Zcl.ManufacturerCode.DANFOSS_A_S`). The bulk of Danfoss attribute
// IDs live in the standard `hvacThermostat` (0x0201) and
// `hvacUserInterfaceCfg` (0x0204) clusters but use mfg-specific
// attribute IDs in the 0x4000-0x4051 / 0x4100-0x4122 ranges.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::danfoss {

// FzConverter that decodes Danfoss-specific hvacThermostat attributes
// (mfg 0x1246). The standard attrs (local_temperature, etc.) are still
// handled by the generic `kFzThermostat`; this converter only emits
// the vendor-specific ones (mounted_mode_active, viewing_direction,
// heat_required, window_open_internal, etc.).
extern const ::zhc::FzConverter kFzDanfossThermostat;

// FzConverter for `hvacUserInterfaceCfg` (0x0204) — currently only
// `viewing_direction` (attr 0x4000, mfg 0x1246).
extern const ::zhc::FzConverter kFzDanfossUserInterfaceCfg;

// Per-attribute TzConverters (writeAttributes with mfg 0x1246).
// Each pairs a string-typed expose key with the manuSpec attribute.
//
// hvacThermostat manu-specific writes:
extern const ::zhc::TzConverter kTzDanfossWindowOpenInternal;        // 0x4000 ENUM8
extern const ::zhc::TzConverter kTzDanfossWindowOpenExternal;        // 0x4003 BOOLEAN
extern const ::zhc::TzConverter kTzDanfossMountedModeControl;        // 0x4013 BOOLEAN
extern const ::zhc::TzConverter kTzDanfossThermostatOrientation;     // 0x4014 BOOLEAN
extern const ::zhc::TzConverter kTzDanfossExternalRoomSensor;        // 0x4015 INT16
extern const ::zhc::TzConverter kTzDanfossRadiatorCovered;           // 0x4016 BOOLEAN
extern const ::zhc::TzConverter kTzDanfossAlgorithmScaleFactor;      // 0x4020 UINT8
extern const ::zhc::TzConverter kTzDanfossLoadBalancingEnable;       // 0x4032 BOOLEAN
extern const ::zhc::TzConverter kTzDanfossLoadRoomMean;              // 0x4040 INT16
extern const ::zhc::TzConverter kTzDanfossRegulationSetpointOffset;  // 0x404c INT8
extern const ::zhc::TzConverter kTzDanfossAdaptionRunControl;        // 0x404e ENUM8
extern const ::zhc::TzConverter kTzDanfossWindowOpenFeatureEnable;   // 0x4051 BOOLEAN

// hvacUserInterfaceCfg manu-specific write:
extern const ::zhc::TzConverter kTzDanfossViewingDirection;          // 0x4000 ENUM8

}  // namespace zhc::devices::danfoss
