// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Legrand (Céliane / Drivia / Netatmo / Bticino) vendor
// converters. Devices that pull from this header should carry a
// `// Tier 2: uses shared legrand converters` comment and a
// `// z2m-source:` provenance line.
//
// Manufacturer code: 0x1021 (LEGRAND_GROUP).
//
// Three Legrand-specific clusters are covered:
//   0xFC01 manuSpecificLegrandDevices
//     attr 0x0000 deviceMode    (DATA16)  — dimmer/switch/pilot mode
//     attr 0x0001 ledInDark     (BOOLEAN) — exposed as `led_in_dark`
//     attr 0x0002 ledIfOn       (BOOLEAN) — exposed as `led_if_on`
//   0xFC40 manuSpecificLegrandDevices2
//     attr 0x0000 pilotWireMode (ENUM8)   — exposed as `pilot_wire_mode`
//     cmd  0x00   command0(buffer)        — write pilot wire mode
//   0xFC41 manuSpecificLegrandDevices3
//     (declared in z2m but no shared attrs in current converters)

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::legrand {

// Manufacturer code for Legrand-group writes/reads.
constexpr std::uint16_t kMfgLegrand = 0x1021;

// ── fz_legrand_cluster_fc01 ──────────────────────────────────────────
//
// Decode `manuSpecificLegrandDevices` (0xFC01) attribute reports.
// Emits `led_in_dark` and `led_if_on` as canonical "ON"/"OFF" strings
// to match z2m's `fzLegrand.cluster_fc01` output shape. `deviceMode`
// is intentionally not decoded here because z2m uses a separate
// per-device converter to coerce the DATA16 into named labels.
//
// z2m-source: zigbee-herdsman-converters/src/lib/legrand.ts
//             fzLegrand.cluster_fc01.
bool fz_cluster_fc01(const DecodedMessage& msg,
                      const FzConverter& self,
                      const PreparedDefinition& def,
                      RuntimeContext& ctx,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Sentinel converter that picks up genBasic-style attribute reports
// on cluster 0xFC01.
extern const FzConverter kFzClusterFc01;

// ── fz_legrand_pilot_wire_mode ──────────────────────────────────────
//
// Decode `manuSpecificLegrandDevices2` (0xFC40) `pilotWireMode`
// (attr 0x0000 ENUM8) into a `pilot_wire_mode` string. Lookup table:
//   0 → comfort
//   1 → comfort_-1
//   2 → comfort_-2
//   3 → eco
//   4 → frost_protection
//   5 → off
//
// z2m-source: legrand.ts fzLegrand.legrand_pilot_wire_mode.
bool fz_pilot_wire_mode(const DecodedMessage& msg,
                         const FzConverter& self,
                         const PreparedDefinition& def,
                         RuntimeContext& ctx,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzPilotWireMode;

// ── tz_legrand_led_mode ─────────────────────────────────────────────
//
// Manufacturer-specific writes on cluster 0xFC01 for the two LED-
// behaviour booleans. Both accept "ON"/"OFF" (string) or true/false
// (bool) — string-lookup is wired in via the generic ZclWriteSpec.
//
// z2m-source: legrand.ts tzLegrand.led_mode.
extern const TzConverter kTzLedInDark;
extern const TzConverter kTzLedIfOn;

// ── tz_legrand_device_mode ──────────────────────────────────────────
//
// Manufacturer-specific write on cluster 0xFC01 attr 0x0000
// `deviceMode` (DATA16). z2m exposes this as the `device_mode` key
// with values: dimmer_on / dimmer_off / switch / auto / pilot_on /
// pilot_off. Encoded as a u16 LUT.
//
// z2m-source: legrand.ts tzLegrand.legrand_device_mode.
extern const TzConverter kTzDeviceMode;

}  // namespace zhc::legrand
