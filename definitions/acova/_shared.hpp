// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova shared converters.
//
// z2m-source: acova.ts (ALCANTARA2 / TAFFETAS2/PERCALE2 / IHC-Enki).
//
// All three Acova electric-radiator thermostats route inbound through
// z2m's fz.thermostat, which decodes the full hvacThermostat surface.
// The generic kFzThermostat only handles 0x0000 / 0x0012 / 0x001C and —
// critically — emits attr 0x0012 under the key "current_heating_setpoint",
// whereas the Acova exposes declare "occupied_heating_setpoint" (z2m
// e.climate().withSetpoint("occupied_heating_setpoint", ...) +
// fz.thermostat emits "occupied_heating_setpoint"). So under the generic
// decoder the occupied setpoint, the unoccupied setpoint, running_state
// and local_temperature_calibration were ALL dead keys.
//
// kFzAcovaThermostatExtras decodes the exact attrs the Acova family
// exposes, with z2m's keys:
//   0x0000 localTemp                  → local_temperature              (raw s16, ×100)
//   0x0010 localTemperatureCalibration→ local_temperature_calibration (raw s16, ×10)
//   0x0012 occupiedHeatingSetpoint    → occupied_heating_setpoint      (raw s16, ×100)
//   0x0014 unoccupiedHeatingSetpoint  → unoccupied_heating_setpoint    (raw s16, ×100)
//   0x001C systemMode                 → system_mode                    (raw u8 enum)
//   0x0029 runningState               → running_state                  (string idle/heat)
//
// Numeric attrs emit the raw int (downstream applies the unit scale, the
// same contract as the generic local_temperature). system_mode keeps the
// raw enum (same contract as the generic decoder). running_state emits
// z2m's string form (matching fz.thermostat getFromLookup); the Acova
// devices declare only {idle, heat}.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::acova {

extern const ::zhc::FzConverter kFzAcovaThermostatExtras;

}  // namespace zhc::devices::acova
