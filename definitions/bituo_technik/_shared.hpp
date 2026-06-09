// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared BITUO TECHNIK vendor converters (DIN-rail energy meters:
// SDM01/SDM02 single/split-phase, SPM01/SPM02 three-phase; U00/U01/U02
// firmware SKUs).
//
// Every bituo meter runs z2m's `m.electricityMeter` with
// `producedEnergy/acFrequency/powerFactor` enabled (plus `threePhase`
// on SDM01W / SPM02) and a pair of vendor `fromZigbee` converters
// (`bituo_fz.total_power`, `bituo_fz.phase_energy`). Those decode a long
// list of STANDARD `haElectricalMeasurement` (0x0B04) and `seMetering`
// (0x0702) attributes that the generic `kFzElectricalMeasurement`
// (power 0x050B / voltage 0x0505 / current 0x0508 only) and
// `kFzMetering` (energy 0x0000 / power 0x0400 only) drop on the floor —
// so produced_energy, ac_frequency, power_factor, power_reactive,
// power_apparent, total_power*, the per-phase b/c voltage/current/power,
// and the per-phase tier-summation energy counters were dead exposes.
//
// Bituo uses NO manufacturer-specific cluster: `bituo_fz.total_power`
// reads the standard `totalActive/Reactive/ApparentPower` attrs and
// `bituo_fz.phase_energy` reads the standard `currentTierN
// SummDelivered/Received` attrs — vendor-assigned slots inside the
// standard clusters, no `manufacturerCode` byte on the wire. So these
// two extras converters wire ALONGSIDE the generic metering/electrical
// pair on every def (Owon / Ubisys extras pattern), emitting only the
// attrs the generics skip.
//
// Scaling: like the generic converters (and matching z2m, whose
// `bituo_fz.electrical_measurement` pre-multiplies power attrs by
// acPowerDivisor only for `fz.electrical_measurement` to divide them
// back out), every channel is a RAW pass-through — the runtime applies
// the cluster multiplier/divisor downstream.
//
// Devices pulling from this header carry a `// Tier 2: uses shared
// bituo_technik converters` comment plus a `// z2m-source:` line.
//
// z2m-source: zigbee-herdsman-converters/src/devices/bituo_technik.ts
//             (bituo_fz.total_power / bituo_fz.phase_energy) +
//             lib/modernExtend.ts genericMeter (electricityMeter
//             threePhase / producedEnergy / acFrequency / powerFactor).

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::bituo_technik {

// ── fz: haElectricalMeasurement (0x0B04) channels the generic skips ──
//
//   0x0300 acFrequency        -> ac_frequency
//   0x0510 powerFactor        -> power_factor
//   0x050E reactivePower      -> power_reactive
//   0x050F apparentPower      -> power_apparent
//   0x0304 totalActivePower   -> total_power
//   0x0305 totalReactivePower -> total_power_reactive
//   0x0306 totalApparentPower -> total_power_apparent
//   0x090B activePowerPhB     -> power_phase_b   0x0A0B activePowerPhC -> power_phase_c
//   0x0905 rmsVoltagePhB      -> voltage_phase_b 0x0A05 rmsVoltagePhC -> voltage_phase_c
//   0x0908 rmsCurrentPhB      -> current_phase_b 0x0A08 rmsCurrentPhC -> current_phase_c
extern const FzConverter kFzBituoElectricalMeasurementExtras;

// ── fz: seMetering (0x0702) channels the generic skips ──────────────
//
//   0x0001 currentSummReceived       -> produced_energy
//   0x0102 currentTier1SummDelivered -> energy_phase_a
//   0x0103 currentTier1SummReceived  -> produced_energy_phase_a
//   0x0104 currentTier2SummDelivered -> energy_phase_b
//   0x0105 currentTier2SummReceived  -> produced_energy_phase_b
//   0x0106 currentTier3SummDelivered -> energy_phase_c
//   0x0107 currentTier3SummReceived  -> produced_energy_phase_c
extern const FzConverter kFzBituoMeteringExtras;

}  // namespace zhc::bituo_technik
