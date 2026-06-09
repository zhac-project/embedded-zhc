// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Orvibo vendor converters. Used by every device that pulls in
// the Orvibo `manuSpecificOrvibo2` (0xFF00) cluster for power_on_behavior.
//
// z2m-source: zigbee-herdsman-converters/src/devices/orvibo.ts
//   helpers `clusterManuSpecificOrviboPowerOnBehavior()` +
//           `orviboSwitchPowerOnBehavior()`.
//
// 12 of 38 Orvibo devices share the same wire shape — manu-specific
// write of attribute 0x0001 (uint8) on cluster 0xFF00 with values
//   1 → "off", 2 → "previous"
// (no `on` choice — that's deliberate per upstream).
//
// Tier 2: shared Orvibo converters.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::orvibo {

// TzConverter for outbound `power_on_behavior` writes — encodes a
// non-manu-specific writeAttributes frame on cluster 0xFF00 / attr
// 0x0001 with the lookup `off→1`, `previous→2`.
//
// Inbound decode of `manuSpecificOrvibo2` attribute reports is not
// shipped yet (no generic enum8 attr-report decoder exists for
// vendor-private clusters). The shadow still stores the most-recent
// successful tz write, so the value round-trips on the SPA without
// the inbound path.
extern const ::zhc::TzConverter kTzOrviboPowerOnBehavior;

// Static expose row reusable by every Orvibo switch port.
//   "power_on_behavior" Enum {off, previous} StateSet, Config category.
extern const ::zhc::Expose kExposePowerOnBehaviorOrvibo;

// ── Scene-remote raw-frame action decoders ──────────────────────────
//
// The CR11S8UZ "sticker switch" and T40S6Z "MixSwitch 6 gangs" are
// stateless scene remotes: each button press emits a vendor-private
// raw frame on the Orvibo cluster (id 0x0017, named `manuSpecificOrvibo`
// upstream) carrying NO standard ZCL attribute. z2m decodes them with
// `fz.orvibo_raw_1` / `fz.orvibo_raw_2`, publishing an `action` string
// such as `"button_1_click"`. They are *not* genOnOff relays — wiring
// the generic kFzOnOff converter behind a `state` expose (as the
// generated stub did) emits nothing on a press, so the action was lost.
//
// ZHC delivers the post-header command body via `msg.raw_body`. z2m's
// documented wire (`orvibo_raw_1`: data = [0x19, tsn, cmd, BUTTON, 0,
// ACTION]) lands button at raw_body[0] and action at raw_body[2] once
// the 3-byte ZCL header (fc/tsn/cmd, non-manufacturer) is stripped.
//
// Button byte → button index:
//   raw_1: {3→1, 11→2, 7→3, 15→4}   raw_2: {1→1 … 7→7}  (identity)
// Action byte → verb: {0→click, 2→hold, 3→release}.
// Emitted key: `action` = "button_<n>_<verb>" (String expose, State).
//
// Matching mirrors the terncy raw precedent: filter on Command type
// (ZHC classifies cluster-specific commands as MessageType::Command).
// The device emits these with the ZCL direction bit set (server→client,
// FC 0x19), so the converter pins Direction::ServerToClient — the
// matcher enforces direction exactly. The decode self-guards — an
// unrecognised button/action byte returns no match — so the wildcard
// cluster (msg.cluster is id-routed, never enforced here) is safe.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             orvibo_raw_1 (cluster 23) + orvibo_raw_2.
// The matching `action` String expose is inlined per-def (ZHC has no
// enum/list action type, and exposes must be constexpr-initialised).
extern const ::zhc::FzConverter kFzOrviboRaw1Action;   // CR11S8UZ
extern const ::zhc::FzConverter kFzOrviboRaw2Action;   // T40S6Z

}  // namespace zhc::orvibo
