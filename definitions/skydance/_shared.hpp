// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Skydance TS0601 / WZ5 family shared expose + binding bundles.
//
// All five WZ5_* TS0601 ports use the same z2m wire stack:
//   fromZigbee: [legacy.fz.tuya_light_wz5]
//   toZigbee:   [legacy.tz.tuya_dimmer_state, legacy.tz.tuya_light_wz5]
// i.e. raw Tuya 0xEF00 datapoint frames carrying state, brightness, and
// (per variant) color_temp / color_hs / white_brightness.
//
// ZHC currently has no WZ5 datapoint-map factory, so every variant in
// this family is BLOCKED on the SET path until the Tuya DP runtime
// gains a `tuya_light_wz5` map (same library-wide gap as the 300+
// Tuya BLOCKED ports — see docs/parity/PARITY_SUMMARY.md).
//
// To keep the exposes table accurate for UI introspection and to make
// the binding correct (0xEF00 instead of the wrong genOnOff/genLevelCtrl
// the auto-generator emitted), the per-variant ports include this
// header and reuse the bundles below.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::skydance {

// 0xEF00 = Tuya manuSpec datapoint cluster.
// Keeping a single 1-entry binding here instead of duplicating per file.
inline constexpr ::zhc::BindingSpec kBindWZ5Tuya[] = {
    {1, 0xEF00},
};
inline constexpr std::size_t kBindWZ5TuyaCount =
    sizeof(kBindWZ5Tuya) / sizeof(kBindWZ5Tuya[0]);

// Common to all five TS0601 variants.
inline constexpr ::zhc::Expose kExpWZ5Base[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
inline constexpr std::size_t kExpWZ5BaseCount =
    sizeof(kExpWZ5Base) / sizeof(kExpWZ5Base[0]);

}  // namespace zhc::devices::skydance
