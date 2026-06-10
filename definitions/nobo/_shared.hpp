// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nobø (Glen Dimplex) shared converters.
//
// z2m-source: nobo.ts (#SWT-IZ / #SPC-IZ) via converters/fromZigbee.ts
//             #thermostat (the single fz wired by m.thermostat).
//
// Both Nobø devices (SWT-IZ floor-heating thermostat, SPC-IZ panel-
// heater control unit) use m.thermostat({systemMode, runningMode,
// setpoints:{occupiedHeatingSetpoint, unoccupiedHeatingSetpoint},
// localTemperature}). m.thermostat wires exactly one fromZigbee —
// fz.thermostat — and exposes a climate surface with:
//   local_temperature, occupied_heating_setpoint,
//   unoccupied_heating_setpoint, system_mode([off,heat]),
//   running_mode([off,heat]).
//
// The generic kFzThermostat decodes ONLY hvacThermostat 0x0000 /
// 0x0012 / 0x001C, and emits attr 0x0012 under the key
// "current_heating_setpoint" — the WRONG key (the def declares
// "occupied_heating_setpoint"). It never touches 0x0014 (unoccupied
// setpoint) or 0x001E (running_mode). Under the auto-generated stub
// (kFzBattery + kFzOnOff — a wholly wrong on/off+battery class) the
// entire thermostat surface was dead: no temperature, no setpoints,
// no mode.
//
// ── kFzNoboThermostat (hvacThermostat 0x0201) ────────────────────────
// Standalone decoder (does NOT chain the generic) emitting z2m's keys:
//   0x0000 localTemp                → local_temperature           (raw s16, ×100)
//   0x0012 occupiedHeatingSetpoint  → occupied_heating_setpoint   (raw s16, ×100)
//   0x0014 unoccupiedHeatingSetpoint→ unoccupied_heating_setpoint (raw s16, ×100)
//   0x001C systemMode               → system_mode                 (raw u8 enum)
//   0x001E runningMode              → running_mode                (string off/heat)
// Numeric attrs emit the raw int (downstream applies the unit scale,
// same contract as the generic local_temperature). system_mode keeps
// the raw enum (same contract as the generic). running_mode emits
// z2m's string form (constants.thermostatRunningMode: 0→off, 3→cool,
// 4→heat); these devices expose only {off, heat}.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nobo {

extern const ::zhc::FzConverter kFzNoboThermostat;

}  // namespace zhc::devices::nobo
