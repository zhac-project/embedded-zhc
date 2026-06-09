// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// NodOn shared converters. Manufacturer code = 0x128B (NODON).
// Cloned from the niko / lumi `_shared.cpp` macro pattern;
// `tz_zcl_write_attr` is the generic encoder declared in
// `_generic/_shared.hpp` and emits fc=0x14 + manu code when the
// `ZclWriteSpec.manufacturer_code` is non-zero.
//
// Covers the manuSpec attributes declared by `nodonModernExtend` in
// z2m's `nodon.ts`:
//   - hvacThermostat 0x4000 trv_mode (enum8)
//   - hvacThermostat 0x4001 valve_position (uint8)
//   - genOnOff       0x0001 impulse_mode_configuration (uint16, ms)
//   - genOnOff       0x1001 switch_type (enum8: bistable/monostable/auto_detect)
//   - closuresWindowCovering 0x0001..0x0004 calibration runtimes (uint16)
//   - closuresWindowCovering 0x1001 switch_type (enum8)
//   - genBinaryInput 0x0055 dry_contact (presentValue) — fz only via kFzNodonDryContact
//   - hvacThermostat 0x4000/0x4001 read path — kFzNodonTrvExtras
//   - closuresWindowCovering 0x0008/0x0009 read path — kFzNodonCoverPositionTilt
//
// Custom 0xFC00 PilotWire cluster (setMode command, `pilot_wire_mode`)
// is NOT wired and is deferred as infra: `cluster_names.hpp` already
// maps 0xFC00 → "manuSpecificPhilips" (a hardcoded collision), and the
// FzConverter dispatch matches on cluster NAME only (no cluster-id
// path), so a NodOn pilot-wire decoder cannot be cleanly targeted
// without cluster-name infra changes. Affects FPS-4-1-00 / SIN-4-FP-20
// / SIN-4-FP-21.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nodon.ts
//             `nodonModernExtend`.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {

using ::zhc::FzConverter;
using ::zhc::TzConverter;

// ── ManuSpec write specs (manu code = 0x128B) ───────────────────────
extern const ::zhc::generic::ZclWriteSpec kSpecImpulseMode;
extern const ::zhc::generic::ZclWriteSpec kSpecSwitchTypeOnOff;
extern const ::zhc::generic::ZclWriteSpec kSpecCalibVertUp;
extern const ::zhc::generic::ZclWriteSpec kSpecCalibVertDown;
extern const ::zhc::generic::ZclWriteSpec kSpecCalibRotUp;
extern const ::zhc::generic::ZclWriteSpec kSpecCalibRotDown;
extern const ::zhc::generic::ZclWriteSpec kSpecSwitchTypeWindowCovering;
extern const ::zhc::generic::ZclWriteSpec kSpecTrvMode;
extern const ::zhc::generic::ZclWriteSpec kSpecValvePosition;

// ── Tz writes (manuSpec) ────────────────────────────────────────────
extern const TzConverter kTzNodonImpulseMode;
extern const TzConverter kTzNodonSwitchTypeOnOff;
extern const TzConverter kTzNodonCalibVertUp;
extern const TzConverter kTzNodonCalibVertDown;
extern const TzConverter kTzNodonCalibRotUp;
extern const TzConverter kTzNodonCalibRotDown;
extern const TzConverter kTzNodonSwitchTypeWindowCovering;
extern const TzConverter kTzNodonTrvMode;
extern const TzConverter kTzNodonValvePosition;

// ── Fz reads (manuSpec attributes when reported back) ───────────────
//
// kFzNodonDryContact reads attr 0x0055 (presentValue, ENUM8) on
// genBinaryInput and emits `dry_contact` ∈ {contact_closed,contact_open}.
extern const FzConverter kFzNodonDryContact;

// kFzNodonTrvExtras reads hvacThermostat attrs 0x4000 (trv_mode, enum8 →
// auto/valve_position_mode/manual) and 0x4001 (valve_position, u8 0..100)
// — the read counterpart to kTzNodonTrvMode / kTzNodonValvePosition,
// which the generic kFzThermostat does not decode. TRV-4-1-00.
extern const FzConverter kFzNodonTrvExtras;

// kFzNodonCoverPositionTilt reads closuresWindowCovering attrs 0x0008
// (position) and 0x0009 (tilt). Replaces the generic lift-only
// kFzCoverPosition on the tilt-capable roller shutters (SIN-4-RS-20[_PRO]).
extern const FzConverter kFzNodonCoverPositionTilt;

}  // namespace zhc::devices::nodon
