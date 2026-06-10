// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Profalux vendor converters.
//
// The older NSAV061 cover does NOT expose closuresWindowCovering. Instead
// it reports its lift position via genLevelCtrl `currentLevel` (0..255) and
// its open/close state via genOnOff `onOff`. z2m decodes this with two
// cover-specific converters and writes it back via a third:
//   fz.cover_position_via_brightness — genLevelCtrl currentLevel (0..255)
//       mapped onto position (0..100), plus a derived OPEN/CLOSE `state`.
//   fz.cover_state_via_onoff         — genOnOff onOff -> `state` OPEN/CLOSE.
//   tz.cover_via_brightness          — `position` (0..100) -> genLevelCtrl
//       moveToLevelWithOnOff (level 0..255).
//
// The generic kFzBrightness / kFzOnOff / kTzBrightness converters can't
// reproduce this: kFzBrightness emits the raw `brightness` key (wrong key,
// no 0..100 scale) so the `position` expose stays dead, kFzOnOff emits a
// boolean `state` rather than the OPEN/CLOSE strings, and kTzBrightness
// claims the `brightness` key, not `position`. These vendor converters
// mirror z2m exactly (modelled on the iluminize 5128.10 precedent).
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//   cover_position_via_brightness + cover_state_via_onoff and
//   toZigbee.ts cover_via_brightness (used by profalux.ts #NSAV061).
//
// Tier 2: shared Profalux converters.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::profalux {

// genLevelCtrl (0x0008) attr 0x0000 currentLevel (0..255) -> `position`
// (0..100, round-half-up) plus a derived `state` ("OPEN" when position>0
// else "CLOSE"). Mirrors z2m fz.cover_position_via_brightness (no
// invert_cover option — ZHC has no per-device cover inversion option yet).
extern const ::zhc::FzConverter kFzNsav061PositionViaBrightness;

// genOnOff (0x0006) attr 0x0000 onOff -> `state` "OPEN"/"CLOSE".
// Mirrors z2m fz.cover_state_via_onoff.
extern const ::zhc::FzConverter kFzNsav061StateViaOnOff;

// `position` (0..100) -> genLevelCtrl moveToLevelWithOnOff (cmd 0x04,
// level 0..255). Mirrors z2m tz.cover_via_brightness (numeric path).
extern const ::zhc::TzConverter kTzNsav061PositionViaBrightness;

}  // namespace zhc::devices::profalux
