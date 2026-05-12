// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Viessmann (ViCare TRV / climate sensor) converters. Devices
// that include this header should carry a `// Tier 2: uses shared
// viessmann converters` comment and a `// z2m-source:` provenance
// line.
//
// Manufacturer code is `Zcl.ManufacturerCode.VIESSMANN_ELEKTRONIK_GMBH`
// (0x1221). Viessmann's TRV adds three manuSpec attrs on
// `hvacThermostat` (cluster 0x0201):
//
//   0x4000 ENUM8   viessmannWindowOpenInternal → "window_open"
//                                                 (decoded as bool: !=0)
//   0x4003 BOOLEAN viessmannWindowOpenForce    → "window_open_force"
//   0x4012 BOOLEAN viessmannAssemblyMode       → "assembly_mode"
//
// In z2m the read-side converter is named `fzLocal.viessmann_thermostat`
// and chains `fz.thermostat.convert(...)` with the three extras. Our
// equivalent splits responsibilities — generic `kFzThermostat` decodes
// the standard attrs; `kFzViessmannThermostat` adds the manu-specific
// trio.
//
// Two non-Viessmann decoders also live here because no generic
// equivalent exists yet:
//
//   * `kFzCo2`             — `msCO2.measuredValue` (cluster 0x040D
//                            attr 0x0000) → `co2` (ppm = raw * 1e6)
//   * `kFzKeypadLockout`   — `hvacUserInterfaceCfg.keypadLockout`
//                            (cluster 0x0204 attr 0x0001) →
//                            `keypad_lockout`
//
// Tz writes:
//
//   * `kTzViessmannWindowOpenForce` — manu-specific BOOLEAN write to
//                                     attr 0x4003 (mfg=0x1221)
//   * `kTzKeypadLockout`            — non-manu ENUM8 write to attr
//                                     0x0001 on hvacUserInterfaceCfg
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::viessmann {

// Manufacturer code for every Viessmann manu-specific write/read.
constexpr std::uint16_t kViessmannMfg = 0x1221;

// FzConverter for Viessmann-specific hvacThermostat attrs (mfg 0x1221).
// Standard attrs (local_temperature, occupied_heating_setpoint,
// system_mode) are still handled by `::zhc::generic::kFzThermostat`;
// this converter only emits the vendor-specific ones.
extern const ::zhc::FzConverter kFzViessmannThermostat;

// FzConverter for `msCO2` (0x040D). Emits `co2` in ppm (raw float
// fraction × 1e6, cast to integer ppm). z2m source:
// `fromZigbee.ts::co2`.
extern const ::zhc::FzConverter kFzCo2;

// FzConverter for `hvacUserInterfaceCfg.keypadLockout` (cluster
// 0x0204 attr 0x0001). Emits `keypad_lockout` as raw ENUM8 (z2m maps
// values via `constants.keypadLockoutMode` but falls back to the raw
// number when unmapped — we always emit the raw number).
extern const ::zhc::FzConverter kFzKeypadLockout;

// TzConverter — manu-specific BOOLEAN write to attr 0x4003 on
// `hvacThermostat`. Accepts bool input.
extern const ::zhc::TzConverter kTzViessmannWindowOpenForce;

// TzConverter — non-manu ENUM8 write to attr 0x0001 on
// `hvacUserInterfaceCfg`. Accepts uint8 input (0..3).
extern const ::zhc::TzConverter kTzKeypadLockout;

}  // namespace zhc::devices::viessmann
