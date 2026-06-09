// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Easyiot vendor-specific shared converters.
//
// Currently a single converter: kFzEasyiotAction — the embedded-zhc
// equivalent of z2m's `easyiot_action` fz (easyiot.ts), used by the
// ZB-WB01/02/03/08 multi-button scene remotes.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {

// genOnOff cluster-command → "<button>_<single|double|long>" action.
//
// Mirrors z2m `easyiot_action`: the WB0x remotes emit genOnOff
// On/Off/Toggle *commands* (device is the client), and z2m remaps them
//   commandToggle (0x02) -> "single"
//   commandOn     (0x01) -> "double"
//   commandOff    (0x00) -> "long"
// then prefixes the originating button derived from `msg.src_endpoint`
// (ep1 -> "1_", ep2 -> "2_", ...). The generic kFzCommandOn/Off/Toggle
// converters emit the *standard* literals ("on"/"off"/"toggle") with no
// button prefix, so they produce neither the right verb nor the button
// identity for these devices.
extern const ::zhc::FzConverter kFzEasyiotAction;

}  // namespace zhc::devices::easyiot
