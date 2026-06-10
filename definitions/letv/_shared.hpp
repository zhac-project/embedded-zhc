// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// LeTV shared converters.
//
//   * `kFzLetv8KeyAction` — decodes the LeTV 8-key (qlwz.letv8key.10)
//     scene remote. z2m's `fz.qlwz_letv8key_switch` reads
//     genMultistateInput (cluster 0x0012) presentValue (attr 0x0055) and
//     combines it with the *source endpoint* to form an `action` string:
//       button = {4:up, 2:down, 5:left, 3:right, 8:center, 1:back,
//                 7:play, 6:voice}[endpoint.ID]
//       gesture = {0:hold, 1:single, 2:double, 3:tripple}[presentValue]
//       action = `<gesture>_<button>`   (e.g. "single_up")
//     The button is carried *inside* the action string (not via an
//     endpoint suffix), so this def has NO endpoint_map — the converter
//     resolves src_endpoint itself and emits a bare `action`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/letv.ts (#LeTV.8KEY)
//             + converters/fromZigbee.ts (qlwz_letv8key_switch).
#pragma once
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::letv {

using ::zhc::FzConverter;

// genMultistateInput 0x0055 presentValue + src_endpoint → "action"
// ("<gesture>_<button>", e.g. "single_up" / "hold_center").
extern const FzConverter kFzLetv8KeyAction;

}  // namespace zhc::devices::letv
