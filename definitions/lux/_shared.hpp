// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared LUX vendor converters.
//
// The LUX KONOz KN-Z-WH1-B04 is a standard HVAC thermostat decoded by
// z2m's `fz.thermostat` (hvacThermostat 0x0201) + `fz.fan`
// (hvacFanCtrl 0x0202). Its climate expose declares
// occupied_heating_setpoint / occupied_cooling_setpoint /
// local_temperature / system_mode / running_state / fan_mode.
//
// The generic `kFzThermostat` only decodes hvacThermostat attrs
// 0x0000 (local_temperature), 0x0012 (current_heating_setpoint) and
// 0x001C (system_mode); `kFzFanMode` covers the hvacFanCtrl fan_mode.
// The cooling setpoint and running state were therefore dead exposes.
// This header exposes a vendor-extras converter that delegates to the
// generic thermostat decoder and then adds the two attributes z2m's
// `fz.thermostat` decodes that the generic converter drops:
//
//   attr 0x0011 OccupiedCoolingSetpoint (s16)   → occupied_cooling_setpoint
//   attr 0x0029 ThermostatRunningState (bitmap16) → running_state (string)
//   attr 0x001E ThermostatRunningMode  (enum8)  → running_mode (string)
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.thermostat (occupied_cooling_setpoint / running_state /
//             running_mode branches) + lib/constants.ts
//             thermostatRunningStates / thermostatRunningMode.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::lux {

// ── fz: hvacThermostat extras the generic decoder skips ───────────
extern const FzConverter kFzLuxThermostatExtras;

}  // namespace zhc::lux
