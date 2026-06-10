// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Leviton vendor-shared converters. The RC-2000WH (Omnistat2) thermostat
// uses z2m's standard `fz.thermostat`, which publishes the full
// hvacThermostat (0x0201) attribute set. The generic `kFzThermostat`
// decodes only 0x0000 / 0x0012 / 0x001C, so the device's
// `pi_heating_demand` (0x0008), `occupied_cooling_setpoint` (0x0011) and
// `local_temperature_calibration` (0x0010) exposes are otherwise dead.
//
// Unlike Danfoss, Leviton does NOT set `dontMapPIHeatingDemand`, so
// pi_heating_demand is `mapNumberRange(raw, 0, 255, 0, 100)` (round-half-up).
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leviton {

// FzConverter that decodes the standard hvacThermostat attributes the
// generic `kFzThermostat` skips but which the RC-2000WH exposes:
//   * 0x0008 PIHeatingDemand  → pi_heating_demand (0-255 remapped to %)
//   * 0x0010 LocalTemperatureCalibration → local_temperature_calibration (s8)
//   * 0x0011 OccupiedCoolingSetpoint     → occupied_cooling_setpoint (s16)
// The core surface (local_temperature / current_heating_setpoint /
// system_mode) stays on the generic `kFzThermostat`.
extern const ::zhc::FzConverter kFzLevitonThermostat;

}  // namespace zhc::devices::leviton
