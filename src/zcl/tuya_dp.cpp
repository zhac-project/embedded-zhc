// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/zcl/tuya_dp.hpp"

#include <cstdint>

namespace zhc {

bool decode_tuya_dp(const TuyaDpRecord& rec, Value& out) {
    const auto dp_t = static_cast<TuyaDpType>(rec.dp_type);
    const auto& b   = rec.value;

    switch (dp_t) {
        case TuyaDpType::Raw:
            out.type  = ValueType::BytesRef;
            out.bytes = b;
            return true;

        case TuyaDpType::Bool:
            if (b.size() != 1) return false;
            out.type = ValueType::Bool;
            out.b    = b[0] != 0;
            return true;

        case TuyaDpType::Numeric: {
            // Signed 32-bit big-endian — unlike ZCL's little-endian.
            if (b.size() != 4) return false;
            const std::uint32_t u =
                (static_cast<std::uint32_t>(b[0]) << 24) |
                (static_cast<std::uint32_t>(b[1]) << 16) |
                (static_cast<std::uint32_t>(b[2]) <<  8) |
                 static_cast<std::uint32_t>(b[3]);
            out.type = ValueType::Int;
            out.i    = static_cast<std::int32_t>(u);  // sign-reinterpret
            return true;
        }

        case TuyaDpType::String:
            // Tuya strings lack NUL termination; surface as BytesRef
            // so the receiving converter can craft a string_view
            // without owning storage.
            out.type  = ValueType::BytesRef;
            out.bytes = b;
            return true;

        case TuyaDpType::Enum:
            if (b.size() != 1) return false;
            out.type = ValueType::Uint;
            out.u    = b[0];
            return true;

        case TuyaDpType::Bitmap: {
            std::uint64_t u = 0;
            if (b.size() == 1)      u = b[0];
            else if (b.size() == 2) u = (std::uint64_t(b[0]) << 8) | b[1];
            else if (b.size() == 4) u = (std::uint64_t(b[0]) << 24) |
                                        (std::uint64_t(b[1]) << 16) |
                                        (std::uint64_t(b[2]) <<  8) |
                                         std::uint64_t(b[3]);
            else return false;
            out.type = ValueType::Uint;
            out.u    = u;
            return true;
        }
    }
    return false;
}

}  // namespace zhc
