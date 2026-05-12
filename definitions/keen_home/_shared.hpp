// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Keen Home shared converters — cover-via-brightness pair shared by the
// SV01 and SV02 smart vents. z2m wires these as
// `fz.cover_position_via_brightness` + `tz.cover_via_brightness`: a smart
// vent reports its open percentage on `genLevelCtrl.currentLevel` (0..255)
// rather than `closuresWindowCovering`. We translate to the standard
// `position` (0..100) shadow key on both legs so the rest of the stack can
// treat the device like any other cover.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::keen_home {

// genLevelCtrl currentLevel attr 0x0000 (u8 0..255) → emits "position"
// (Uint 0..100) and "state" (StringRef "OPEN" / "CLOSE").
extern const ::zhc::FzConverter kFzKeenCoverPosition;

// Accepts "position" (Uint 0..100) → genLevelCtrl moveToLevelWithOnOff
// (cmd 0x04, level u8 0..255 + transtime u16 LE = 0).
// Accepts "state" string ("OPEN" / "CLOSE") via the same encoder by
// mapping to position 100 / 0 respectively. "STOP" emits genLevelCtrl
// stop (cmd 0x03).
extern const ::zhc::TzConverter kTzKeenCoverPosition;

}  // namespace zhc::devices::keen_home
