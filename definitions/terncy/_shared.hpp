// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared TERNCY (Beevon / AduroSmart-cluster) vendor converters. Every
// device that pulls from this header should carry a `// Tier 2: uses
// shared terncy converters` comment and a `// z2m-source:` provenance
// line.
//
// Background. TERNCY's awareness switch / knob / light socket emit a
// custom "raw" client→server frame on the manuSpecificClusterAduroSmart
// (0xFCCC) cluster. The frame body is undecoded ZCL — z2m's
// `fzLocal.terncy_raw` reads `msg.data[4]` to discriminate action vs
// motion frames, then `msg.data[6]` (action 1..4) or `msg.data[7]`
// (motion side lookup). Because ZHC classifies cluster-specific
// commands as `MessageType::Command`, we filter on
// `Command + ClientToServer + cluster=manuSpecificClusterAduroSmart`
// and read the post-header span via `msg.raw_body`.
//
// The knob (TERNCY-SD01) additionally emits attribute reports on
// attribute 0x001b (`terncyRotation`, INT16) — z2m's
// `fzLocal.terncy_knob`. Sign indicates direction; magnitude / 12 is
// the action_number.
//
// The contact sensor (TERNCY-DC01) uses `m.binary("contact",
// genBinaryInput, presentValue)` — emitted via `kFzTerncyContact`.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::terncy {

// Body-shape selector for the terncy_raw decoder. PP01 wants both the
// action (msg.data[4]==0) and the motion path (msg.data[4]==4) — the
// latter emits `occupancy=true`. SD01 / LS01 only want the action path.
struct TerncyRawConfig {
    bool emit_action;
    bool emit_motion_occupancy;
};

extern const TerncyRawConfig kTerncyRawActionOnly;
extern const TerncyRawConfig kTerncyRawActionAndMotion;

// Decode a raw ClientToServer frame on
// `manuSpecificClusterAduroSmart`. Body layout (z2m comments):
//   raw_body[4] == 0 → action frame; raw_body[6] in {1,2,3,4} maps to
//                      "single" / "double" / "triple" / "quadruple".
//   raw_body[4] == 4 → motion frame; raw_body[7] in {5,7,40,56} maps
//                      to action_side "right" / "left" and emits
//                      occupancy=true (only when emit_motion_occupancy
//                      is set on user_config).
//
// Two ready-wired descriptors are provided so devices don't have to
// build their own; pick the one matching the device's expose set:
//   - `kFzTerncyRawAction`        — action only (SD01, LS01)
//   - `kFzTerncyRawActionMotion`  — action + motion+occupancy (PP01)
//
// z2m-source: zigbee-herdsman-converters/src/devices/terncy.ts
//             fzLocal.terncy_raw.
extern const FzConverter kFzTerncyRawAction;
extern const FzConverter kFzTerncyRawActionMotion;

// Decode a `terncyRotation` attribute report (attr 0x001b INT16) on
// `manuSpecificClusterAduroSmart`. Emits:
//   action          = "rotate"
//   action_direction = "clockwise" / "counterclockwise"
//   action_number    = abs(rotation) / 12  (Float)
//
// z2m-source: zigbee-herdsman-converters/src/devices/terncy.ts
//             fzLocal.terncy_knob.
extern const FzConverter kFzTerncyKnob;

// Decode `genBinaryInput.presentValue` (attr 0x0055) into
//   contact = bool   (true = closed, false = open).
//
// z2m-source: zigbee-herdsman-converters/src/devices/terncy.ts
//             terncyExtend.contact (m.binary).
extern const FzConverter kFzTerncyContact;

// Decode `msTemperatureMeasurement` 0x0000 with `scale: 10` — the
// Terncy DC01 / PP01 report measuredValue in 0.1 °C units instead of
// the standard 0.01 °C. Generic `kFzTemperature` would divide by 100
// and lose a decade of precision. Emits `temperature` (Float °C).
//
// z2m-source: zigbee-herdsman-converters/src/lib/modernExtend.ts
//             m.temperature({scale: 10}).
extern const FzConverter kFzTerncyTempScale10;

}  // namespace zhc::terncy
