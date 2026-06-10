// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Airzone Aidoo vendor converters.
//
// The generic `kFzThermostat` decodes only hvacThermostat (0x0201)
// attrs 0x0000 (local_temperature), 0x0012 (current_heating_setpoint)
// and 0x001C (system_mode). The Airzone Aidoo AZAI6ZBEMHI AC controller
// also publishes a cooling setpoint that z2m's `fz.thermostat` decodes
// but the generic converter drops — so that expose was dead:
//
//   attr 0x0011 OccupiedCoolingSetpoint (s16) → "current_cooling_setpoint"
//
// `current_cooling_setpoint` is a raw s16 pass-through (0.01 °C wire
// units), mirroring how the generic decoder treats the heating setpoint
// (the runtime scales /100 downstream).
//
// The z2m device exposes neither running_state nor running_mode (its
// `e.climate()` chain only adds localTemperature/systemMode/fanMode and
// the two setpoints), so this extras converter is intentionally limited
// to the cooling setpoint — unlike the Owon extras which also map
// running_state.
//
// Wired ALONGSIDE the generic `kFzThermostat` on the AZAI6ZBEMHI def
// (which still emits local_temperature / current_heating_setpoint /
// system_mode), and the generic `kFzFanMode` (fan_mode) + `kFzOnOff`.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.thermostat (occupied_cooling_setpoint branch).

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::airzone_aidoo {

// ── fz: thermostat extras the generic decoder skips ───────────────
//   attr 0x0011 OccupiedCoolingSetpoint (s16) → "current_cooling_setpoint"
extern const FzConverter kFzAirzoneThermostatExtras;

}  // namespace zhc::airzone_aidoo
