// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Intuis (Muller Intuitiv) shared converters.
//
// z2m-source: intuis.ts (#intuisradiator) via converters/fromZigbee.ts
//             #thermostat + #hvac_user_interface.
//
// The intuisradiator electric heater wires z2m's fz.thermostat +
// fz.hvac_user_interface + m.occupancy + m.electricityMeter. The
// climate expose declares occupied_heating_setpoint,
// unoccupied_heating_setpoint, running_state([idle,heat]) and
// system_mode([off,heat]); the UI exposes keypad_lockout and
// temperature_display_mode.
//
// The generic kFzThermostat decodes ONLY hvacThermostat 0x0000 /
// 0x0012 / 0x001C, and — critically — emits attr 0x0012 under the key
// "current_heating_setpoint" (the wrong key; the def declares
// "occupied_heating_setpoint"). It does not touch 0x0014
// (unoccupied) or 0x0029 (running_state) at all, and there is no
// generic decoder for hvacUserInterfaceCfg (0x0204). Under the
// auto-generated wiring (kFzMetering + kFzThermostat) the occupied
// setpoint, unoccupied setpoint, running_state, occupancy,
// keypad_lockout and temperature_display_mode were ALL dead.
//
// ── kFzIntuisThermostat (hvacThermostat 0x0201) ──────────────────────
// Standalone decoder (does NOT chain the generic) emitting z2m's keys:
//   0x0000 localTemp                → local_temperature           (raw s16, ×100)
//   0x0012 occupiedHeatingSetpoint  → occupied_heating_setpoint   (raw s16, ×100)
//   0x0014 unoccupiedHeatingSetpoint→ unoccupied_heating_setpoint (raw s16, ×100)
//   0x001C systemMode               → system_mode                 (raw u8 enum)
//   0x0029 runningState             → running_state               (string idle/heat)
// Numeric attrs emit the raw int (downstream applies the unit scale,
// same contract as the generic local_temperature). system_mode keeps
// the raw enum. running_state emits z2m's string form
// (constants.thermostatRunningStates: 0→idle, 1→heat); the device
// exposes only {idle, heat}.
//
// ── kFzIntuisHvacUi (hvacUserInterfaceCfg 0x0204) ────────────────────
// Closes z2m fz.hvac_user_interface for the two attrs this device
// exposes:
//   0x0000 keypadLockout    → keypad_lockout           (string, constants.keypadLockoutMode)
//   0x0001 tempDisplayMode  → temperature_display_mode (string celsius/fahrenheit)
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::intuis {

extern const ::zhc::FzConverter kFzIntuisThermostat;
extern const ::zhc::FzConverter kFzIntuisHvacUi;

}  // namespace zhc::devices::intuis
