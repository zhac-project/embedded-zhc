// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Datek (Eva / APEX / ID Lock) vendor converters. Devices that
// pull from this header carry a `// Tier 2: uses shared datek
// converters` comment plus a `// z2m-source:` provenance line.
//
// Manufacturer code: 0x1337 (DATEK_WIRELESS_AS).
//
// Two cluster surfaces use mfg-specific writes:
//   ssIasZone (0x0500)         attr 0x4000 — `led_on_motion` BOOL
//   closuresDoorLock (0x0101)  attrs 0x4000..0x4005 — ID-lock manuSpec
//
// All TZ writes route through the generic `tz_zcl_write_attr` encoder
// which stamps `fc=0x14` + the mfg-code into the writeAttributes(0x02)
// frame.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::datek {

// Manufacturer code for Datek-group writes/reads.
constexpr std::uint16_t kMfgDatek = 0x1337;

// ── fz_datek_idlock ────────────────────────────────────────────────
//
// Decode `closuresDoorLock` mfg-specific attribute reports for the
// Datek ID Lock 150 / 202. Emits:
//   0x4000 BOOL  → "master_pin_mode"
//   0x4001 BOOL  → "rfid_enable"
//   0x4003 UINT8 → "service_mode" (lookup: 0=deactivated, 1/5=random_pin_1x_use, 6/9=random_pin_24_hours)
//   0x4004 UINT8 → "lock_mode"   (lookup: 0=auto_off_away_off, 1=auto_on_away_off, 2=auto_off_away_on, 3=auto_on_away_on)
//   0x4005 BOOL  → "relock_enabled"
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             idlock.
bool fz_idlock(const DecodedMessage& msg,
               const FzConverter& self,
               const PreparedDefinition& def,
               RuntimeContext& ctx,
               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzIdlock;

// ── fz_datek_led_on_motion ─────────────────────────────────────────
//
// Decode `ssIasZone` mfg-specific attribute 0x4000 (BOOL) into
// "led_on_motion" for the HSE2927E motion sensor.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             led_on_motion.
bool fz_led_on_motion(const DecodedMessage& msg,
                      const FzConverter& self,
                      const PreparedDefinition& def,
                      RuntimeContext& ctx,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLedOnMotion;

// ── tz: ID Lock manuSpec writes ────────────────────────────────────
//
// All on closuresDoorLock (0x0101) with mfg=0x1337.
extern const TzConverter kTzMasterPinMode;     // attr 0x4000 BOOL
extern const TzConverter kTzRfidEnable;        // attr 0x4001 BOOL
extern const TzConverter kTzServiceMode;       // attr 0x4003 UINT8 (lookup)
extern const TzConverter kTzLockMode;          // attr 0x4004 UINT8 (lookup)
extern const TzConverter kTzRelockEnabled;     // attr 0x4005 BOOL

// ── tz: standard (non-mfg) write for ID-lock sound_volume ─────────
//
// closuresDoorLock (0x0101) attr 0x0024 `soundVolume` UINT8.
// z2m maps strings via `constants.lockSoundVolume` (silent/low/high).
// We mirror that lookup here so the converter accepts both string
// labels and raw uint inputs.
extern const TzConverter kTzLockSoundVolume;

// ── tz: motion-sensor manuSpec writes ──────────────────────────────
//
// On ssIasZone (0x0500), attr 0x4000 BOOL, mfg=0x1337.
extern const TzConverter kTzLedOnMotion;

// ── tz: standard (non-mfg) write for occupancy_timeout ─────────────
//
// msOccupancySensing (0x0406) attr 0x0010 `pirOToUDelay` UINT16,
// mfg-code=0 (standard write). Lives next to the motion-sensor
// converters because that's the only Datek device that exposes it.
extern const TzConverter kTzOccupancyTimeout;

}  // namespace zhc::datek
