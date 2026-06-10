// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Zunzunbee vendor converters.
//
// The SSWZ8T "Slate switch" is an 8-button battery scene controller. It
// does NOT use genScenes / genOnOff command frames like a typical scene
// remote — instead it packs the pressed button + press type into the
// `ssIasZone` (0x0500) zoneStatus word and reports it via
// commandStatusChangeNotification / attributeReport / readResponse:
//
//   bit 0          → press type   (0 = short_press, 1 = long_press)
//   bits 1..8      → button number, one-hot: masked value & 0x01FE maps
//                    2→1, 4→2, 8→3, 16→4, 32→5, 64→6, 128→7, 256→8
//
// z2m surfaces this as a single composite `action` of the form
//   button_<n>_<short|long>_press
// (see fzLocal.fzZunzunbeeSlateSwitchIAS). The button number is baked
// INTO the action string, so there is no per-endpoint demux: this is one
// IAS endpoint emitting one composite `action` key (no endpoint_map /
// endpoint_action_suffix).
//
// The generic `kFzIasZoneStatusChange` would mislabel this frame as a
// bare alarm/alarm_2/tamper bitfield, so we own the decode here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/zunzunbee.ts.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::zunzunbee {

// SSWZ8T 8-button scene switch. Decodes the ssIasZone zoneStatus word
// into `action = button_<n>_<short|long>_press`. Fires on the same frame
// set z2m listens to (commandStatusChangeNotification + attributeReport +
// readResponse), Direction::ServerToClient (device is the cluster server).
extern const FzConverter kFzZunzunbeeSlateSwitch;

}  // namespace zhc::zunzunbee
