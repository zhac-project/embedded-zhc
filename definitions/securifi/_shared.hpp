// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Securifi (Almond / Peanut) vendor converters.
//
// The Almond Click button reports clicks through the ssIasAce
// `commandArm` command, abusing the `armmode` field as a click-type
// code. z2m `fz.almond_click` maps that field through a device-specific
// lookup — NOT the standard IAS-ACE arm-mode names the generic
// `kFzIasAceArm` emits:
//
//   armmode 3 → "single"
//   armmode 0 → "double"
//   armmode 2 → "long"
//
// (Generic kFzIasAceArm would mislabel these as "arm_all_zones",
// "disarm", "arm_night_zones".) Hence a dedicated vendor decoder.
//
// z2m-source: zigbee-herdsman-converters/src/devices/securifi.ts.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::securifi {

// ssIasAce (0x0501) commandArm decoder for the Almond Click button.
// Mirrors z2m `fz.almond_click`: reads the first body byte (armmode)
// and emits `action` ∈ {single, double, long}. The 100 ms duplicate-
// press debounce z2m applies is stateful runtime behaviour, not a wire
// decode, and is intentionally not reproduced here.
extern const FzConverter kFzAlmondClick;

}  // namespace zhc::securifi
