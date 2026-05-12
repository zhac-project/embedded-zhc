// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Lytko vendor write-attribute converters. Lytko thermostats
// expose a mix of standard hvacThermostat / hvacUserInterfaceCfg
// attributes and four manufacturer-specific ones (manuCode 0x7777
// = Zcl.ManufacturerCode.CUSTOM_LYTKO):
//
//   hvacThermostat (0x0201)
//     0x0034 occupiedSetback      uint8   non-manu, write
//     0x7700 sensorType           enum8   manu, write   (lookup 0..9)
//     0x7701 targetTempFirst      bool    manu, write
//   hvacUserInterfaceCfg (0x0204)
//     0x0001 keypadLockout        enum8   non-manu, write (standard)
//     0x7700 brightnessActive     uint8   manu, write
//     0x7701 brightnessStandby    uint8   manu, write
//
// Every device pulls these via `definitions/_generic/_shared.hpp`'s
// `tz_zcl_write_attr` engine, so wiring a device only needs a pointer
// into the corresponding `kTzLytko*` symbol below.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts
// (lytkoExtend.lytkoHvacThermostatCluster + lytkoHvacUserInterfaceCfg).

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lytko {

extern const TzConverter kTzLytkoSensorType;
extern const TzConverter kTzLytkoTargetTempFirst;
extern const TzConverter kTzLytkoOccupiedSetback;
extern const TzConverter kTzLytkoKeypadLockout;
extern const TzConverter kTzLytkoBrightnessActive;
extern const TzConverter kTzLytkoBrightnessStandby;

}  // namespace zhc::devices::lytko
