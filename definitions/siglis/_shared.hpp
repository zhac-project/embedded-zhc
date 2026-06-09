// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Siglis (zigfred) vendor converters. Devices pulling from this
// header should carry a `// Tier 2: uses shared siglis converters`
// comment and a `// z2m-source:` provenance line.
//
// z2m-source: zigbee-herdsman-converters/src/devices/siglis.ts.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::siglis {

// ── zigfred button-event decoder (manuSpecificSiglisZigfred 0xFC42) ───
//
// The zigfred plus / uno wall controllers report button presses through
// a manufacturer-specific cluster command rather than the standard
// genScenes / genOnOff command stream. z2m models the cluster as
// `manuSpecificSiglisZigfred` (ID 0xFC42, manufacturerCode SIGLIS_AG)
// with one client→server command `siglisZigfredButtonEvent` (cmd 0x02):
//
//   parameters: button:u8, type:u8, duration:u16   (all little-endian).
//
// z2m's `zifgredFromZigbeeButtonEvent` maps the two lookups
//   button {0:button_1, 1:button_2, 2:button_3, 3:button_4}
//   type   {0:release, 1:single, 2:double, 3:hold}
// into a single `action: "<button>_<type>"` string (e.g.
// `button_1_single`). The `duration` field is ignored, exactly as z2m
// does. Unknown button / type codes publish nothing (no `action`).
//
// No generic converter fits: the command lives on a custom cluster with
// a bespoke payload, so this is the only decoder that can surface the
// device's primary interaction channel. The match is keyed on
// (manufacturer-specific cluster-command, cmd 0x02, client→server);
// `manufacturer_specific` disambiguates it from genOnOff Toggle (also
// cmd 0x02) which is never manufacturer-specific.
//
// z2m-source: siglis.ts `zifgredFromZigbeeButtonEvent`.
extern const FzConverter kFzZigfredButtonEvent;

}  // namespace zhc::siglis
