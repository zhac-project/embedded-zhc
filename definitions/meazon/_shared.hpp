// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared MEAZON vendor converter (BIZY_PLUG metering plug, DINRAIL
// 1-phase meter).
//
// Both devices run z2m's vendor `fz.meazon_meter` converter, NOT the
// standard metering path. `fz.meazon_meter` decodes a set of
// MANUFACTURER-SPECIFIC attributes (MEAZON_S_A) carried INSIDE the
// `seMetering` (0x0702) cluster — proprietary attribute IDs, none of
// which overlap the standard `seMetering` (0x0000 currentSummDelivered /
// 0x0400 instantaneousDemand) or `haElectricalMeasurement` (0x0B04)
// slots. The auto-generated ports wired the generic `kFzMetering`
// (0x0702 attrs 0x0000/0x0400) + `kFzElectricalMeasurement` (0x0B04
// attrs 0x050B/0x0505/0x0508) instead, so EVERY channel
// (power/voltage/current/energy) was a dead expose: the device reports
// power on attr 0x2001, voltage on 0x2004/0x2015, current on
// 0x2007/0x2018 and energy on 0x3000 — the generics never look there.
// The device also reports on NO 0x0B04 cluster at all (z2m binds only
// genOnOff + seMetering on endpoint 10).
//
// `fz.meazon_meter` channel map (proprietary seMetering attrs):
//   0x2001 (8193)  -> power           (W)
//   0x2004 (8196)  -> voltage         (V)   ── primary
//   0x2015 (8213)  -> voltage         (V)   ── alternate slot
//   0x2007 (8199)  -> current         (A)   ── primary
//   0x2018 (8216)  -> current         (A)   ── alternate slot
//   0x3000 (12288) -> energy          (kWh) ── BIZY_PLUG only
//
// z2m applies only `precisionRound(value, 2)` (no scale factor), so all
// channels are RAW pass-through here, matching the generic-converter
// convention (the runtime scales downstream). The reports arrive
// manufacturer-specific (fc bit 0x04 + MEAZON_S_A code); the decoder
// surfaces the attribute payloads transparently and the converter
// matches on cluster + ServerToClient (frame_flags reserved in v1).
//
// Devices pulling from this header carry a `// Tier 2: uses shared
// meazon converters` comment plus a `// z2m-source:` line.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             (meazon_meter) + src/devices/meazon.ts.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::meazon {

// seMetering (0x0702) Meazon manufacturer-specific channels:
// power (0x2001), voltage (0x2004/0x2015), current (0x2007/0x2018),
// energy (0x3000). Replaces the generic kFzMetering +
// kFzElectricalMeasurement pair (which decode standard attrs this
// device never reports).
extern const FzConverter kFzMeazonMeter;

}  // namespace zhc::meazon
