// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// MultIR-specific shared converters.
//
//   kFzMultirSosAction — MIR-SO100 SOS button. z2m fzLocal.MIRSO100 reads a
//     raw ssIasZone (0x0500) command frame and switches on msg.data[3] (the
//     first payload byte after the 3-byte ZCL header): 0 → single, 1 →
//     double, 128 → hold. Emits `action`. The Tier-1 auto-port mis-modelled
//     this button as a settable genOnOff switch (phantom `state` + kTzOnOff)
//     with a 0x0006 binding the device does not speak.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::multir {

extern const ::zhc::FzConverter kFzMultirSosAction;

}  // namespace zhc::devices::multir
