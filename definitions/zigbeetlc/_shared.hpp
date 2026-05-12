// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared ZigbeeTLc converters / write specs.
//
// All 10 devices in zigbeetlc.ts share the exact same `extend` block:
// temperature + humidity + battery (voltage) + 11 hvacUserInterfaceCfg
// (0x0204) attrs that are NOT manufacturer-specific (mfgcode = 0).
//
//   0x0204 hvacUserInterfaceCfg — non-manu writes:
//     attr 0x0000 tempDisplayMode          ENUM8   ("celsius" / "fahrenheit")
//     attr 0x0002 comfort_smiley           ENUM8   (0=on, 1=off — z2m valueOn=true→0)
//     attr 0x0100 temperature_calibration  INT16   (°C × 100)
//     attr 0x0101 humidity_calibration     INT16   (% × 100)
//     attr 0x0102 comfort_temperature_min  INT16   (°C × 100)
//     attr 0x0103 comfort_temperature_max  INT16   (°C × 100)
//     attr 0x0104 comfort_humidity_min     UINT16  (% × 100)
//     attr 0x0105 comfort_humidity_max     UINT16  (% × 100)
//     attr 0x0106 enable_display           ENUM8   (0=on, 1=off)
//     attr 0x0107 measurement_interval     UINT8   (seconds, 3..255)
//
// All writes route through `::zhc::generic::tz_zcl_write_attr` with
// fc=0x10 (no mfg envelope). Devices that pull from this header should
// carry a `// Tier 2: uses shared zigbeetlc converters` comment and a
// `// z2m-source:` line.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::zigbeetlc {

// ── hvacUserInterfaceCfg (0x0204) writeAttributes ────────────────
extern const TzConverter kTzTempDisplayMode;        // ENUM8  attr 0x0000
extern const TzConverter kTzComfortSmiley;          // ENUM8  attr 0x0002
extern const TzConverter kTzTemperatureCalibration; // INT16  attr 0x0100
extern const TzConverter kTzHumidityCalibration;    // INT16  attr 0x0101
extern const TzConverter kTzComfortTemperatureMin;  // INT16  attr 0x0102
extern const TzConverter kTzComfortTemperatureMax;  // INT16  attr 0x0103
extern const TzConverter kTzComfortHumidityMin;     // UINT16 attr 0x0104
extern const TzConverter kTzComfortHumidityMax;     // UINT16 attr 0x0105
extern const TzConverter kTzEnableDisplay;          // ENUM8  attr 0x0106
extern const TzConverter kTzMeasurementInterval;    // UINT8  attr 0x0107

// Shared expose array — every device in this vendor exposes exactly
// this set. Indexed by `kSharedExposes` / `kSharedExposesCount`.
extern const ::zhc::Expose kSharedExposes[];
extern const std::uint8_t  kSharedExposesCount;

// Shared from-zigbee table (battery + temperature + humidity).
extern const ::zhc::FzConverter* const kSharedFz[];
extern const std::uint8_t               kSharedFzCount;

// Shared to-zigbee table (10 hvacUserInterfaceCfg writes).
extern const ::zhc::TzConverter* const kSharedTz[];
extern const std::uint8_t               kSharedTzCount;

// Shared bindings: ep1 genPowerCfg, msTemperatureMeasurement,
// msRelativeHumidity, hvacUserInterfaceCfg.
extern const ::zhc::BindingSpec kSharedBindings[];
extern const std::uint8_t       kSharedBindingsCount;

}  // namespace zhc::zigbeetlc
