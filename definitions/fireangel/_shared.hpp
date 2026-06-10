// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared FireAngel vendor converters.
//
// Both FireAngel CO products report on the IAS Zone status-change
// notification (ssIasZone cmd 0x00) but with NON-standard bit layouts that
// the generic typed IAS decoders cannot model:
//
//   * W2-Module (zigbeeModel "Alarm_SD_Device", gateway/repeater module)
//     decodes carbon_monoxide from zoneStatus *bit 8* (z2m
//     fz.W2_module_carbon_monoxide: `(zoneStatus & (1 << 8)) > 8`) — not the
//     standard bit 0. The generic kFzIasCoAlarm reads bit 0, so it would
//     never fire here.
//
//   * ZBCO-AE-10X-EUR (fingerprint "Alarm_SD_Device" + manufacturerName
//     "Fireangel") decodes carbon_monoxide/tamper/battery_low on the
//     standard bits 0/2/3 (handled by the generic kFzIasCoAlarm) PLUS a
//     vendor self-test flag from bits 5 OR 9 (z2m fzLocal.fireangel_co_test).
//     z2m latches `test` true and clears it after an 8 s timeout; with no
//     timer available host-side we publish the instantaneous bit state.
//
// z2m-source: zigbee-herdsman-converters/src/devices/fireangel.ts

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::fireangel {

using ::zhc::FzConverter;

// W2-Module: ssIasZone status-change, zoneStatus bit 8 → `carbon_monoxide`.
extern const FzConverter kFzW2ModuleCarbonMonoxide;

// ZBCO-AE-10X-EUR: ssIasZone status-change, `test` = (bit 5) OR (bit 9).
extern const FzConverter kFzFireangelCoTest;

}  // namespace zhc::fireangel
