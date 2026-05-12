// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "zhc/config.hpp"
#include "zhc/types.hpp"

namespace zhc {

// ZCL foundation command ids that carry attribute record streams.
constexpr std::uint8_t CMD_READ_ATTR              = 0x00;
constexpr std::uint8_t CMD_READ_ATTR_RESPONSE     = 0x01;
constexpr std::uint8_t CMD_WRITE_ATTR             = 0x02;
constexpr std::uint8_t CMD_REPORT_ATTR            = 0x0A;
constexpr std::uint8_t CMD_DEFAULT_RESPONSE       = 0x0B;

// Parsed ZCL data-type value size in bytes. Returns -1 for unknown,
// variable-length types where the byte layout depends on a size byte
// are handled inline by the caller (octet / char strings).
int zcl_type_fixed_size(std::uint8_t type);

// Read a single attribute value at `data`, given its ZCL datatype,
// and convert it into a `Value`. Returns the number of bytes consumed
// or -1 on malformed input / unsupported type.
int parse_zcl_value(std::span<const std::uint8_t> data,
                    std::uint8_t type,
                    Value& out);

// Walk every record in a Report Attributes payload (records packed as
// `attr_id(2 LE) | type(1) | value(N)`). For each record, resolve the
// attribute id to a key string and append to `out`. Unknown attribute
// ids are formatted as decimal strings into `key_scratch` (must outlive
// `out`). Known attr-id → key mapping is caller-provided via `known`.
//
// Returns true on success (payload fully consumed or cleanly exhausted)
// or false on malformed input. Partially-populated `out` is still valid.
struct AttrKeyEntry {
    std::uint16_t attr_id;
    const char*   key;
};

bool parse_report_attributes(std::span<const std::uint8_t> payload,
                              std::span<const AttrKeyEntry> known,
                              char* key_scratch,
                              std::size_t scratch_cap,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Read Attributes Response walker — per-record shape is
// `attr_id(2 LE) | status(1) | [type(1) | value(N)]` (type+value only
// when status == 0x00). Same semantics as the report walker otherwise.
bool parse_read_attr_response(std::span<const std::uint8_t> payload,
                               std::span<const AttrKeyEntry> known,
                               char* key_scratch,
                               std::size_t scratch_cap,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Parse a Xiaomi `0xFF01` / MI-struct payload. Shape: the ZCL value is
// type 0x42 (octet-string with 1-byte length prefix) whose bytes are a
// packed TLV stream: `tag(1) | type(1) | value(N)`. Writes each tag
// decoded into `arena` using the decimal tag number as the key.
bool parse_mi_struct(std::span<const std::uint8_t> struct_bytes,
                      char* key_scratch,
                      std::size_t scratch_cap,
                      FixedPayload<ZHC_MI_STRUCT_CAP>& arena);

// Parse a Tuya DP stream (command 0x01/0x02 on cluster 0xEF00). Stream
// shape after the 3-byte per-command prefix: `dp_id(1) | dp_type(1) |
// len(2 BE) | value(len)`. Fills `out_records` up to its capacity and
// writes the actual count to `count`. Returns false on malformed input.
bool parse_tuya_dp_stream(std::span<const std::uint8_t> payload,
                           std::span<TuyaDpRecord> out_records,
                           std::size_t& count);

}  // namespace zhc
