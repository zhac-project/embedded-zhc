// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Tuya DP record → `Value` decode helpers.
//
// The `manuSpecificTuya` (0xEF00) cluster carries a list of DP records
// of the shape { dp_id, dp_type, value[] } where `value` is encoded
// per Tuya's own (not-ZCL) type system. Numerics are big-endian,
// strings carry no NUL, and bitmaps come in 1/2/4-byte lengths.
//
// `parse_tuya_dp_stream` (in foundation.cpp) splits the wire frame
// into a sequence of `TuyaDpRecord`s; this helper takes those raw
// records and materialises their payloads into `Value`s that
// converters can consume uniformly.

#include <cstdint>

#include "zhc/types.hpp"

namespace zhc {

enum class TuyaDpType : std::uint8_t {
    Raw     = 0x00,
    Bool    = 0x01,
    Numeric = 0x02,   // s32 big-endian
    String  = 0x03,
    Enum    = 0x04,   // u8
    Bitmap  = 0x05,   // u8/u16/u32 depending on length
};

// Decode `rec.value` into `out`. Returns true on success; false when
// the record's bytes don't fit the declared type (truncation / bad
// length). Caller supplies `rec.dp_type` as the discriminator.
bool decode_tuya_dp(const TuyaDpRecord& rec, Value& out);

}  // namespace zhc
