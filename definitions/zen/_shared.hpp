// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Zen vendor-shared converters. The Zen-01-W thermostat uses z2m's standard
// `fz.thermostat`, which publishes the full hvacThermostat (0x0201) attribute
// set. The generic `kFzThermostat` decodes only 0x0000 / 0x0012 / 0x001C, so
// the device's `occupied_cooling_setpoint` (0x0011), `running_state` (0x0029)
// and `local_temperature_calibration` (0x0010) exposes are otherwise dead.
//
// Pattern cloned from `definitions/leviton/_shared.hpp`. The core surface
// (local_temperature / current_heating_setpoint / system_mode) stays on the
// generic `kFzThermostat`; fan_mode stays on the generic `kFzFanMode`.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::zen {

// FzConverter that decodes the standard hvacThermostat attributes the generic
// `kFzThermostat` skips but which the Zen-01-W exposes via z2m fz.thermostat:
//   * 0x0010 LocalTemperatureCalibration → local_temperature_calibration (s8)
//   * 0x0011 OccupiedCoolingSetpoint     → occupied_cooling_setpoint (s16)
//   * 0x0029 RunningState                → running_state (u16 enum, raw)
extern const ::zhc::FzConverter kFzZenThermostat;

}  // namespace zhc::devices::zen
