// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/zcl/foundation.hpp"

#include <cstdio>
#include <cstring>

namespace zhc {

int zcl_type_fixed_size(std::uint8_t type) {
    switch (type) {
        // 1-byte
        case 0x08: case 0x10: case 0x18: case 0x20:
        case 0x28: case 0x30:
            return 1;
        // 2-byte
        case 0x09: case 0x19: case 0x21: case 0x29:
        case 0x31: case 0x38:
            return 2;
        // 3-byte
        case 0x0A: case 0x1A: case 0x22: case 0x2A:
            return 3;
        // 4-byte
        case 0x0B: case 0x1B: case 0x23: case 0x2B:
        case 0x39:
            return 4;
        // 5..8-byte
        case 0x0C: case 0x1C: case 0x24: case 0x2C: return 5;
        case 0x0D: case 0x1D: case 0x25: case 0x2D: return 6;
        case 0x0E: case 0x1E: case 0x26: case 0x2E: return 7;
        case 0x0F: case 0x1F: case 0x27: case 0x2F:
        case 0x3A:
            return 8;
        case 0x00:  // no data
            return 0;
        default:
            return -1;
    }
}

namespace {

// Format the decimal representation of `id` into `scratch[offset..]`.
// Returns pointer into scratch where the NUL-terminated string begins,
// or nullptr when the buffer would overflow. Advances `offset`.
const char* format_decimal_key(std::uint16_t id,
                                char* scratch,
                                std::size_t scratch_cap,
                                std::size_t& offset) {
    // 5 digits + NUL = max 6 bytes per key.
    if (offset + 6 > scratch_cap) return nullptr;
    char* dst = scratch + offset;
    int written = std::snprintf(dst, 6, "%u", static_cast<unsigned>(id));
    if (written < 0 || static_cast<std::size_t>(written) >= 6) return nullptr;
    offset += static_cast<std::size_t>(written) + 1;  // include NUL
    return dst;
}

const char* lookup_key(std::uint16_t attr_id,
                        std::span<const AttrKeyEntry> known) {
    for (const auto& e : known) {
        if (e.attr_id == attr_id) return e.key;
    }
    return nullptr;
}

// Helper: decode a ZCL value into `Value`. Returns bytes consumed or -1.
int decode_value(std::span<const std::uint8_t> data,
                 std::uint8_t type,
                 Value& out) {
    const int fixed = zcl_type_fixed_size(type);
    if (fixed > 0) {
        if (data.size() < static_cast<std::size_t>(fixed)) return -1;
        out.type = ValueType::Uint;  // default; refine for signed/float
        std::uint64_t u = 0;
        for (int i = 0; i < fixed; ++i) {
            u |= static_cast<std::uint64_t>(data[i]) <<
                 (8 * static_cast<unsigned>(i));
        }
        switch (type) {
            case 0x10:  // bool
                out.type = ValueType::Bool;
                out.b = u != 0;
                break;
            case 0x28: case 0x29: case 0x2A: case 0x2B:
            case 0x2C: case 0x2D: case 0x2E: case 0x2F: {
                // signed integer — sign-extend
                std::int64_t signed_val = static_cast<std::int64_t>(u);
                const int bits = fixed * 8;
                if (bits < 64) {
                    const std::int64_t sign_bit = std::int64_t{1} << (bits - 1);
                    if (signed_val & sign_bit) {
                        signed_val |= -(std::int64_t{1} << bits);
                    }
                }
                out.type = ValueType::Int;
                out.i = signed_val;
                break;
            }
            case 0x39: {
                float f = 0.0f;
                std::memcpy(&f, &u, sizeof(f));
                out.type = ValueType::Float;
                out.f = f;
                break;
            }
            default:
                out.u = u;
                break;
        }
        return fixed;
    }
    if (fixed == 0) {
        out.type = ValueType::None;
        return 0;
    }
    // Variable-length: octet-string 0x41 / char-string 0x42 — 1-byte len.
    // Both map to `BytesRef` because `Value::str` can't carry a length; the
    // Xiaomi MI-struct is transported inside a "char string" (0x42) and
    // contains NUL bytes mid-stream. Callers treating the span as text
    // should construct `std::string_view{ (const char*)bytes.data(),
    // bytes.size() }` at the boundary.
    if (type == 0x41 || type == 0x42) {
        if (data.empty()) return -1;
        const std::uint8_t len = data[0];
        if (data.size() < static_cast<std::size_t>(len) + 1) return -1;
        out.type  = ValueType::BytesRef;
        out.bytes = data.subspan(1, len);
        return 1 + len;
    }
    // Long octet / char string: 2-byte length. Both stored as BytesRef:
    // the raw radio buffer is length-prefixed with no trailing NUL, so a
    // StringRef pointer would tempt callers into running strlen/strcmp
    // off the end of the string into adjacent frame bytes. ValueType::
    // StringRef is reserved for converter-constructed literal C strings.
    if (type == 0x43 || type == 0x44) {
        if (data.size() < 2) return -1;
        const std::uint16_t len =
            static_cast<std::uint16_t>(data[0]) |
            (static_cast<std::uint16_t>(data[1]) << 8);
        if (data.size() < static_cast<std::size_t>(len) + 2) return -1;
        out.type  = ValueType::BytesRef;
        out.bytes = data.subspan(2, len);
        return 2 + len;
    }
    return -1;
}

}  // namespace

int parse_zcl_value(std::span<const std::uint8_t> data,
                    std::uint8_t type,
                    Value& out) {
    return decode_value(data, type, out);
}

bool parse_report_attributes(std::span<const std::uint8_t> payload,
                              std::span<const AttrKeyEntry> known,
                              char* key_scratch,
                              std::size_t scratch_cap,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::size_t pos = 0;
    std::size_t scratch_offset = 0;
    while (pos + 3 <= payload.size()) {
        const std::uint16_t attr_id =
            static_cast<std::uint16_t>(payload[pos]) |
            (static_cast<std::uint16_t>(payload[pos + 1]) << 8);
        const std::uint8_t type = payload[pos + 2];
        const auto value_span   = payload.subspan(pos + 3);

        Value v{};
        const int consumed = decode_value(value_span, type, v);
        if (consumed < 0) return false;
        // Advance before any skip so we keep walking the frame.
        pos += 3 + static_cast<std::size_t>(consumed);

        const char* key = lookup_key(attr_id, known);
        if (!key) {
            key = format_decimal_key(attr_id, key_scratch,
                                      scratch_cap, scratch_offset);
            // Scratch exhausted — drop this single unknown attr, keep
            // walking. Earlier C-2 behaviour returned `false` here and
            // the adapter discarded every already-decoded attr, which
            // silently lost state on Aqara/Lumi multi-attr reports.
            if (!key) continue;
        }
        if (!out.put(key, v)) return false;  // payload full
    }
    return pos == payload.size();
}

bool parse_read_attr_response(std::span<const std::uint8_t> payload,
                               std::span<const AttrKeyEntry> known,
                               char* key_scratch,
                               std::size_t scratch_cap,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::size_t pos = 0;
    std::size_t scratch_offset = 0;
    while (pos + 3 <= payload.size()) {
        const std::uint16_t attr_id =
            static_cast<std::uint16_t>(payload[pos]) |
            (static_cast<std::uint16_t>(payload[pos + 1]) << 8);
        const std::uint8_t status = payload[pos + 2];
        pos += 3;
        if (status != 0x00) {
            // No datatype/value for non-success records — skip.
            continue;
        }
        if (pos >= payload.size()) return false;
        const std::uint8_t type = payload[pos++];
        const auto value_span   = payload.subspan(pos);

        Value v{};
        const int consumed = decode_value(value_span, type, v);
        if (consumed < 0) return false;
        pos += static_cast<std::size_t>(consumed);

        const char* key = lookup_key(attr_id, known);
        if (!key) {
            key = format_decimal_key(attr_id, key_scratch,
                                      scratch_cap, scratch_offset);
            // See parse_report_attributes — drop only the unknown attr,
            // not the whole frame (C-2).
            if (!key) continue;
        }
        if (!out.put(key, v)) return false;
    }
    return pos == payload.size();
}

bool parse_mi_struct(std::span<const std::uint8_t> struct_bytes,
                      char* key_scratch,
                      std::size_t scratch_cap,
                      FixedPayload<ZHC_MI_STRUCT_CAP>& arena) {
    std::size_t pos = 0;
    std::size_t scratch_offset = 0;
    while (pos + 2 <= struct_bytes.size()) {
        const std::uint8_t tag  = struct_bytes[pos];
        const std::uint8_t type = struct_bytes[pos + 1];
        const auto value_span   = struct_bytes.subspan(pos + 2);

        Value v{};
        const int consumed = decode_value(value_span, type, v);
        if (consumed < 0) return false;

        if (scratch_offset + 6 > scratch_cap) return false;
        char* key = key_scratch + scratch_offset;
        const int n = std::snprintf(key, 6, "%u", static_cast<unsigned>(tag));
        if (n < 0 || static_cast<std::size_t>(n) >= 6) return false;
        scratch_offset += static_cast<std::size_t>(n) + 1;

        if (!arena.put(key, v)) return false;
        pos += 2 + static_cast<std::size_t>(consumed);
    }
    return pos == struct_bytes.size();
}

bool parse_tuya_dp_stream(std::span<const std::uint8_t> payload,
                           std::span<TuyaDpRecord> out_records,
                           std::size_t& count) {
    count = 0;
    std::size_t pos = 0;
    while (pos + 4 <= payload.size()) {
        const std::uint8_t  dp_id = payload[pos];
        const std::uint8_t  dp_t  = payload[pos + 1];
        const std::uint16_t len   =
            static_cast<std::uint16_t>(payload[pos + 2]) << 8 |
            static_cast<std::uint16_t>(payload[pos + 3]);     // big-endian
        pos += 4;
        if (pos + len > payload.size()) return false;
        if (count >= out_records.size()) {
            // Out of slots: report success with `count == capacity` so
            // the caller still sees the DPs already decoded. Previous
            // `return false` discarded every valid DP in the frame.
            return true;
        }
        out_records[count++] = TuyaDpRecord{
            dp_id, dp_t, payload.subspan(pos, len),
        };
        pos += len;
    }
    return pos == payload.size();
}

}  // namespace zhc
