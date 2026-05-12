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
//
// Custom 0xFC00 PilotWire cluster (setMode command, `pilot_wire_mode`)
// is not yet wired here; left as TODO for a follow-up port.
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

}  // namespace zhc::devices::nodon
