// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Ubisys vendor converters / write specs.
//
// Ubisys industrial-grade modules use three manufacturer-specific
// clusters anchored at manufacturer code 0x10F2 ("Ubisys
// Technologies"):
//
//   0xFC00 manuSpecificUbisysDeviceSetup
//          attr 0x0000 inputConfigurations  (ARRAY, write)
//          attr 0x0001 inputActions         (ARRAY, write)
//          NOTE: per the upstream lib comment, this cluster does NOT
//          actually use a manufacturer-code envelope (z2m sets it to
//          null on writes — historical zigbee-herdsman quirk).
//
//   0xFC01 manuSpecificUbisysDimmerSetup    (mfgcode 0x10F2)
//          attr 0x0000 capabilities  BITMAP8
//          attr 0x0001 status        BITMAP8
//          attr 0x0002 mode          BITMAP8
//
//   0x0201 hvacThermostat — Ubisys-extended attributes (mfgcode 0x10F2)
//          attr 0x0010 ubisysTemperatureOffset             INT8
//          attr 0x0012 ubisysVacationMode                  BOOLEAN
//          attr 0x0013 ubisysRemoteTemperature             INT16
//          attr 0x0014 ubisysRemoteTemperatureValidDuration UINT16
//
// Only writes are exposed as TzConverters here — the array-typed
// inputConfigurations/inputActions writes use opaque key=hex-string
// payloads (see fz/tz_configure_device_setup in upstream ubisys.ts);
// they are tracked as PARTIAL in UBISYS_PARITY.md.
//
// Devices that pull from this header should carry a `// Tier 2: uses
// shared ubisys converters` comment and a `// z2m-source:` line.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::ubisys {

// Manufacturer code for the UBISYS_TECHNOLOGIES_GMBH ZCL envelope.
constexpr std::uint16_t kMfgCode = 0x10F2;

// ── DeviceSetup (0xFC00) write specs ──────────────────────────────
// `inputActions` and `inputConfigurations` are ARRAY (0x48) typed in
// the protocol. The generic `tz_zcl_write_attr` cannot encode arrays
// from a scalar `Value`, so these are placeholders — opaque ARRAY
// blob writes need a vendor-specific encoder (see PARITY doc §gaps).
//
// The TzConverter targets remain registered so devices advertise the
// keys; the encoder rejects non-trivial payloads at runtime.

// ── DimmerSetup (0xFC01) ──────────────────────────────────────────
extern const TzConverter kTzUbisysDimmerCapabilities; // "capabilities"  attr 0x0000
extern const TzConverter kTzUbisysDimmerStatus;       // "status"        attr 0x0001
extern const TzConverter kTzUbisysDimmerMode;         // "mode"          attr 0x0002

// ── hvacThermostat (0x0201) — Ubisys H1/H10 extensions ────────────
extern const TzConverter kTzUbisysLocalTempOffset;        // INT8  0x0010
extern const TzConverter kTzUbisysVacationMode;           // BOOL  0x0012
extern const TzConverter kTzUbisysRemoteTemperature;      // INT16 0x0013
extern const TzConverter kTzUbisysRemoteTempDuration;     // UINT16 0x0014

// ── fz: dimmer-setup status/capabilities decoder ──────────────────
//
// Decodes a BITMAP8 attribute report on cluster manuSpecificUbisys-
// DimmerSetup (0xFC01), attribute 0x0000 (capabilities) / 0x0001
// (status) / 0x0002 (mode), and emits z2m-style boolean keys per the
// bit map laid out in upstream ubisys.ts fz.dimmer_setup. Capabilities
// bits (per technical reference):
//   bit 0  → forwardPhaseControl
//   bit 1  → reversePhaseControl
//   bit 5  → reactanceDiscriminator
//   bit 6  → configurableCurve
//   bit 7  → overloadDetection
//
// z2m-source: zigbee-herdsman-converters/src/devices/ubisys.ts
//             ubisys.fz.dimmer_setup.
extern const FzConverter kFzUbisysDimmerSetup;

// ── fz: thermostat extras the generic decoder skips ───────────────
//
// The generic `kFzThermostat` only decodes hvacThermostat (0x0201)
// attrs 0x0000 (local_temperature), 0x0012 (current_heating_setpoint)
// and 0x001C (system_mode). The Ubisys H1 / H10 climate exposes also
// publish two standard attributes that z2m's `fz.thermostat` decodes
// but the generic converter drops, so they were dead exposes:
//
//   attr 0x0008 PIHeatingDemand   (u8)  → "pi_heating_demand" (%)
//   attr 0x001E ThermostatRunningMode (enum8) → "running_mode" (string)
//
// Unlike Danfoss, Ubisys does NOT set `dontMapPIHeatingDemand`, so the
// raw 0-255 value is remapped to 0-100 % (z2m `mapNumberRange`), using
// the canonical half-up rounding `(v*100 + 127) / 255`.
//
// `running_mode` is mapped to z2m's `constants.thermostatRunningMode`
// strings: 0→"off", 3→"cool", 4→"heat".
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.thermostat (pi_heating_demand + running_mode branches).
extern const FzConverter kFzUbisysThermostatExtras;

// ── fz: cover lift + tilt ─────────────────────────────────────────
//
// The generic `kFzCoverPosition` decodes only closuresWindowCovering
// (0x0102) attr 0x0008 (CurrentPositionLiftPercentage → "position").
// The J1 shutter controller also reports attr 0x0009
// (CurrentPositionTiltPercentage), which z2m's `fz.cover_position_tilt`
// publishes as "tilt". This converter emits both (raw 0-100; the J1
// reports 0xFF / 255 when a position is unknown — those are skipped,
// matching the z2m `<= 100` guard).
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             fz.cover_position_tilt.
extern const FzConverter kFzUbisysCoverPositionTilt;

}  // namespace zhc::ubisys
