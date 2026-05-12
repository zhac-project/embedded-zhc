// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Moes-local shared helpers.
//
// Moes is a pure Tuya OEM — every device routes through Tuya's
// manuSpecificTuya (0xEF00) cluster + DP stream. The "factories"
// `moesS` / `moesSwitch` in z2m wrap `tuya.modernExtend.tuyaBase` and
// `tuya.modernExtend.tuyaOnOff` with Moes-specific DP maps + labels.
//
// In our static-memory world, these wrappers collapse into type
// aliases: a `moes::MoesS<Cfg>` is structurally identical to
// `tuya::factory::TuyaOnOff<Cfg>` but its distinct template name gives
// the platform adapter a way to tell "this is Moes" via
// `typeid`-style comparisons if it ever cares (today it doesn't).
//
// z2m-source: zigbee-herdsman-converters/src/devices/moes.ts.

#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::moes {

// `moesS<Cfg>` — multi-state Moes wall panels / TRVs / scene remotes.
// Cfg must expose `static constexpr tuya::TuyaDatapointMap dp_map`.
template <typename Cfg>
using MoesS = ::zhc::tuya::factory::TuyaOnOff<Cfg>;

// `moesSwitch<Cfg>` — Moes wall / relay switches with power-on-
// behavior + indicator-mode on top of tuyaOnOff.
template <typename Cfg>
using MoesSwitch = ::zhc::tuya::factory::TuyaOnOff<Cfg>;

}  // namespace zhc::moes
