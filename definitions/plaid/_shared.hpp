// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// PLAID SYSTEMS shared converters.
//
// The Spruce PS-SPRZMS-SLP3 temperature + moisture sensor reports its
// battery on genPowerCfg via the `mainsVoltage` attribute (0x0000, u16,
// raw mV) rather than the usual batteryVoltage (0x0020) / battery-percent
// (0x0021) attributes. z2m's `fz.plaid_battery` reads `mainsVoltage`,
// emits `voltage` directly (no scaling) and derives `battery` (%) from
// meta.battery.voltageToPercentage {min: 2500, max: 3000} — a linear
// toPercentage(voltage, 2500, 3000). The generic kFzBattery reads only
// attrs 0x0020/0x0021/0x0035, so it would not decode this device at all;
// hence the bespoke converter here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/plaid.ts
//             (#PS-SPRZMS-SLP3) and src/converters/fromZigbee.ts
//             (fz.plaid_battery) + src/lib/utils.ts
//             (batteryVoltageToPercentage / toPercentage).

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::plaid {

using ::zhc::FzConverter;

// genPowerCfg `mainsVoltage` (attr 0x0000, u16, raw mV) →
//   voltage = raw  (mV, direct)
//   battery = toPercentage(raw, 2500, 3000)  (%, linear, clamped 0..100)
// Mirrors z2m fz.plaid_battery with meta.battery.voltageToPercentage
// {min: 2500, max: 3000}. Used by the Spruce PS-SPRZMS-SLP3.
extern const FzConverter kFzPlaidBattery;

}  // namespace zhc::devices::plaid
