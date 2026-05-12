// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace zhc {

// Parsed ZCL frame-control header. Carries every bit the dispatcher
// needs to populate `DecodedMessage` and match selectors.
struct ZclHeader {
    std::uint8_t  frame_control;
    bool          cluster_specific;           // frame type bit 0
    bool          manufacturer_specific;
    bool          direction_server_to_client;
    bool          disable_default_response;
    std::uint16_t manufacturer_code;          // 0 when not manu-specific
    std::uint8_t  transaction_sequence;
    std::uint8_t  command_id;
};

// Parse the 3- or 5-byte ZCL header from `in`. On success fills `out`
// and writes the header's byte length to `header_len`. Returns false
// on truncated input.
bool parse_zcl_header(std::span<const std::uint8_t> in,
                      ZclHeader& out,
                      std::size_t& header_len);

}  // namespace zhc
