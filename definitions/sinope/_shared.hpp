// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Sinope vendor converters. Devices that pull from this header
// should carry a `// Tier 2: uses shared sinope converters` comment
// and a `// z2m-source:` provenance line.
//
// Two manufacturer-specific clusters are modelled:
//   - manuSpecificSinope (0xFF01) — display/eco/load/keypad/backlight
//   - hvacThermostat    (0x0201) manuSpec attributes 0x0400-0x0404
//
// All writes use mfgcode 0x119C (SINOPE_TECHNOLOGIES) and tunnel
// through the generic `tz_zcl_write_attr` encoder (fc=0x14, mfg
// bytes prepended automatically when `manufacturer_code != 0`).

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::sinope {

// Decode `manuSpecificSinope` (0xFF01) attribute reports. Switches
// on attribute id and emits canonical z2m output keys:
//
//   0x0002 keypadLockout              → "keypad_lockout" (unlock|lock)
//   0x000A timeFormatToDisplay        → "time_format" (24h|12h)
//   0x0010 outdoorTempToDisplay       → "thermostat_outdoor_temperature" (×0.01)
//   0x0011 outdoorTempToDisplayTimeout → "outdoor_temperature_timeout"
//   0x0012 secondScreenBehavior       → "second_display_mode" (auto|setpoint|outdoor temp)
//   0x0050 connectedLoad              → "connected_load"
//   0x0070 auxConnectedLoad           → "aux_connected_load" (65535 → "disabled")
//   0x0102 GFCiStatus                 → "gfci_status" (off|on)
//   0x0105 floorControlMode           → "floor_control_mode" (ambiant|floor)
//   0x0107 floorTemperature           → "floor_temperature" (×0.01)
//   0x010C floorLimitStatus           → "floor_limit_status" (off|on)
//   0x010D roomTemperature            → "room_temperature" (×0.01)
//
// z2m-source: zigbee-herdsman-converters/src/devices/sinope.ts
//             fzLocal.sinope.
bool fz_sinope_manu_specific(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Decode the manuSpecific attributes that Sinope adds to the standard
// `hvacThermostat` (0x0201) cluster:
//
//   0x0400 sinopeOccupancy       → "thermostat_occupancy" (unoccupied|occupied)
//   0x0401 sinopeMainCycleOutput → "main_cycle_output" (15s|5min|10min|15min|20min|30min)
//   0x0402 sinopeBacklight       → "backlight_auto_dim" (on_demand|sensing)
//   0x0404 sinopeAuxCycleOutput  → "aux_cycle_output"
//
// z2m-source: zigbee-herdsman-converters/src/devices/sinope.ts
//             fzLocal.thermostat (sinope-specific block).
bool fz_sinope_hvac_manu(const DecodedMessage& msg,
                          const FzConverter& self,
                          const PreparedDefinition& def,
                          RuntimeContext& ctx,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Sentinel FzConverters — bind these into the device's `kFz` array.
extern const FzConverter kFzSinopeManu;       // cluster 0xFF01
extern const FzConverter kFzSinopeHvacManu;   // cluster 0x0201, manuSpec attrs

// Manu-specific TzConverters (writeAttributes with mfgcode 0x119C).
// All wired through `::zhc::generic::tz_zcl_write_attr`.
//
// manuSpecificSinope (0xFF01):
extern const TzConverter kTzSinopeKeypadLockout;          // "keypad_lockout"
extern const TzConverter kTzSinopeTimeFormat;             // "time_format"
extern const TzConverter kTzSinopeSecondDisplayMode;      // "second_display_mode"
extern const TzConverter kTzSinopeOutdoorTemperature;     // "thermostat_outdoor_temperature"
extern const TzConverter kTzSinopeOutdoorTempTimeout;     // "outdoor_temperature_timeout"
extern const TzConverter kTzSinopeConnectedLoad;          // "connected_load"
extern const TzConverter kTzSinopeAuxConnectedLoad;       // "aux_connected_load"
extern const TzConverter kTzSinopeEcoMode;                // "eco_mode"
extern const TzConverter kTzSinopeFloorControlMode;       // "floor_control_mode"
//
// hvacThermostat (0x0201) manuSpec:
extern const TzConverter kTzSinopeOccupancy;              // "thermostat_occupancy"
extern const TzConverter kTzSinopeMainCycleOutput;        // "main_cycle_output"
extern const TzConverter kTzSinopeBacklightAutoDim;       // "backlight_auto_dim"
extern const TzConverter kTzSinopeAuxCycleOutput;         // "aux_cycle_output"

}  // namespace zhc::sinope
