// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared L&S Lighting (ls) converters — a single SEMOTE remote needs
// two bespoke genOnOff command decoders that the generic shared
// catalogue can't supply: emit `on_double`/`off_double` action
// strings (the catalogue's kFzCommandOff/kFzCommandOffWithEffect
// both emit literal "off", and there's no decoder for genOnOff
// commandOnWithRecallGlobalScene cmd 0x07).
//
// z2m source: zigbee-herdsman-converters/src/devices/ls.ts
//
// Per ls.ts, the SEMOTE button helper layers two custom fz on top of
// `m.commandsOnOff({commands: ["on", "off"]})`:
//   commandOnWithRecallGlobalScene (cmd 0x07) → action="on_double"
//   commandOffWithEffect           (cmd 0x40) → action="off_double"
// `addActionGroup` enriches the payload with `action_group` (the
// Zigbee group id the command was bound to). ZHC's adapter does not
// surface the bind group on inbound frames yet, so the matching
// `action_group` numeric expose is published but stays absent at
// runtime — see LS_PARITY.md for the open-gap details.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::ls {

// genOnOff cmd 0x07 commandOnWithRecallGlobalScene → action="on_double".
extern const FzConverter kFzLsCommandOnDouble;

// genOnOff cmd 0x40 commandOffWithEffect → action="off_double".
// The 2-byte payload (effect_id, variant) is ignored, matching the
// shared catalogue's kFzCommandOffWithEffect — the only difference
// is the emitted action label.
extern const FzConverter kFzLsCommandOffDouble;

}  // namespace zhc::ls
