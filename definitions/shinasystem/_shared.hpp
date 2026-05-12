// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared ShinaSystem (SiHAS) vendor converters.
//
// ShinaSystem packs every "custom" attribute onto a STANDARD ZCL cluster
// (genOnOff 0x0006, hvacThermostat 0x0201, genAnalogInput 0x000C) using
// vendor-private attribute IDs in the 0x9000 range — but WITHOUT a
// manufacturer code. So writes are plain ZCL writeAttributes (cmd 0x02,
// fc 0x10) and `ZclWriteSpec.manufacturer_code` stays 0 for every spec
// here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/shinasystem.ts

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::shinasystem {

using ::zhc::FzConverter;
using ::zhc::TzConverter;

// ── from-Zigbee ─────────────────────────────────────────────────────

// `sihas_action` — genOnOff command-style action decoder for the
// SBM/SQM/MSM/CSM/DIO remote families.
//
// Triggers on raw genOnOff commands (commandOn=0x01 → "double",
// commandOff=0x00 → "single", commandToggle=0x02 → "long").
//
// For the multi-button variants (SBM300ZB[2-4], SBM300ZC[2-4],
// MSM-300ZB, SQM300ZC4) the endpoint id (1..4) is prefixed as
// `<ep>_<gesture>` (e.g. "2_single"). For 1-button variants the bare
// gesture is emitted.
//
// Endpoint-prefixing is selected by passing a non-null `user_config`
// of type `const SihasActionConfig*`.
struct SihasActionConfig {
    bool prefix_endpoint;   // true → "<ep>_<gesture>", false → "<gesture>"
};
extern const SihasActionConfig kSihasActionPlain;
extern const SihasActionConfig kSihasActionMulti;

extern const FzConverter kFzSihasAction;        // plain (single endpoint)
extern const FzConverter kFzSihasActionMulti;   // endpoint-prefixed

// `sihas_people_cnt` — genAnalogInput presentValue decoder for the
// CSM-300Z multipurpose people-counter. Emits:
//   "people"  (uint, integer part of presentValue, capped at 80)
//   "status"  (string: "idle"/"in"/"out" from the fractional digit)
extern const FzConverter kFzSihasPeopleCnt;

// ── to-Zigbee ──────────────────────────────────────────────────────
//
// Every spec below targets a vendor-private attribute id in the 0x9000
// range on a *standard* cluster, with manufacturer_code = 0. The
// generic `tz_zcl_write_attr` encoder handles the on-wire packing.
//
// Each kTz* converter is a stand-alone TzConverter which adapter code
// can hand to a port via the `to_zigbee[]` array.

// genOnOff (0x0006) — multi-purpose controls
extern const TzConverter kTzSihasOperationMode;     // 0x9000 enum8: auto/push/latch
extern const TzConverter kTzSihasRfPairing;         // 0x9001 enum8
extern const TzConverter kTzSihasOverheatMode;      // 0x9005 enum8 (read-only get)
extern const TzConverter kTzSihasCloseTimeout;      // 0x9006 u16 (seconds)
extern const TzConverter kTzSihasCloseRemainTimeout;// 0x9007 u16 (seconds)
extern const TzConverter kTzSihasVolume;            // 0x9008 enum8 voice/high/low
extern const TzConverter kTzSihasDiStatus;          // 0x9009 enum8 Close/Open
extern const TzConverter kTzSihasDiType;            // 0x900a enum8 Button/Door
extern const TzConverter kTzSihasDoPulseTime;       // 0x900b u16 ms
extern const TzConverter kTzSihasButtonLockMode;    // 0x900e enum8 unlock/lock
extern const TzConverter kTzSihasSwitch3WayMode;    // 0x900f enum8 disable/enable

// hvacThermostat (0x0201) — HQM thermostat extras
extern const TzConverter kTzSihasScheduleTime;      // 0x9002 u8 minutes
extern const TzConverter kTzSihasValveStatus;       // 0x9003 bool (read-only)

}  // namespace zhc::shinasystem
