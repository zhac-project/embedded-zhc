// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Owon vendor converters.
//
// The generic `kFzThermostat` decodes only hvacThermostat (0x0201)
// attrs 0x0000 (local_temperature), 0x0012 (current_heating_setpoint)
// and 0x001C (system_mode). Several Owon HVAC controllers / fan-coils
// (AC201, AC221, PCT504, PCT512) also publish two further *standard*
// hvacThermostat attributes that z2m's `fz.thermostat` decodes but the
// generic converter drops — so those were dead exposes:
//
//   attr 0x0011 OccupiedCoolingSetpoint (s16) → "current_cooling_setpoint"
//   attr 0x0029 ThermostatRunningState (bitmap16) → "running_state" (string)
//
// `current_cooling_setpoint` is a raw s16 pass-through (0.01 °C wire
// units), mirroring how the generic decoder treats the heating
// setpoint (the runtime scales /100 downstream).
//
// `running_state` is mapped to z2m's `constants.thermostatRunningStates`
// strings. Owon devices only ever run heat/cool/idle/fan_only, so the
// common low bits are covered: 0→"idle", 1→"heat", 2→"cool",
// 4→"fan_only". Unknown values are dropped (matching getFromLookup's
// miss). This matches z2m's lookup on the running-state bitmap.
//
// Wired ALONGSIDE the generic `kFzThermostat` on each thermostat def
// (which still emits local_temperature / current_heating_setpoint /
// system_mode), exactly like the Ubisys / Danfoss extras pattern.
//
// Devices pulling from this header carry a `// Tier 2: uses shared
// owon converters` comment plus a `// z2m-source:` line.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.thermostat (occupied_cooling_setpoint + running_state
//             branches) + lib/constants.ts thermostatRunningStates.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::owon {

// ── fz: thermostat extras the generic decoder skips ───────────────
//
//   attr 0x0011 OccupiedCoolingSetpoint (s16) → "current_cooling_setpoint"
//   attr 0x0029 ThermostatRunningState (enum/bitmap) → "running_state"
extern const FzConverter kFzOwonThermostatExtras;

}  // namespace zhc::owon
