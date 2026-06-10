// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Imhotep Creation vendor converters.
//
// The generic `kFzThermostat` decodes only hvacThermostat (0x0201)
// attrs 0x0000 (local_temperature), 0x0012 (current_heating_setpoint)
// and 0x001C (system_mode). The Imhotep heater thermostats (E-Ctrl and
// whitelabels) and the BRI4P underfloor-heating bridge also publish the
// further *standard* hvacThermostat attributes that z2m's `fz.thermostat`
// decodes but the generic converter drops — so those were dead exposes:
//
//   attr 0x0011 OccupiedCoolingSetpoint  (s16) → "current_cooling_setpoint"
//   attr 0x0015 MinHeatSetpointLimit     (s16) → "min_heat_setpoint_limit"
//   attr 0x0016 MaxHeatSetpointLimit     (s16) → "max_heat_setpoint_limit"
//   attr 0x0017 MinCoolSetpointLimit     (s16) → "min_cool_setpoint_limit"
//   attr 0x0018 MaxCoolSetpointLimit     (s16) → "max_cool_setpoint_limit"
//
// All five are raw s16 pass-throughs (0.01 °C wire units), mirroring how
// the generic decoder treats the heating setpoint (the runtime scales
// /100 downstream), and matching z2m's `precisionRound(x,2)/100` branches
// in `fz.thermostat`. The cooling-setpoint expose surfaces as
// "current_cooling_setpoint" (z2m `.withSetpoint("occupied_cooling_setpoint")`),
// exactly like the Owon extras pattern.
//
// Wired ALONGSIDE the generic `kFzThermostat` on the E-Ctrl + BRI4P defs
// (which still emit local_temperature / current_heating_setpoint /
// system_mode), exactly like the Owon / Ubisys / Danfoss extras pattern.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.thermostat (occupiedCoolingSetpoint + min/maxHeatSetpointLimit
//             + min/maxCoolSetpointLimit branches).

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::imhotepcreation {

// ── fz: thermostat extras the generic decoder skips ───────────────
//
//   attr 0x0011 OccupiedCoolingSetpoint → "current_cooling_setpoint"
//   attr 0x0015 MinHeatSetpointLimit    → "min_heat_setpoint_limit"
//   attr 0x0016 MaxHeatSetpointLimit    → "max_heat_setpoint_limit"
//   attr 0x0017 MinCoolSetpointLimit    → "min_cool_setpoint_limit"
//   attr 0x0018 MaxCoolSetpointLimit    → "max_cool_setpoint_limit"
extern const FzConverter kFzImhotepThermostatExtras;

}  // namespace zhc::imhotepcreation
