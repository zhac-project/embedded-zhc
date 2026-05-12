// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// DP-specific modern-extend helpers — typed constructors that produce
// single `TuyaDpMapEntry` records for a device's DP map. The result
// plugs straight into a `TuyaDpMapEntry[]` array; fz_tuya_datapoints
// handles the decoding uniformly.
//
// z2m-source: tuya.ts `tuya.modernExtend.dp*`.

#include <cstdint>

#include "definitions/tuya/_shared.hpp"

namespace zhc::tuya::dp {

// `dpAction(dp_id, table, n)` — enum DP mapped to an action string.
constexpr TuyaDpMapEntry action(std::uint8_t dp_id,
                                  const TuyaEnumEntry* table,
                                  std::uint8_t n) {
    return { dp_id, "action", TuyaDpType::Enum, 1, table, n };
}

// `dpNumeric(dp_id, key, divisor)` — scaled integer DP.
constexpr TuyaDpMapEntry numeric(std::uint8_t dp_id,
                                   const char* key,
                                   std::int32_t divisor = 1) {
    return { dp_id, key, TuyaDpType::Numeric, divisor, nullptr, 0 };
}

// `dpBinary(dp_id, key)` — bool DP.
constexpr TuyaDpMapEntry binary(std::uint8_t dp_id, const char* key) {
    return { dp_id, key, TuyaDpType::Bool, 1, nullptr, 0 };
}

// `dpEnumLookup(dp_id, key, table, n)` — enum DP → string label.
constexpr TuyaDpMapEntry enum_lookup(std::uint8_t dp_id,
                                      const char* key,
                                      const TuyaEnumEntry* table,
                                      std::uint8_t n) {
    return { dp_id, key, TuyaDpType::Enum, 1, table, n };
}

// `dpTemperature(dp_id, divisor)` — canonical name "temperature";
// z2m's helper defaults to /10 so e.g. 251 → 25.1 °C.
constexpr TuyaDpMapEntry temperature(std::uint8_t dp_id,
                                       std::int32_t divisor = 10) {
    return { dp_id, "temperature", TuyaDpType::Numeric, divisor, nullptr, 0 };
}

}  // namespace zhc::tuya::dp
