// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Plugwise vendor converters. Manufacturer code 0x1172
// (Zcl.ManufacturerCode.PLUGWISE_B_V). Mirrors `plugwiseExtend.*`
// from z2m `src/devices/plugwise.ts` — there is no `lib/plugwise.ts`,
// the extend functions are inlined in that file.
//
// The Plugwise heating family (106-03 Tom TRV, 170-01 Emma) places
// vendor-specific attributes on the standard `hvacThermostat`
// (0x0201) cluster. Some attributes use the low 0x40xx range without
// a per-attribute `manufacturerCode` in the cluster definition, but
// every tz path in z2m still passes `{manufacturerCode: PLUGWISE_B_V}`
// through the writeAttributes call — so all writes here are mfg-spec.
//
// Pattern is cloned from `definitions/danfoss/_shared.{hpp,cpp}`:
//   * `tz_zcl_write_attr` (in `_generic/_shared.cpp`) encodes the
//     fc=0x14 + mfg-code header — we just hand it a `ZclWriteSpec`.
//   * `fz_plugwise_thermostat` walks the decoded payload and emits
//     human-readable keys for the Plugwise attribute IDs that the
//     generic `kFzThermostat` ignores (it only handles 0x0000 /
//     0x0008 / 0x0012 / 0x001C).
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::plugwise {

// Decoder for Plugwise hvacThermostat attribute reports. Covers both
// the manufacturer-specific 0x40xx/0xF0xx attributes AND the standard
// ZCL attributes that the generic kFzThermostat skips (it only handles
// 0x0000 / 0x0012 / 0x001C):
//   0x0001 OutdoorTemperature (INT16)           -> "outdoor_temperature"
//   0x0008 PIHeatingDemand (UINT8, 0..100)      -> "pi_heating_demand"
//   0x0010 LocalTemperatureCalibration (INT8)   -> "local_temperature_calibration"
//   0x0011 OccupiedCoolingSetpoint (INT16)      -> "occupied_cooling_setpoint"
//   0x0029 ThermostatRunningState (MAP16)       -> "running_state" (idle/heat/cool/fan_only)
// Manufacturer-specific (mfg 0x1172) attributes:
//   0x4001 plugwiseValvePosition (UINT8)        -> "valve_position"
//   0x4002 (raw)                                -> "error_status"
//   0x4003 plugwiseCurrentHeatingSetpoint INT16 -> "current_heating_setpoint" (/100)
//   0x4008 plugwiseTDiff INT16                  -> "plugwise_t_diff"
//   0x4012 plugwisePushForce UINT32             -> "force" (lookup -> "standard"/"high"/"very_high")
//   0x4014 plugwiseRadioStrength BOOLEAN        -> "radio_strength" ("normal"/"high")
//   0xF000 plugwiseExternalHeatDemand UINT16    -> "external_heat_demand"
//   0xF001 plugwiseExternalHeatDemandTimeout    -> "external_heat_demand_timeout"
//   0xF002 plugwiseBoilerWaterTemp INT16        -> "boiler_water_temperature" (/100)
//   0xF003 plugwiseDhwTemp INT16                -> "dhw_temperature" (/100)
//   0xF004 plugwiseReturnWaterTemp INT16        -> "return_water_temperature" (/100)
//   0xF005 plugwiseApplicationFaultCode BITMAP8 -> "application_fault_code"
//   0xF006 plugwiseOemFaultCode UINT8           -> "oem_fault_code"
//   0xF007 plugwiseMaxDhwSetpoint INT16         -> "max_dhw_setpoint" (/100)
//   0xF008 plugwiseMaxBoilerSetpoint INT16      -> "max_boiler_setpoint" (/100)
extern const ::zhc::FzConverter kFzPlugwiseThermostat;

// Per-attribute manuSpec writers (writeAttributes, fc=0x14, mfg=0x1172):
extern const ::zhc::TzConverter kTzPlugwiseValvePosition;          // 0x4001 UINT8
extern const ::zhc::TzConverter kTzPlugwisePushForce;              // 0x4012 UINT32, "force" string lookup
extern const ::zhc::TzConverter kTzPlugwiseRadioStrength;          // 0x4014 BOOLEAN, "radio_strength" string lookup
extern const ::zhc::TzConverter kTzPlugwiseExternalHeatDemand;     // 0xF000 UINT16
extern const ::zhc::TzConverter kTzPlugwiseExternalHeatDemandTo;   // 0xF001 UINT16
extern const ::zhc::TzConverter kTzPlugwiseMaxDhwSetpoint;         // 0xF007 INT16
extern const ::zhc::TzConverter kTzPlugwiseMaxBoilerSetpoint;      // 0xF008 INT16

// Cluster-specific command 0xA0 plugwiseCalibrateValve. Empty payload,
// mfg-specific (fc=0x15, mfg=0x1172). Listens to the "calibrate_valve"
// expose key — the value is ignored (z2m-side only sends a binary
// "calibrate" trigger and updates the state mirror on convertSet).
extern const ::zhc::TzConverter kTzPlugwiseCalibrateValve;

}  // namespace zhc::devices::plugwise
