// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Livolo custom cluster converters — see _shared.cpp.
//
// Livolo TI0001-* devices encode state via raw genPowerCfg (0x0001)
// frames whose body starts with the magic 2-byte header [0x7A 0xD1].
// Actual data follows at fixed offsets (data[10] = DP id, data[14] =
// status byte). Outbound state writes are 16-byte manuSpec
// `writeAttributes` frames (mfg = 0x1AD2, fixed TSN = 0xE9) where the
// "type" byte selects which gang is addressed. Neither shape fits
// ZHC's generic encoder; the bespoke runtime lives here.

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::livolo {

// Existing sensor decoders.
extern const ::zhc::FzConverter kFzLivoloHygrometer;
extern const ::zhc::FzConverter kFzLivoloIlluminance;

// 2026-04-28r post-sweep — switch / socket / pir state decoders.
//   kFzLivoloNewSwitchState1Gang — bit 0 → "state" (single-gang).
//   kFzLivoloNewSwitchState2Gang — bits 0..1 → state_left/state_right
//                                  (data[10] must be 7).
//   kFzLivoloNewSwitchState4Gang — bits 0..3 → state_left / state_right
//                                  / state_bottom_left / state_bottom_right.
//   kFzLivoloPirState            — bit 0 → "occupancy" (bool, data[10]==7).
extern const ::zhc::FzConverter kFzLivoloNewSwitchState1Gang;
extern const ::zhc::FzConverter kFzLivoloNewSwitchState2Gang;
extern const ::zhc::FzConverter kFzLivoloNewSwitchState4Gang;
extern const ::zhc::FzConverter kFzLivoloPirState;

// 2026-06-10 parity pass — curtain-switch / dimmer / cover decoders that
// were previously wired to dead generic on/off/cover converters.
//   kFzLivoloCurtainState — gate data[10] in {5,2}; state_left=(s==1),
//                           state_right=(s==0) (non-bitmask polarity).
//   kFzLivoloDimmerState  — data[10]==7/13 -> "state"; ==5 -> "brightness".
//   kFzLivoloCoverState   — dp 0x0c/0x0f, reportType 0x04 -> position/
//                           state/moving; 0x12 -> motor_speed;
//                           0x13 -> motor_direction.
extern const ::zhc::FzConverter kFzLivoloCurtainState;
extern const ::zhc::FzConverter kFzLivoloDimmerState;
extern const ::zhc::FzConverter kFzLivoloCoverState;

// 2026-04-28r post-sweep — state encoder for switch / socket / cover /
// dimmer ports. Accepts keys `state`, `state_left`, `state_right`,
// `state_bottom_left`, `state_bottom_right`. Input may be a Bool, a
// StringRef "ON"/"OFF", or a non-zero Uint. Emits a 16-byte manuSpec
// writeAttribute frame to genPowerCfg attr 0x0001 with the
// gang-specific type byte + payload. The `genLevelCtrl` channel-
// selector prelude that z2m sends is omitted; for single-gang
// devices and the bottom-* gangs of the 4-gang switch this matches
// the device's expectation. `state_left` / `state_right` on the
// 4-gang switch may need follow-up hardware testing.
extern const ::zhc::TzConverter kTzLivoloState;

}  // namespace zhc::devices::livolo
