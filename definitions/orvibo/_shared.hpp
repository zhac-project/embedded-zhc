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

}  // namespace zhc::orvibo
