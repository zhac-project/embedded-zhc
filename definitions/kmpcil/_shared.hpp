// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// KMPCIL shared converters. The vendor's two devices (RES005 environment
// sensor, KMPCIL-tag-001 arrival sensor) deviate from generic Tier-1 in
// two ways that ship as bespoke converters here:
//
//   1. RES005 carries on/off state on `genBinaryOutput` attr 0x0055
//      (presentValue) instead of `genOnOff`. Both fz and tz handle
//      writeAttributes against attr 0x55 with the BOOLEAN ZCL type 0x10.
//
//   2. KMPCIL-tag-001 packs three bool channels (power_state /
//      vibration / occupancy) into the bottom three bits of
//      `genBinaryInput` attr 0x0055 (presentValue). z2m's
//      `kmpcilConverters.presence_binary_input` decoder unpacks these
//      bits; we mirror it here. The presence-timeout side-channel that
//      z2m schedules with `setTimeout` is not portable to embedded —
//      we emit `presence = true` per frame and let downstream rules /
//      the WebUI decide on a host-side stale-frame timeout.
//
// z2m-source: zigbee-herdsman-converters/src/devices/kmpcil.ts and
//             src/converters/{from,to}Zigbee.ts (kmpcil_res005_*).

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::kmpcil {

using ::zhc::FzConverter;
using ::zhc::TzConverter;

// genBinaryOutput presentValue (attr 0x55) → state bool.
// Used by RES005 ("OFF"/"ON" via presentValue 0/1).
extern const FzConverter kFzKmpcilRES005OnOff;

// genBinaryInput presentValue (attr 0x55) → occupancy bool.
// Used by RES005 (presentValue 1 → occupied).
extern const FzConverter kFzKmpcilRES005Occupancy;

// genBinaryInput presentValue (attr 0x55) → power_state | vibration |
// occupancy + presence=true. Used by KMPCIL-tag-001.
extern const FzConverter kFzKmpcilPresenceBinaryInput;

// genPowerCfg batteryVoltage (attr 0x20) → voltage(mV). Companion to the
// presence_binary_input decoder; emits voltage in mV like kFzBattery
// does, minus the percentage estimate (KMPCIL doesn't ship the
// `voltageToPercentage` table on-device).
extern const FzConverter kFzKmpcilPresencePower;

// state ∈ {ON, OFF, TOGGLE} → writeAttributes genBinaryOutput attr 0x55
// (BOOLEAN, type 0x10). Used by RES005.
extern const TzConverter kTzKmpcilRES005OnOff;

}  // namespace zhc::devices::kmpcil
