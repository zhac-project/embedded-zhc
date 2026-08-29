// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared TERNCY (Beevon / AduroSmart-cluster) vendor converters. Every
// device that pulls from this header should carry a `// Tier 2: uses
// shared terncy converters` comment and a `// z2m-source:` provenance
// line.
//
// Background. TERNCY's awareness switch / knob / light socket emit a
// custom "raw" server→client frame on the manuSpecificClusterAduroSmart
// (0xFCCC) cluster. The frame body is undecoded ZCL — z2m's
// `fzLocal.terncy_raw` reads the WHOLE frame buffer (`msg.data`,
// header included): `msg.data[4]` discriminates action vs motion, then
// `msg.data[6]` (action 1..4) or `msg.data[7]` (motion side lookup).
// The AduroSmart header is manufacturer-specific (5 bytes:
// fc/mfg_lo/mfg_hi/tsn/cmd), so the z2m full-frame indices map onto the
// ZHC `DecodedMessage` as: msg.data[4] == `msg.command_id` (the
// discriminator IS the command id), msg.data[6] == `raw_body[1]`,
// msg.data[7] == `raw_body[2]`. ZHC classifies cluster-specific
// commands as `MessageType::Command`, so we filter on
// `Command + ServerToClient + cluster=manuSpecificClusterAduroSmart`
// and read the post-header span via `msg.raw_body` (offsets above).
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
// `manuSpecificClusterAduroSmart`. Layout (z2m comments, remapped to
// the ZHC header-stripped body):
//   command_id == 0 → action frame; raw_body[1] in {1,2,3,4} maps to
//                      "single" / "double" / "triple" / "quadruple".
//   command_id == 4 → motion frame; raw_body[2] in {5,7,40,56} maps
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

// Decode the TERNCY WS01 wall-switch action frame on
// `manuSpecificClusterAduroSmart` (0xFCCC, mfg 0x1228 XIAOYAN).
//
// A DIFFERENT frame shape from `kFzTerncyRawAction` above — same
// cluster, but the WS01 switches use a wider click table (1..7 instead
// of 1..4) and add a hold/release command with a duration payload:
//
//   z2m full-frame          ZHC (5-byte manuf-specific header stripped)
//   ----------------------  ----------------------------------------
//   data[0..2] 0d 28 12      frame-control + mfg code -> matched by the
//                            descriptor (manu-specific, S->C)
//   data[4]  discriminator   msg.command_id
//   data[5]                  raw_body[0]
//   data[6]  click index     raw_body[1]
//   data[7]  duration lo     raw_body[2]
//   data[8]  duration hi     raw_body[3]
//
//   command_id == 0x00 -> click. raw_body[1] in 1..7 maps to
//                          single/double/triple/quadruple/5_click/
//                          6_click/7_click.
//   command_id == 0x29 -> hold (raw_body[0]==0x02) or release
//                          (raw_body[0]==0x08), plus
//                          `action_duration` = raw_body[2] |
//                          raw_body[3] << 8.
//
// The endpoint rides in the action VALUE, not the key (`single_l1`) —
// matching z2m and the `kAlwaysGlobalKeys` policy in dispatch.cpp. The
// label is resolved from the def's `endpoint_map` via `src_endpoint`;
// a frame from an endpoint outside the map is dropped.
//
// z2m-source: zigbee-herdsman-converters/src/devices/terncy.ts
//             fzLocal.ws01_action.
extern const FzConverter kFzTerncyWs01Action;

}  // namespace zhc::terncy
