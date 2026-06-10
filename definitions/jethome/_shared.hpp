// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// JetHome shared converters.
//
//   * `kFzJetHomeMultiStateAction` — decodes `genMultistateInput`
//     (cluster 0x0012) presentValue (attr 0x0055) into the discrete-input
//     `action` string used by the WS7 3-channel module. Mirrors z2m's
//     `jetHome.fz.multiStateAction` actionLookup: each channel reports the
//     same five gestures, with the high byte (0 / 256 / 512 / 1024)
//     selecting the source slot — but the source endpoint already carries
//     that distinction, so the low byte maps to the gesture and the
//     dispatch layer suffixes `action` → `action_in<n>` per endpoint_map.
//
// z2m-source: zigbee-herdsman-converters/src/devices/jethome.ts.
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::jethome {

using ::zhc::FzConverter;

// `genMultistateInput` 0x0055 presentValue → "action" string
// (release / single / double / triple / hold).
extern const FzConverter kFzJetHomeMultiStateAction;

}  // namespace zhc::devices::jethome
