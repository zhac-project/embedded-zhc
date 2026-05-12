// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Inovelli (Red / Blue series) vendor converters. Inovelli
// stuffs ~80 configuration parameters into the manufacturer-specific
// cluster `manuSpecificInovelli` (0xFC31, mfg `INOVELLI` = 0x122f) on
// endpoint 1, with a parallel mmWave companion cluster
// `manuSpecificInovelliMMWave` only on the VZM32-SN.
//
// z2m's `inovelliExtend.device({attrs, ...})` walks a JS attribute
// table at runtime, building one writeAttributes call per parameter.
// We can't build that table dynamically inside the static-memory
// runtime, so each writeable parameter we want to surface gets its
// own `ZclWriteSpec` + `TzConverter`.
//
// SCOPE: This first cut wires the 12 most commonly-tweaked
// configuration parameters from `COMMON_ATTRIBUTES` —
// ramp/dim speeds, default levels, auto-off timer, switch invert,
// state-after-power-restored, output-mode, smart-bulb-mode, and a
// pair of LED-bar globals (ledColorWhenOn, ledColorWhenOff). Per-
// device specialised attributes (relayClick, fan gears, mmWave,
// LED-effect *commands*, scene/button-tap raw frames) are tracked in
// `docs/parity/INOVELLI_PARITY.md`.
//
// z2m-source: zigbee-herdsman-converters/src/lib/inovelli.ts
//             COMMON_ATTRIBUTES, COMMON_DIMMER_ATTRIBUTES,
//             COMMON_DIMMER_ON_OFF_ATTRIBUTES.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::inovelli {

// z2m: `INOVELLI = 0x122f`. Used for every manuSpec write/report.
constexpr std::uint16_t kInovelliMfgCode = 0x122f;

// Manufacturer-specific cluster IDs (from z2m's `addCustomCluster()`).
//   0xFC31 manuSpecificInovelli
//   0xFC32 manuSpecificInovelliMMWave   (VZM32-SN only)
constexpr std::uint16_t kInovelliClusterId       = 0xFC31;
constexpr std::uint16_t kInovelliMMWaveClusterId = 0xFC32;

// ── COMMON_ATTRIBUTES — writeable scalars ─────────────────────────
//
// Every key matches the camelCase property name z2m uses in
// `inovelliExtend.device({attrs:[{attributes: VZM*_ATTRIBUTES, ...}]})`,
// which is what the SPA / front-ends learned to send.

extern const TzConverter kTzInoDimmingSpeedUpRemote;     // attr 1 u8
extern const TzConverter kTzInoDimmingSpeedUpLocal;      // attr 2 u8
extern const TzConverter kTzInoRampRateOffToOnRemote;    // attr 3 u8
extern const TzConverter kTzInoRampRateOffToOnLocal;     // attr 4 u8
extern const TzConverter kTzInoDimmingSpeedDownRemote;   // attr 5 u8
extern const TzConverter kTzInoDimmingSpeedDownLocal;    // attr 6 u8
extern const TzConverter kTzInoRampRateOnToOffRemote;    // attr 7 u8
extern const TzConverter kTzInoRampRateOnToOffLocal;     // attr 8 u8
extern const TzConverter kTzInoInvertSwitch;             // attr 11 bool
extern const TzConverter kTzInoAutoTimerOff;             // attr 12 u16
extern const TzConverter kTzInoDefaultLevelLocal;        // attr 13 u8
extern const TzConverter kTzInoDefaultLevelRemote;       // attr 14 u8
extern const TzConverter kTzInoStateAfterPowerRestored;  // attr 15 u8

// COMMON_DIMMER_ATTRIBUTES — writeable scalars (dimmer-only)
extern const TzConverter kTzInoMinimumLevel;             // attr 9  u8
extern const TzConverter kTzInoMaximumLevel;             // attr 10 u8
extern const TzConverter kTzInoOutputMode;               // attr 21 bool
extern const TzConverter kTzInoSmartBulbMode;            // attr 22 bool

}  // namespace zhc::devices::inovelli
