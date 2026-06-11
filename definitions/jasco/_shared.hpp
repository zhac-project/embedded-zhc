// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Jasco shared converters.
//
// z2m-source: jasco.ts #43095 via converters/fromZigbee.ts
//             command_on_state / command_off_state.
//
// The Jasco 43095 plug-in switch reports its physical button presses as
// genOnOff cluster *commands* (commandOn 0x01 / commandOff 0x00), not as
// onOff attribute (0x0000) reports. z2m wires fz.command_on_state /
// fz.command_off_state, which fold those commands into the `state`
// property ("ON" / "OFF") — NOT into `action`. The generic kFzCommandOn /
// kFzCommandOff converters emit `action="on"`/`action="off"` (the device
// has no `action` expose), so they cannot stand in here.
//
// kFzCommandOnState / kFzCommandOffState decode the same genOnOff command
// ids the generic command converters do but emit `state` as a Bool
// (true=on / false=off) — the exact representation kFzOnOff uses for the
// attribute path, so the two decode paths converge on one key.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::jasco {

// genOnOff cmd 0x01 (commandOn)  → state = true
extern const ::zhc::FzConverter kFzCommandOnState;
// genOnOff cmd 0x00 (commandOff) → state = false
extern const ::zhc::FzConverter kFzCommandOffState;

}  // namespace zhc::devices::jasco
