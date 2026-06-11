// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared BYUN gas/smoke-sensor vendor converters. Every device that pulls
// from this header carries a `// Tier 2: uses shared byun converters`
// comment plus a `// z2m-source:` provenance line.
//
// Background. The BYUN M415-5C (gas) and M415-6C (smoke) sensors abuse
// standard clusters in a vendor-specific way; z2m drives them with four
// bespoke fromZigbee converters (byun.ts → fz.byun_{smoke,gas}_{true,false})
// rather than the generic IAS-zone decoder. The generated ports wired the
// generic `kFzIasZone`, which emits the bare keys `alarm`/`tamper`/
// `battery_low` — none of which match the `smoke`/`gas` exposes z2m
// declares (dead-key, gap class c). Worse, the alarm-CLEAR signal arrives
// on entirely different clusters that the generic IAS decoder never reads,
// so the sensor could latch ON forever (dropped channel, gap class b).
//
// z2m semantics, reproduced 1:1:
//   ALARM  (true): ssIasZone (0x0500) commandStatusChangeNotification
//                  (cmd 0x00, server→client), fired ONLY when the raw
//                  zoneStatus word == 33 (0x21 = alarm1|test). Magic
//                  equality, NOT a bit test — so a generic bit0 reader
//                  would over-trigger and also emit phantom tamper/
//                  battery_low. Emits {smoke|gas}: true.
//   CLEAR (false):
//     smoke — pHMeasurement (0x0409) attributeReport, measuredValue
//             (attr 0x0000) == 0 → {smoke: false}.
//     gas   — raw cluster 0x040A (decimal 1034) frame whose first byte
//             (the ZCL frame-control octet) == 0x18 (24) → {gas: false}.
//             z2m's `type:["raw"]` converter reads `msg.data[0]`, which is
//             the whole-frame buffer's frame-control byte.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::byun {

// Selector for the shared alarm-true / alarm-false converters. `key`
// points at the static expose name ("smoke" or "gas").
struct ByunAlarmConfig {
    const char* key;
};

extern const ByunAlarmConfig kCfgSmoke;
extern const ByunAlarmConfig kCfgGas;

// ssIasZone (0x0500) commandStatusChangeNotification: emit `<key> = true`
// iff the raw zoneStatus word == 33 (z2m byun.ts byun_{smoke,gas}_true).
extern const FzConverter kFzByunSmokeTrue;
extern const FzConverter kFzByunGasTrue;

// pHMeasurement (0x0409) attributeReport, measuredValue == 0 → smoke=false
// (z2m byun.ts byun_smoke_false).
extern const FzConverter kFzByunSmokeFalse;

// raw cluster 0x040A frame, frame-control byte == 0x18 → gas=false
// (z2m byun.ts byun_gas_false).
extern const FzConverter kFzByunGasFalse;

}  // namespace zhc::devices::byun
