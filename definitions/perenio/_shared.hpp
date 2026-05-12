// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Perenio shared converters.
//
// Two custom converters live here:
//
//   * `kFzPerenioSmartPlug`  — decodes the manufacturer-specific
//     `perenioSpecific` cluster (id 0xFC7B, manuf 0x007B) used by the
//     PEHPL0X power-link plug. Maps the numeric attribute ids straight
//     to z2m output keys (rms_voltage, active_power, consumed_energy,
//     voltage_min/max, power_max, consumed_energy_limit, default_on_off_state).
//
//   * `kFzPerenioSwitchType` — decodes `genMultistateValue.presentValue`
//     (attr 0x0055) into the `switch_type` enum used by PEHWE20.
//
// `kFzPerenioDiagnostic` for `haDiagnostic` (last_message_lqi/rssi)
// reuses generic patterns; declared here for symmetry.
//
// Toz: per-attribute manuf-specific writeAttributes encoders that
// share `::zhc::generic::tz_zcl_write_attr` via `ZclWriteSpec*`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/perenio.ts.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::perenio {

using ::zhc::FzConverter;
using ::zhc::TzConverter;

// --- fromZigbee ----------------------------------------------------

// `perenioSpecific` (manuSpec 0xFC7B / 0x007B) attribute report.
// Hard-coded attr-id → out-key mapping (see _shared.cpp).
extern const FzConverter kFzPerenioSmartPlug;

// `genMultistateValue` 0x0055 presentValue → "switch_type" string.
extern const FzConverter kFzPerenioSwitchType;

// `haDiagnostic` lastMessageLqi (0x011C) / lastMessageRssi (0x011D).
extern const FzConverter kFzPerenioDiagnostic;

// --- toZigbee ------------------------------------------------------

// genMultistateValue.presentValue write — accepts the switch_type
// enum string, maps to the perenio lookup table, encodes a standard
// (non-manuf) writeAttributes frame.
extern const TzConverter kTzPerenioSwitchType;

// perenioSpecific writes — every spec sets `manufacturer_code = 0x007B`.
extern const TzConverter kTzPerenioDefaultState;     // attr 0  enum8
extern const TzConverter kTzPerenioVoltageMin;       // attr 4  uint16
extern const TzConverter kTzPerenioVoltageMax;       // attr 5  uint16
extern const TzConverter kTzPerenioPowerMax;         // attr 11 uint16
extern const TzConverter kTzPerenioEnergyLimit;      // attr 15 uint16
// Writing any one of {alarm_voltage_min, alarm_voltage_max,
// alarm_power_max, alarm_consumed_energy} clears the alarm latch
// (attr 1 := 0). Wired to all four keys via the same encoder.
extern const TzConverter kTzPerenioAlarmsResetVMin;
extern const TzConverter kTzPerenioAlarmsResetVMax;
extern const TzConverter kTzPerenioAlarmsResetPMax;
extern const TzConverter kTzPerenioAlarmsResetEnergy;

}  // namespace zhc::devices::perenio
