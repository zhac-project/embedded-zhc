// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Wally vendor converters for the WallyHome multi-sensor
// (U02I007C.01 / zigbeeModel "MultiSensor").
//
// The device carries TWO IAS zones on DIFFERENT endpoints, both
// reporting over ssIasZone (0x0500) commandStatusChangeNotification
// (cmd 0x00, zoneStatus bit 0):
//   endpoint 1  → contact    (z2m fz.U02I007C01_contact)
//   endpoint 2  → water_leak (z2m fz.U02I007C01_water_leak)
//
// z2m gates each converter to its endpoint (`if (msg.endpoint.ID !== N)
// return;`). The generic typed IAS converters (kFzIasContactAlarm /
// kFzIasWaterLeakAlarm) fire on WILDCARD_ENDPOINT, so wiring both on the
// same def would cross-fire: a leak report on ep2 would also flip
// `contact`, and vice-versa. These two converters pin `.endpoint` to 1
// and 2 respectively so the dispatcher (`c.endpoint != raw_endpoint`
// gate in dispatch.cpp) routes each zone to exactly one key — matching
// z2m's per-endpoint semantics.
//
// Polarity mirrors z2m exactly:
//   contact    = !(zoneStatus bit0)   (closed door = magnet present = true)
//   water_leak =  (zoneStatus bit0)   (leak detected = true)
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             U02I007C01_contact, U02I007C01_water_leak.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::wally {

// ssIasZone status-change on endpoint 1 → `contact` (inverted bit0).
extern const FzConverter kFzContactEp1;

// ssIasZone status-change on endpoint 2 → `water_leak` (raw bit0).
extern const FzConverter kFzWaterLeakEp2;

}  // namespace zhc::devices::wally
