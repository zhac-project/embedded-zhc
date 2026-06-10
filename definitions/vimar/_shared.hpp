// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Vimar vendor-shared converters. The 02973.B IoT thermostat uses z2m's
// standard `fz.thermostat`, which publishes the full hvacThermostat
// (0x0201) attribute set. The generic `kFzThermostat` decodes only
// 0x0000 / 0x0012 / 0x001C, so the device's `occupied_cooling_setpoint`
// (0x0011) expose — declared by z2m's `e.climate().withSetpoint(
// "occupied_cooling_setpoint", …)` — is otherwise dead.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vimar {

// FzConverter that decodes the standard hvacThermostat cooling-setpoint
// attribute the generic `kFzThermostat` skips but which the 02973.B
// exposes:
//   * 0x0011 OccupiedCoolingSetpoint → occupied_cooling_setpoint (s16)
// The core surface (local_temperature / current_heating_setpoint /
// system_mode) stays on the generic `kFzThermostat`, wired alongside.
extern const ::zhc::FzConverter kFzVimarThermostatExtras;

}  // namespace zhc::devices::vimar
