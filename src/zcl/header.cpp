// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/zcl/header.hpp"

namespace zhc {

bool parse_zcl_header(std::span<const std::uint8_t> in,
                      ZclHeader& out,
                      std::size_t& header_len) {
    if (in.size() < 3) return false;

    const std::uint8_t fc = in[0];
    out.frame_control               = fc;
    out.cluster_specific            = (fc & 0x03) == 0x01;
    out.manufacturer_specific       = (fc & 0x04) != 0;
    out.direction_server_to_client  = (fc & 0x08) != 0;
    out.disable_default_response    = (fc & 0x10) != 0;

    std::size_t pos = 1;
    if (out.manufacturer_specific) {
        if (in.size() < 5) return false;
        out.manufacturer_code =
            static_cast<std::uint16_t>(in[pos]) |
            (static_cast<std::uint16_t>(in[pos + 1]) << 8);
        pos += 2;
    } else {
        out.manufacturer_code = 0;
    }

    out.transaction_sequence = in[pos++];
    out.command_id           = in[pos++];
    header_len               = pos;
    return true;
}

}  // namespace zhc
