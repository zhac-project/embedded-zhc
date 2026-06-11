// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Echostar vendor-specific shared converters.
//
// Currently a single converter: kFzSageBellAction — the embedded-zhc
// equivalent of z2m's `SAGE206612_state` fz (echostar.ts), used by the
// SAGE206612 "SAGE by Hughes" doorbell sensor.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::echostar {

// genOnOff cluster-command → "bell1"/"bell2" action.
//
// Mirrors z2m `SAGE206612_state`: the doorbell emits genOnOff On/Off
// *commands* (device is the client) and z2m remaps them
//   commandOn  (0x01) -> "bell1"
//   commandOff (0x00) -> "bell2"
// (z2m wraps this in a per-endpoint debounce window; the load-bearing
// behaviour for the shadow is the command→action key map, which is what
// we reproduce here). The generic kFzCommandOn/Off converters emit the
// *standard* "on"/"off" literals, which are wrong for this device.
extern const ::zhc::FzConverter kFzSageBellAction;

}  // namespace zhc::devices::echostar
