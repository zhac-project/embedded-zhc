// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared AMINA vendor converter (single device: amina S EV Charger).
//
// The amina S drives its AC metering through z2m's
// `m.electricityMeter({cluster: "electrical", acFrequency: true,
// threePhase: true})` plus a standalone `m.numeric total_active_power`.
// That expands (lib/modernExtend.ts genericMeter, `cluster:"electrical"`
// branch + the `threePhase` block) to a STANDARD `haElectricalMeasurement`
// (0x0B04) expose set:
//
//   power / voltage / current               (base electrical branch)
//   ac_frequency                            (acFrequency:true)
//   power_phase_b/c, voltage_phase_b/c,
//   current_phase_b/c                       (threePhase:true)
//
// plus the standalone `total_active_power` numeric (attribute
// `totalActivePower` 0x0304, key `total_active_power`, unit kW).
//
// The generic `kFzElectricalMeasurement` only decodes activePower 0x050B
// (→power), rmsVoltage 0x0505 (→voltage) and rmsCurrent 0x0508
// (→current). So `ac_frequency`, every per-phase b/c channel, and the
// device's PRIMARY instantaneous-power reading (`total_active_power`,
// which the firmware reports via `totalActivePower` 0x0304 — confirmed
// by amina.ts `fzLocal.poll_energy`, which keys its energy poll off
// `msg.data.totalActivePower`) were dropped on the floor.
//
// `kFzAminaElectricalMeasurementExtras` wires ALONGSIDE the generic
// `kFzElectricalMeasurement` on the def (Owon / Bituo extras pattern),
// emitting only the standard attrs the generic skips. No
// manufacturer-specific cluster is involved here — these are vanilla
// 0x0B04 attribute ids. (The EV-specific state / alarms / charge-limit /
// offline-config channels ride amina's manuSpecific cluster 0xFEE7 and
// are decoded by the per-device converters in Ami_amina_S.cpp.)
//
// Scaling: raw pass-through, matching the generic converters — the
// runtime applies the cluster multiplier/divisor downstream.
//
// z2m-source: zigbee-herdsman-converters/src/devices/amina.ts +
//             lib/modernExtend.ts genericMeter (electricityMeter
//             cluster:"electrical" / acFrequency / threePhase).

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::amina {

// ── fz: haElectricalMeasurement (0x0B04) channels the generic skips ──
//
//   0x0300 acFrequency       -> ac_frequency
//   0x0304 totalActivePower  -> total_active_power   (s32; device's
//                               primary instantaneous-power reading)
//   0x090B activePowerPhB    -> power_phase_b    0x0A0B activePowerPhC -> power_phase_c
//   0x0905 rmsVoltagePhB     -> voltage_phase_b  0x0A05 rmsVoltagePhC  -> voltage_phase_c
//   0x0908 rmsCurrentPhB     -> current_phase_b  0x0A08 rmsCurrentPhC  -> current_phase_c
extern const FzConverter kFzAminaElectricalMeasurementExtras;

}  // namespace zhc::devices::amina
