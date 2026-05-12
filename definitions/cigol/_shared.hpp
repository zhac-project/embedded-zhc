// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Cigol Electronics converters — single device, but with three
// bespoke standard-cluster decoders + one bespoke attribute writer.
// Keeps the device cpp lean and matches the §16 _shared layout.
//
// z2m source: zigbee-herdsman-converters/src/devices/cigol.ts
//
// The Cigol Connect is a Zigbee interface module for LK IHC
// installations. Up to 32 push-button inputs and 16 relay outputs
// landed across a fixed endpoint plan:
//   inputs  : 1..8, 11..18, 31..38, 41..48
//   outputs : 21..28, 51..58
//
// Per-EP exposes (z2m): `state_<ep>`, `input_<ep>`, `switch_action_<ep>`.
// ZHC dispatch rewrites the bare keys with `_<label>` based on
// `src_endpoint`; we declare a `EndpointLabel{"<ep>", <ep>}` for every
// implemented endpoint in the device cpp. The numeric label matches
// z2m's `e.<expose>().withEndpoint("${ep}")` shape.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::cigol {

// ── Fz: cigolInput (genMultistateInput, attr 0x0055 presentValue) ──
//
// presentValue → action label:
//   0 -> "off"     1 -> "single"
//   2 -> "double"  4 -> "hold"     (anything else -> "off")
// Emits key "input" (will be suffixed `_<ep>` by the dispatcher).
extern const FzConverter kFzCigolInput;

// ── Fz: cigolSwitchActionReport (genOnOffSwitchCfg, attr 0x0010) ────
//
// switchActions (UINT8 enum) → label:
//   0 -> "on"  1 -> "off"  2 -> "toggle"  (else -> "off")
// Emits key "switch_action" (suffixed per-EP).
extern const FzConverter kFzCigolSwitchActionReport;

// ── Tz: cigolSwitchAction → write `switchActions` (attr 0x0010) ─────
//
// Accepts "off" / "on" / "toggle" (case-insensitive). Writes 0x0010
// on `genOnOffSwitchCfg` (cluster 0x0007) as ENUM8 (data type 0x30).
// Frame layout: FC=0x10 (profile-wide, disable default response),
// TSN placeholder, cmd 0x02 (writeAttributes), attr-id LE, data
// type, value byte.
extern const TzConverter kTzCigolSwitchAction;

}  // namespace zhc::cigol
