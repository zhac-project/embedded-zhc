// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared KAMI (Yi) vendor converters. Devices pulling from this header
// should carry a `// Tier 2: uses shared kami converters` comment and a
// `// z2m-source:` provenance line.
//
// z2m-source: zigbee-herdsman-converters/src/devices/kami.ts +
//             converters/fromZigbee.ts (KAMI_contact, KAMI_occupancy).

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::kami {

// ── KAMI contact decoder (cluster ssIasZone, raw byte 7) ─────────────
//
// The KAMI N20 contact sensor does NOT report through the standard IAS
// zoneStatus bitmask. z2m's `fz.KAMI_contact` reads the *raw frame
// buffer* on ssIasZone (0x0500) and decodes the contact state from the
// absolute byte at index 7:
//     {contact: msg.data[7] === 0}
// (open = byte7 nonzero -> contact false; closed = byte7 zero ->
//  contact true).
//
// The generic `kFzIasZone` is wrong here — it would publish
// alarm/tamper/battery_low booleans and never the `contact` key. This
// converter emits `contact`.
//
// z2m-source: converters/fromZigbee.ts `KAMI_contact`.
extern const FzConverter kFzKamiContact;

// ── KAMI occupancy/motion decoder (cluster msOccupancySensing) ───────
//
// z2m's `fz.KAMI_occupancy` reads the raw frame buffer on
// msOccupancySensing (0x0406) and emits `action: "motion"` only when the
// absolute byte at index 7 equals 1:
//     if (msg.data[7] === 1) return {action: "motion"};
// (no `occupancy` boolean — the device is modelled as an action button
//  whose only value is "motion").
//
// z2m-source: converters/fromZigbee.ts `KAMI_occupancy`.
extern const FzConverter kFzKamiOccupancy;

}  // namespace zhc::kami
