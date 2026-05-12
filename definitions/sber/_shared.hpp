// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared SDevices ("Sber") vendor converters. Every device that pulls
// from this header should carry a `// Tier 2: uses shared sber
// converters` comment and a `// z2m-source:` provenance line.
//
// Manufacturer code: 0x121B (SBERDEVICES_LTD).
// Manu cluster:      "manuSpecificSDevices" (ID 0xFCCF).

#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::sber {

// Decode reports on cluster `manuSpecificSDevices` (0xFCCF). Switches
// on attr-id and emits canonical exposes keys. Currently handles:
//
//   0x1002 buttonEnableMultiClick (bool)  → "multi_click"
//   0x1003 childLock              (bool)  → "child_lock"
//   0x2001 ledIndicatorOnEnable   (bool)  → "led_on_enable"
//   0x2005 ledIndicatorOffEnable  (bool)  → "led_off_enable"
//   0x2009 ledIndicationType      (enum8) → "led_indication_type"
//   0x3011 upperVoltageThreshold  (uint32)→ "upper_voltage_threshold"
//   0x3012 lowerVoltageThreshold  (uint32)→ "lower_voltage_threshold"
//   0x3013 upperCurrentThreshold  (uint32)→ "upper_current_threshold"
//   0x3014 upperTempThreshold     (int16) → "upper_temp_threshold"
//   0x4001 rmsVoltageMv           (uint32)→ "voltage" (mV)
//   0x4002 rmsCurrentMa           (uint32)→ "current" (mA)
//   0x4003 activePowerMw          (int32) → "power"   (mW)
//
// z2m-source: zigbee-herdsman-converters/src/devices/sber.ts
extern const FzConverter kFzSberManuSpecific;

// Canonical TZ writers (manu-specific writeAttributes, fc=0x14, mfg
// 0x121B). All bind to cluster "manuSpecificSDevices" / cluster_id
// 0xFCCF / command 0x02 (writeAttributes), and forward to
// `tz_zcl_write_attr` via a `ZclWriteSpec` in `user_config`.
extern const TzConverter kTzSberChildLock;
extern const TzConverter kTzSberMultiClick;
extern const TzConverter kTzSberLedOnEnable;
extern const TzConverter kTzSberLedOffEnable;
extern const TzConverter kTzSberLedIndicationType;
extern const TzConverter kTzSberUpperVoltageThreshold;
extern const TzConverter kTzSberLowerVoltageThreshold;
extern const TzConverter kTzSberUpperCurrentThreshold;
extern const TzConverter kTzSberUpperTempThreshold;

}  // namespace zhc::devices::sber
