// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Develco / Frient vendor converters. Every device that pulls
// from this header should carry a `// Tier 2: uses shared develco
// converters` comment and a `// z2m-source:` provenance line.
//
// Develco (manufacturer code 0x1015) extends three standard clusters
// with manu-specific attributes:
//   genBasic    (0x0000) attr 0x8001  develcoLedControl     (bitmap8)
//   ssIasZone   (0x0500) attr 0x8001  develcoAlarmOffDelay  (uint16)
//   manuSpec    (0xFC03) attr 0x0000  measuredValue (VOC ppb, uint16)
//
// z2m-source: zigbee-herdsman-converters/src/lib/develco.ts +
//             devices/develco.ts (top `develco = { fz, tz }` block).

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::develco {

// Manufacturer code for Develco Products A/S — used by every manu-
// specific read/write on this vendor.
constexpr std::uint16_t kManuCodeDevelco = 0x1015;

// ── VOC + air-quality decoder (cluster 0xFC03) ────────────────────
//
// Decodes a single u16 attribute report on `manuSpecificDevelcoAirQuality`
// (cluster 0xFC03, attr 0x0000) and emits two payload keys:
//
//   "voc"          — Float, µg/m³ (raw ppb × 4.5 — Sensirion SGP3x note)
//   "air_quality"  — String enum from the AQSZB-110 manual table
//                    {excellent, good, moderate, poor, unhealthy,
//                     out_of_range}
//
// Thresholds (ppb):
//   <= 65    excellent
//   <= 220   good
//   <= 660   moderate
//   <= 2200  poor
//   <= 5500  unhealthy
//   >  5500  out_of_range
//
// z2m-source: lib/develco.ts `develcoModernExtend.voc` +
//             `develcoModernExtend.airQuality`.
bool fz_develco_voc(const DecodedMessage& msg,
                     const FzConverter& self,
                     const PreparedDefinition& def,
                     RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzDevelcoVoc;

// ── LED control decoder (cluster genBasic 0x8001) ─────────────────
//
// Decodes `develcoLedControl` (manu-specific bitmap8 on genBasic
// attr 0x8001) into a string enum:
//   0   → "off"
//   1   → "fault_only"
//   2   → "motion_only"
//   255 → "both"
//
// Used by MOSZB-140 / MOSZB-153 motion sensors. Output key is
// `led_control`.
//
// z2m-source: devices/develco.ts `develco.fz.led_control`.
bool fz_develco_led_control(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzDevelcoLedControl;

// ── IAS occupancy timeout decoder (cluster ssIasZone 0x8001) ──────
//
// Decodes `develcoAlarmOffDelay` (manu-specific uint16 on ssIasZone
// attr 0x8001) into a numeric `occupancy_timeout` (seconds). z2m
// validates a 5-second floor on the write side (see
// `kTzDevelcoOccupancyTimeout`); reads are pass-through.
//
// z2m-source: devices/develco.ts `develco.fz.ias_occupancy_timeout`.
bool fz_develco_occupancy_timeout(const DecodedMessage& msg,
                                    const FzConverter& self,
                                    const PreparedDefinition& def,
                                    RuntimeContext& ctx,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzDevelcoOccupancyTimeout;

// ── Canonical TZ writes ──────────────────────────────────────────
//
// Both writes target Develco manu-specific attributes (mfg 0x1015):
//
//   kTzDevelcoLedControl       genBasic   attr 0x8001 enum8
//                              lookup → {off=0, fault_only=1,
//                                         motion_only=2, both=255}
//   kTzDevelcoOccupancyTimeout ssIasZone  attr 0x8001 uint16 (seconds)
//
// Both rely on `::zhc::generic::tz_zcl_write_attr` for ZCL framing
// (fc=0x14 + manu_code + writeAttributes). No 5-second clamp on the
// write side here — adapter / SPA can enforce. Behavioural match.
//
// z2m-source: devices/develco.ts `develco.tz.led_control` /
//             `develco.tz.ias_occupancy_timeout`.
extern const TzConverter kTzDevelcoLedControl;
extern const TzConverter kTzDevelcoOccupancyTimeout;

}  // namespace zhc::develco
