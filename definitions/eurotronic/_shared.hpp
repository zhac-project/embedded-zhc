// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Eurotronic (Spirit / Comet TRV) converters. Devices that
// include this header should carry a `// Tier 2: uses shared eurotronic
// converters` comment and a `// z2m-source:` provenance line.
//
// All Eurotronic-specific attributes live on cluster `hvacThermostat`
// (0x0201) in the manufacturer-specific id range 0x4000-0x4008. The
// manufacturer code is `Zcl.ManufacturerCode.NXP_SEMICONDUCTORS`
// (0x1037) — z2m reuses NXP because Jennic / NXP made the silicon.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::eurotronic {

// Manufacturer code for every Eurotronic manu-specific write/read.
constexpr std::uint16_t kEurotronicMfgCode = 0x1037;

// ── fz_eurotronic_thermostat ───────────────────────────────────────
//
// Wraps the generic kFzThermostat decoder and additionally extracts
// the Eurotronic manu-specific attrs from `hvacThermostat`:
//
//   0x4000 enum8  trv_mode           → "trv_mode" (1=manual, 2=auto)
//   0x4001 u8     valve_position     → "valve_position" (0..255)
//   0x4002 u32    error_status       → "error_status" (raw bitfield)
//   0x4003 s16    current_heating_setpoint
//                                    → "current_heating_setpoint" (°C, /100)
//   0x4008 map24  host_flags         → derived "child_lock"  (bit 7)
//                                                "mirror_display"(bit 1)
//                                                "system_mode"  (bits 2/4/5)
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             eurotronic_thermostat.
bool fz_eurotronic_thermostat(const DecodedMessage& msg,
                               const FzConverter& self,
                               const PreparedDefinition& def,
                               RuntimeContext& ctx,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzEurotronicThermostat;

// ── manu-specific writeAttributes converters ──────────────────────
//
// Encoded via the data-driven `tz_zcl_write_attr` (fc=0x14, mfg=0x1037)
// for the simple scalar attrs. host_flags / mirror_display / child_lock
// share attr 0x4008 as a packed bitfield and therefore have a custom
// encoder that reads the device's last reported value off the shadow
// payload via the input key — z2m does a real read-then-write, but at
// runtime we only have the proposed scalar so we emit the nominal
// "system mode + child + mirror" composition with bit 0 always set.
//
// z2m-source: zigbee-herdsman-converters/src/converters/toZigbee.ts
//             eurotronic_{trv_mode,valve_position,current_heating_setpoint,
//                         host_flags,mirror_display,child_lock}.

extern const TzConverter kTzEurotronicTrvMode;          // attr 0x4000
extern const TzConverter kTzEurotronicValvePosition;    // attr 0x4001
extern const TzConverter kTzEurotronicCurrentSetpoint;  // attr 0x4003

// host_flags composite — accepts keys "system_mode", "mirror_display",
// "child_lock", "eurotronic_host_flags" and rebuilds the 24-bit map.
extern const TzConverter kTzEurotronicHostFlags;
extern const TzConverter kTzEurotronicMirrorDisplay;
extern const TzConverter kTzEurotronicChildLock;

}  // namespace zhc::eurotronic
