// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared SmartThings / Samsung vendor converters. Devices pulling from
// this header should carry a `// Tier 2: uses shared smartthings
// converters` comment and a `// z2m-source:` provenance line.
//
// z2m-source: zigbee-herdsman-converters/src/devices/smartthings.ts +
//             converters/fromZigbee.ts.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::smartthings {

// ── SmartThings button action decoder (cluster ssIasZone) ─────────────
//
// The IM6001-BTP01 button repurposes the IAS ssIasZone (0x0500)
// commandStatusChangeNotification command: instead of an alarm bitmask,
// the 16-bit `zoneStatus` field carries a button-state code, and z2m's
// `fz.command_status_change_notification_action` maps the *whole value*
// (not individual bits) through {0:"off", 1:"single", 2:"double",
// 3:"hold"} into a single `action` string.
//
// The generic `kFzIasZoneStatusChange` is wrong here — it would publish
// `alarm_1`/`alarm_2`/`tamper`/`battery_low` booleans and never the
// `action` enum the expose advertises. This converter emits `action`.
//
// z2m-source: converters/fromZigbee.ts
//             `command_status_change_notification_action`.
extern const FzConverter kFzStButtonAction;

}  // namespace zhc::smartthings
