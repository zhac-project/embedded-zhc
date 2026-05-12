// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/zcl/decoder.hpp"

#include "zhc/zcl/header.hpp"

namespace zhc {

namespace {

constexpr std::uint16_t CLUSTER_TUYA = 0xEF00;

MessageType classify_foundation_cmd(std::uint8_t command_id) {
    switch (command_id) {
        case CMD_READ_ATTR:          return MessageType::Read;
        case CMD_READ_ATTR_RESPONSE: return MessageType::ReadResponse;
        case CMD_WRITE_ATTR:         return MessageType::Write;
        case CMD_REPORT_ATTR:        return MessageType::AttributeReport;
        default:                     return MessageType::Raw;
    }
}

}  // namespace

bool decode_frame(const InboundApsFrame& frame,
                  std::span<const AttrKeyEntry> known,
                  DecodedMessage& out) {
    out = DecodedMessage{};

    ZclHeader hdr{};
    std::size_t hlen = 0;
    if (!parse_zcl_header(frame.data, hdr, hlen)) return false;

    const auto body = frame.data.subspan(hlen);

    // Tuya family covers only the DP-stream command ids. Other
    // cluster-specific commands on cluster 0xEF00 (mcuSyncTime, etc.)
    // are classified as normal Zcl so cluster-specific FzConverters
    // match them without going through the DP parser.
    const bool is_tuya_dp_cmd =
        hdr.command_id == 0x01 ||
        hdr.command_id == 0x02 ||
        hdr.command_id == 0x06;
    out.family = (frame.cluster_id == CLUSTER_TUYA &&
                   hdr.cluster_specific && is_tuya_dp_cmd)
                 ? FrameFamily::TuyaDp
                 : FrameFamily::Zcl;
    out.cluster               = nullptr;  // dispatch layer maps id → name
    out.direction             = hdr.direction_server_to_client
                                ? Direction::ServerToClient
                                : Direction::ClientToServer;
    out.command_id            = hdr.command_id;
    out.transaction_sequence  = hdr.transaction_sequence;
    out.manufacturer_specific = hdr.manufacturer_specific;
    out.manufacturer_code     = hdr.manufacturer_code;
    out.src_endpoint          = frame.src_endpoint;
    out.dst_endpoint          = frame.dst_endpoint;
    out.linkquality           = frame.linkquality;
    out.group_id              = frame.group_id;
    out.raw_data              = frame.data;
    out.raw_body              = body;

    out.type = hdr.cluster_specific
               ? MessageType::Command
               : classify_foundation_cmd(hdr.command_id);

    switch (out.type) {
        case MessageType::AttributeReport:
            return parse_report_attributes(body, known,
                                           out.unknown_key_scratch,
                                           sizeof(out.unknown_key_scratch),
                                           out.payload);
        case MessageType::ReadResponse:
            return parse_read_attr_response(body, known,
                                            out.unknown_key_scratch,
                                            sizeof(out.unknown_key_scratch),
                                            out.payload);
        default:
            // Read / Write / Command / Raw — body stays in `raw_data`
            // for cluster-specific or Tuya converters to walk.
            return true;
    }
}

}  // namespace zhc
