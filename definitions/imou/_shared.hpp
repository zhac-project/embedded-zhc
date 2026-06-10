// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared IMOU (Dahua) vendor converters.
//
// IMOU battery sensors decode entirely off ssIasZone (0x0500). Two of
// them need behaviour the generic typed-IAS converters can't express,
// so they live here:
//
//   ZGA1-EN gas detector — z2m `m.iasZoneAlarm({zoneType:"gas",
//     zoneAttributes:["alarm_1","alarm_2","tamper","test"]})`. Because
//     BOTH alarm bits are requested, z2m emits the split keys
//     `gas_alarm_1` (bit 0) + `gas_alarm_2` (bit 1) — not the collapsed
//     `gas` key the generic kFzIasGasAlarm{,2} produce. It also surfaces
//     `test` (bit 8). kFzImouGasAlarm reproduces that exact payload.
//
//   ZE1-EN wireless switch — z2m `imouAlarmButton()` decodes an IAS Zone
//     Status Change Notification and emits `action:"press"` ONLY when
//     zoneStatus === 2. kFzImouAlarmButton reproduces that.
//
// z2m-source: zigbee-herdsman-converters/src/devices/imou.ts
//             (`imouAlarmButton()` + the ZGA1-EN `m.iasZoneAlarm` call) +
//             lib/modernExtend.ts `iasZoneAlarm`.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::imou {

// ── ZGA1-EN gas alarm (ssIasZone status-change) ───────────────────────
//
// zoneStatus bit map → payload keys:
//   bit 0 → "gas_alarm_1"  (Bool)
//   bit 1 → "gas_alarm_2"  (Bool)
//   bit 2 → "tamper"       (Bool)
//   bit 8 → "test"         (Bool)
//
// Mirrors z2m's `bothAlarms` branch in `iasZoneAlarm`: two split alarm
// keys instead of the collapsed `gas`. No battery_low (not requested by
// the z2m zoneAttributes for ZGA1-EN).
bool fz_imou_gas_alarm(const DecodedMessage& msg,
                        const FzConverter& self,
                        const PreparedDefinition& def,
                        RuntimeContext& ctx,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzImouGasAlarm;

// ── ZE1-EN wireless switch (ssIasZone status-change → action) ─────────
//
// Emits `action = "press"` only when the reported zoneStatus == 2
// (matches z2m `imouAlarmButton()`). Any other status produces no
// payload, so a cleared/idle report does not spuriously fire.
bool fz_imou_alarm_button(const DecodedMessage& msg,
                           const FzConverter& self,
                           const PreparedDefinition& def,
                           RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzImouAlarmButton;

}  // namespace zhc::devices::imou
