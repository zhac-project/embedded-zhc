// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Stelpro (Quebec) vendor converters. Every device that pulls
// from this header should carry a `// Tier 2: uses shared stelpro
// converters` comment and a `// z2m-source:` provenance line.
//
// Stelpro thermostats use mfgcode 0x1185 and add two manu-specific
// attributes on hvacThermostat (cluster 0x0201):
//   * 0x4001 stelproOutdoorTemp (int16, °C * 100) — write only, drives
//     the small "outside" reading on the device LCD.
//   * 0x401C stelproSystemMode  (u8 enum) — read on attribute report.
//     Value 5 means "Eco"; z2m collapses it to "auto".
//
// Two non-manu vendor attributes also appear on hvacThermostat for the
// HT402 (Hilo edition):
//   * 0x4008 power  (u16, W)
//   * 0x4009 energy (uint, Wh; downstream divides by 1000 → kWh)

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::stelpro {

// hvacThermostat decoder. Wraps the generic thermostat fz with two
// add-ons:
//   * stelproSystemMode (0x401C) == 5 → emits system_mode="auto".
//   * pIHeatingDemand   (0x0008) >= 10 → running_state="heat", else
//     "idle".
// All standard attrs (local_temperature, occupied_heating_setpoint,
// system_mode raw enum) still flow through the generic decoder.
//
// z2m-source: stelpro.ts `fzLocal.stelpro_thermostat`.
extern const FzConverter kFzStelproThermostat;

// hvacThermostat power/energy decoders for HT402.
//
// z2m-source: stelpro.ts `fzLocal.power` / `fzLocal.energy`.
extern const FzConverter kFzStelproPower;
extern const FzConverter kFzStelproEnergy;

// Tz: writes attr 0x4001 stelproOutdoorTemp on hvacThermostat with
// mfgcode 0x1185. Accepts key "outdoor_temperature_display" as numeric
// °C; raw is value * 100 (int16).
//
// z2m-source: stelpro.ts `tzLocal.stelpro_thermostat_outdoor_temperature`.
extern const TzConverter kTzStelproOutdoorTemp;

// Tz: writes the standard "peak demand event icon" command on
// hvacThermostat (HT402 only). Implemented as a `peak_demand_icon`
// numeric write to attribute 0x4012 (u8) with mfgcode 0x1185.
//
// z2m-source: zigbee-herdsman-converters/src/converters/toZigbee.ts
//             `tz.stelpro_peak_demand_event_icon`.
extern const TzConverter kTzStelproPeakDemandIcon;

}  // namespace zhc::stelpro
