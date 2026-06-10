// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Tier 2: EasyAccess (Datek) door-lock converters.
//
// The EasyFinger V2 (EasyCode903G2.1) is a battery door lock on the
// standard closuresDoorLock (0x0101) cluster. Beyond the generic
// lock-state attribute report it carries:
//   * a bespoke RAW closuresDoorLock command stream (the keypad / RFID /
//     zigbee unlock event), decoded by `easycode_action`.
//   * a soundVolume enum (attr 0x0024) with EasyAccess' own label set.
//   * an `auto_relock` binary written to autoRelockTime (0x0023).
//
// z2m-source: zigbee-herdsman-converters/src/devices/easyaccess.ts +
//             src/converters/fromZigbee.ts `easycode_action`, `lock` +
//             src/converters/toZigbee.ts `easycode_auto_relock`,
//             `lock_sound_volume`.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::easyaccess {

// RAW closuresDoorLock command → `action`. Mirrors z2m's
// `easycode_action`: index [4] holds lock(13)/zigbee_unlock(14); any
// other code falls through to index [3] for rfid_unlock(3) /
// keypad_unlock(0). Emits action = "lock" | "zigbee_unlock" |
// "rfid_unlock" | "keypad_unlock".
extern const ::zhc::FzConverter kFzEasycodeAction;

// closuresDoorLock soundVolume (attr 0x0024 ENUM8) attribute report →
// `sound_volume` (StringRef). Uses z2m's `constants.lockSoundVolume`
// labels: 0 = silent_mode, 1 = low_volume, 2 = high_volume. The generic
// kFzLock decodes only lock_state, so this vendor converter fills the
// soundVolume half that z2m's `fz.lock` covers.
extern const ::zhc::FzConverter kFzEasycodeSoundVolume;

// `sound_volume` writer (closuresDoorLock soundVolume 0x0024 ENUM8).
// z2m `tz.lock_sound_volume`. Vendor copy because EasyAccess' labels
// (silent_mode/low_volume/high_volume) differ from the generic
// kTzLockSoundVolume's (silent/low/high).
extern const ::zhc::TzConverter kTzEasycodeSoundVolume;

// `auto_relock` binary writer → autoRelockTime (0x0023) value ? 1 : 0.
// z2m `tz.easycode_auto_relock`.
extern const ::zhc::TzConverter kTzEasycodeAutoRelock;

}  // namespace zhc::devices::easyaccess
