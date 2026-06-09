// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Hive vendor converters.
//
// The generic `kFzThermostat` decodes only hvacThermostat (0x0201)
// attrs 0x0000 (local_temperature), 0x0012 (current_heating_setpoint)
// and 0x001C (system_mode). Every Hive heating receiver (SLR1 / SLR1b /
// SLR1c / SLR1d / SLR2 / SLR2b / SLR2c / OTR1) is wired upstream with
// `fz.thermostat`, which ALSO decodes attr 0x0029 (ThermostatRunningState),
// and the z2m exposes carry `.withRunningState(["idle","heat"])`. The
// generic converter drops 0x0029, so `running_state` was missing from the
// shadow on every Hive receiver — a real parity gap.
//
// `kFzHiveThermostatExtras` decodes attr 0x0029 → "running_state" (string),
// mapped through z2m's `constants.thermostatRunningStates`. Hive receivers
// only ever publish heat / idle, but the full low-bit lookup is covered so
// any reported value lands on the right z2m label; unknown values are
// dropped (matching getFromLookup's miss).
//
// Wired ALONGSIDE the generic `kFzThermostat` on each receiver def (which
// still emits local_temperature / current_heating_setpoint / system_mode),
// exactly like the Owon / Ubisys / Danfoss extras pattern.
//
// Devices pulling from this header carry a `// Tier 2: uses shared hive
// converters` comment plus a `// z2m-source:` line.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.thermostat (runningState branch) + lib/constants.ts
//             thermostatRunningStates.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::hive {

// ── fz: thermostat extras the generic decoder skips ───────────────
//
//   attr 0x0029 ThermostatRunningState (map16) → "running_state"
extern const FzConverter kFzHiveThermostatExtras;

}  // namespace zhc::hive
