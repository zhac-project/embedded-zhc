// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared BTicino (Legrand-family) vendor converters.
//
// The K4003C/L4003C/N4003C/NT4003C light switch reports its physical
// button state on genBinaryInput (0x000F) presentValue (attr 0x0055),
// NOT only on genOnOff. z2m's `fz.K4003C_binary_input` decodes that
// presentValue into BOTH an `action` ("on"/"off") and the relay
// `state` ("ON"/"OFF"). The generated port wired only kFzOnOff (genOnOff
// 0x0006 → state) and dropped the action entirely, so the
// e.action(["identify","on","off"]) surface was dead. `kFzK4003CBinaryInput`
// restores it.
//
// z2m-source: bticino.ts #K4003C/... fromZigbee fz.K4003C_binary_input;
//   converters/fromZigbee.ts K4003C_binary_input:
//     {action: isOn ? "on":"off", state: isOn ? "ON":"OFF"}

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::bticino {

// genBinaryInput (0x000F) presentValue → action on/off + state.
extern const FzConverter kFzK4003CBinaryInput;

}  // namespace zhc::devices::bticino
