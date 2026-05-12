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

}  // namespace zhc::ubisys
