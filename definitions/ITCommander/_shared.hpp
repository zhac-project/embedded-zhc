// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared IT Commander vendor converters.
//
// The ITCMDR_Click button reports presses on genMultistateInput
// (cluster 0x0012) via the PresentValue attribute (0x0055). z2m's
// `fz.itcmdr_clicks` maps the raw presentValue to an action string:
//   0 → "hold", 1 → "single", 2 → "double", 3 → "triple",
//   4 → "quadruple", 255 → "release", anything else → "many".
// No generic converter covers this multistate-input → action mapping
// (the generic kFzCommand* family decodes genOnOff / genLevelCtrl
// cluster commands, not multistate PresentValue), so it lives here.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             (itcmdr_clicks).

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::ITCommander {

// genMultistateInput (0x0012) PresentValue → `action` string converter
// for the ITCMDR_Click button.
extern const FzConverter kFzItcmdrClicks;

}  // namespace zhc::ITCommander
