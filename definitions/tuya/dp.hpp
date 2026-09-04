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

// `dpBinaryInv(dp_id, key)` — bool DP with inverted polarity, i.e. z2m's
// `valueConverter.trueFalse0` (wire 0 => true). Sets kTuyaDpFlagInvertBool;
// fz_tuya_datapoints flips the decoded bool. Used by alarm DPs (gas/smoke)
// whose "detected" state reports as 0.
constexpr TuyaDpMapEntry binary_inv(std::uint8_t dp_id, const char* key) {
    return { dp_id, key, TuyaDpType::Bool, 1, nullptr, 0, kTuyaDpFlagInvertBool };
}

// `dpPositionInv(dp_id, key)` — cover-position Numeric DP whose firmware counts
// opposite to z2m: decode emits `100 - raw`, encode sends `100 - value`
// (kTuyaDpFlagInvertPosition). For covers in z2m `coverPositionInvert`.
constexpr TuyaDpMapEntry position_inv(std::uint8_t dp_id, const char* key) {
    return { dp_id, key, TuyaDpType::Numeric, 1, nullptr, 0, kTuyaDpFlagInvertPosition };
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

// ── packed-payload datapoints ────────────────────────────────────────
//
// These map ONE datapoint onto SEVERAL keys, so they take a key table
// instead of a single `key`. See TuyaDpExpandFn in _shared.hpp.

// `dpPhaseVariant2(dp_id, keys)` — 8-byte voltage/current/power blob,
// upstream's narrow-read variant. Pass `&kTuyaPhaseKeysPlain` for the
// unsuffixed triple.
constexpr TuyaDpMapEntry phase_variant2(std::uint8_t dp_id,
                                          const TuyaPhaseKeys* keys) {
    return { dp_id, nullptr, TuyaDpType::Raw, 1, nullptr, 0, 0, 0.0f,
             &tuya_dp_expand_phase_variant2, keys };
}

// `dpPhaseVariant2WithPhase(dp_id, keys)` — same blob and the same 16-bit
// reads, plus the negative-power offset correction, emitting the per-phase
// key triple `keys` names.
constexpr TuyaDpMapEntry phase_variant2_with_phase(std::uint8_t dp_id,
                                                     const TuyaPhaseKeys* keys) {
    return { dp_id, nullptr, TuyaDpType::Raw, 1, nullptr, 0, 0, 0.0f,
             &tuya_dp_expand_phase_variant2_phase, keys };
}

// `dpThresholds(dp_id, table)` — packed [id, enabled, value_be16] records.
constexpr TuyaDpMapEntry thresholds(std::uint8_t dp_id,
                                      const TuyaThresholdTable* table) {
    return { dp_id, nullptr, TuyaDpType::Raw, 1, nullptr, 0, 0, 0.0f,
             &tuya_dp_expand_thresholds, table };
}

// `dpFaultBitmap(dp_id, key, table)` — bitmap of fault flags joined into one
// comma-separated string under `key`.
constexpr TuyaDpMapEntry fault_bitmap(std::uint8_t dp_id, const char* key,
                                        const TuyaFaultTable* table) {
    return { dp_id, key, TuyaDpType::Bitmap, 1, nullptr, 0, 0, 0.0f,
             &tuya_dp_expand_fault_bitmap, table };
}

}  // namespace zhc::tuya::dp
