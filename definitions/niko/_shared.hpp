// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Niko-specific shared converters.
//
// Two manufacturer-specific clusters are used by Niko wall switches /
// dimmers / outlets:
//
//   manuSpecificNikoConfig  cluster id 0xFC00, manu code NIKO_NV (0x125F)
//     0x0000 enum8     switchOperationMode  (control_relay=2, decoupled=1)
//     0x0100 uint24    outletLedColor       (RGB-ish lookup)
//     0x0101 uint8     outletChildLock      (0=lock, 1=unlock)
//     0x0104 uint8     ledEnable            (bool)
//     0x0105 bitmap8   ledState             (per-endpoint bit; 0/1)
//     0x0107 bitmap32  ledSyncMode          (4-bit-per-ep field)
//
//   manuSpecificNikoState   cluster id 0xFC01, manu code NIKO_NV
//     0x0001 bitmap8   switchActionReporting (false=0x01, true=0x1F)
//     0x0002 bitmap32  switchAction          (button events; report-only)
//
// z2m-source: niko.ts `local.modernExtend.addCustomClusterManuSpecificNiko*`
// + `local.tz.outlet_*`, `local.tz.switch_*`, `local.fz.*`.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::niko {

using ::zhc::FzConverter;
using ::zhc::TzConverter;

// ── Tz writes (cluster 0xFC00 — NikoConfig) ────────────────────────
// Shared by 170-33505 outlet, 552-721X1/X2 switches, 552-72201 dimmer.
extern const TzConverter kTzNikoOperationMode;     // operation_mode
extern const TzConverter kTzNikoChildLock;         // child_lock
extern const TzConverter kTzNikoLedEnable;         // led_enable
extern const TzConverter kTzNikoLedColor;          // led_color
extern const TzConverter kTzNikoLedState;          // led_state
// led_sync_mode (BITMAP32 0x0107): BLOCKED — generic tz_zcl_write_attr
// only supports types up to 4 bytes (u32 0x23, bitmap8 0x18, bitmap16
// 0x19). Adding bitmap32 (0x1B) to `zcl_type_len` is the upstream fix.

// ── Tz writes (cluster 0xFC01 — NikoState) ─────────────────────────
extern const TzConverter kTzNikoActionReporting;   // action_reporting

// ── Fz reads ───────────────────────────────────────────────────────
//
// kFzNikoConfig  emits `operation_mode`, `child_lock`, `led_enable`,
//                `led_color`, `led_state`, `led_sync_mode` from
//                attribute reports on cluster 0xFC00. Multi-endpoint
//                handling for `led_state` / `led_sync_mode` is *not*
//                done here — runtime keys are bitfield-as-uint and the
//                S3 shadow keeps per-key state. Good enough for parity.
extern const FzConverter kFzNikoConfig;

// kFzNikoState   emits `action` from cluster 0xFC01 attr 0x0002. The
//                shifted-button decoding (left/right/_ext, single/hold/
//                release) follows niko.ts `local.fz.switch_action`.
//                The model name is matched at runtime (not done here) —
//                two-button vs one-button mapping done via user_config.
extern const FzConverter kFzNikoSwitchAction1Btn;
extern const FzConverter kFzNikoSwitchAction2Btn;

}  // namespace zhc::devices::niko
