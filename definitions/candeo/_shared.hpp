// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Candeo vendor converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/candeo.ts
//
// Three custom slices live here:
//
//   1. external_switch_type — Candeo's manu-specific genBasic
//      attribute 0x8803 (mfgcode 0x1224).  Decoded as
//      "external_switch_type" enum ("momentary" / "toggle"). Used by
//      C203 / C204 / C205 / C-ZB-DM204 / C-ZB-SM205-2G — five SKUs,
//      so this lives shared instead of inline.
//
//   2. RD1P knob press / rotation actions — the C-ZB-RD1P-DPM and
//      C-ZB-RD1P-REM publish endpoint-2 genOnOff / genLevelCtrl
//      cluster commands (no payload) which are remapped to a fixed
//      action vocabulary per `rd1pKnobActionsMap` in upstream.
//
//   3. candeoRotaryRemoteControl (cluster 0xFF03, mfgcode 0x115F per
//      m.deviceAddCustomCluster) used only by C-ZB-SR5BR. Tracked as
//      BLOCKED in CANDEO_PARITY.md — the cluster id 0xFF03 isn't yet
//      mapped in cluster_names.hpp so the dispatcher can't route a
//      cluster-string match here without a wildcard guard. Documented
//      and stubbed.
//
// Devices that pull from this header should carry a `// Tier 2: uses
// shared candeo converters` comment.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::candeo {

// Manufacturer code under which Candeo's switch-type attribute is
// envelope-tagged on the wire. The dispatcher already strips the
// envelope before reaching the converter, so we only check this in
// the encoder for the WriteAttributes path.
constexpr std::uint16_t kSwitchTypeMfgCode = 0x1224;

// genBasic (0x0000) attribute 0x8803 (enum8 — 0=momentary, 1=toggle).
// FZ decodes attribute reports / read responses; emits
//   external_switch_type = "momentary" | "toggle"
// Direction: Server→Client (remote reading our cluster server).
extern const ::zhc::FzConverter kFzExternalSwitchType;

// TZ writes the same attribute. Accepts string "momentary"/"toggle"
// or raw uint (0/1). Emits a manu-specific WriteAttributes (FC=0x14)
// with manufacturer_code = 0x1224.
extern const ::zhc::TzConverter kTzExternalSwitchType;

// ── RD1P (C-ZB-RD1P-DPM / C-ZB-RD1P-REM) endpoint-2 knob actions ─────
//
// The knob is reported as cluster commands on endpoint 2:
//
//   genOnOff (0x0006):
//     cmd 0x00 commandOff           → action = "double_pressed"
//     cmd 0x01 commandOn            → action = "pressed"
//     cmd 0x02 commandToggle        → action = "held"
//     cmd 0x03 commandRelease       → action = "released"
//
//   genLevelCtrl (0x0008):
//     cmd 0x05 commandMoveWithOnOff → action = "started_rotating_<dir>"
//                                      where dir = "left" if rate<0 else "right"
//                                      (kept as raw "started_rotating" in this
//                                      stateless decoder — direction lives in
//                                      payload byte 0)
//     cmd 0x06 commandStepWithOnOff → action = "rotating_<dir>"
//     cmd 0x07 commandStop          → action = "stopped_rotating"
//
// The two FZ entries below are the press- and rotation-side decoders.
// They run on Direction::ClientToServer.
extern const ::zhc::FzConverter kFzRd1pKnobPress;
extern const ::zhc::FzConverter kFzRd1pKnobRotation;

}  // namespace zhc::candeo
