// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Halo Smart Labs shared converters.
//
// The two Halo SKUs (HALO, HALO+) share an `ssIasZone` (0x0500)
// demuxing strategy that the generic `kFzIasZone` cannot express:
//   - endpoint 1 → smoke / tamper / battery_low / test (zonestatus
//     bit 8) plus mains_power_connected (inverse of AC fault, bit 7)
//   - endpoint 3 → carbon_monoxide
// We expose one converter (`kFzHaloIasZone`) that implements that
// branching exactly the way `haloZoneStatus()` does in
// halo_smart_labs.ts.
//
// All four manufacturer-specific clusters (`haloDeviceStatus` 0xFD00,
// `haloControl` 0xFD01, `haloSensors` 0xFD02, `haloWeather` 0xFD03)
// are NOT yet wired here — `cluster_id_to_name()` does not know about
// them, so any FzConverter selecting on the cluster name would never
// match. Adding those names is outside this file's permitted edit
// scope and tracked in the parity doc.

#pragma once

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::halo_smart_labs {

// ssIasZone (0x0500) decoder, endpoint-aware.
//
// EP1 zoneStatus bits emitted as:
//   bit 0 (alarm_1)        → smoke
//   bit 2 (tamper)         → tamper
//   bit 3 (battery_low)    → battery_low
//   bit 7 (ac_fault)       → mains_power_connected (inverted)
//   bit 8 (test)           → test
// EP3 zoneStatus bits:
//   bit 0 (alarm_1)        → carbon_monoxide
extern const ::zhc::FzConverter kFzHaloIasZone;

}  // namespace zhc::devices::halo_smart_labs
