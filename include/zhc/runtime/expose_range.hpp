// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>
#include <cstring>

// Default numeric bounds for the common, cluster-standard writable attributes,
// keyed by expose `name`. The firmware's per-device exposes (embedded-zhc) do
// not currently carry value_min/value_max, so the exposes serializer consults
// this table to fill them — covering brightness/color_temp/position/etc. without
// editing any of the ~6600 generated/hand defs. A def that sets explicit bounds
// on its `Expose` overrides this (the serializer prefers the def's values).
//
// These are the generic ZCL ranges (z2m's defaults); a device with a tighter
// physical range still works — the firmware clamps, and a UI slider over the
// generic range is fine. Per-device-exact ranges are an additive future step
// (the generator emitting explicit bounds into definitions/*/generated/*).
namespace zhc {

// Fills [min, max] (+ step) for a known standard numeric attribute by name.
// Returns false (and leaves the outputs untouched) when the name is unknown.
inline bool default_numeric_range(const char* name,
                                  std::int32_t& min,
                                  std::int32_t& max,
                                  std::int32_t& step) {
    if (name == nullptr) return false;
    struct Range { const char* name; std::int32_t min, max, step; };
    static constexpr Range kRanges[] = {
        {"brightness",          0, 254, 1},   // genLevelCtrl currentLevel
        {"color_temp",        150, 500, 1},   // genColorCtrl colorTemperature (mireds)
        {"color_temp_startup", 150, 500, 1},
        {"position",            0, 100, 1},   // closuresWindowCovering lift %
        {"tilt",                0, 100, 1},   // closuresWindowCovering tilt %
    };
    for (const auto& r : kRanges) {
        if (std::strcmp(name, r.name) == 0) {
            min = r.min; max = r.max; step = r.step;
            return true;
        }
    }
    return false;
}

}  // namespace zhc
