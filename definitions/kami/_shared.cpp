// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Shared KAMI (Yi) vendor converters.
// z2m-source: zigbee-herdsman-converters/src/devices/kami.ts +
//             converters/fromZigbee.ts (KAMI_contact, KAMI_occupancy).

#include "definitions/kami/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::kami {
namespace {

// z2m's KAMI converters index the absolute byte 7 of the raw ZCL frame
// (the full buffer incl. ZCL header), regardless of cluster semantics —
// this is non-standard Yi/KAMI firmware. `raw_data` here is the full
// frame, so `raw_data[7]` matches z2m's `msg.data[7]` exactly.
constexpr std::size_t kKamiRawByte = 7;

// fz.KAMI_contact: {contact: msg.data[7] === 0}
bool fz_kami_contact(const DecodedMessage& msg, const FzConverter&,
                      const PreparedDefinition&, RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_data.size() <= kKamiRawByte) return false;
    Value v{};
    v.type = ValueType::Bool;
    v.b    = (msg.raw_data[kKamiRawByte] == 0);
    out.put("contact", v);
    return true;
}

// fz.KAMI_occupancy: if (msg.data[7] === 1) return {action: "motion"};
bool fz_kami_occupancy(const DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_data.size() <= kKamiRawByte) return false;
    if (msg.raw_data[kKamiRawByte] != 1) return false;   // z2m: publish nothing
    Value v{};
    v.type = ValueType::StringRef;
    v.str  = "motion";   // static-storage literal — outlives dispatch
    out.put("action", v);
    return true;
}

}  // namespace

extern const FzConverter kFzKamiContact{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::Command) |
                         type_bit(MessageType::Raw),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // Device → coordinator.
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_kami_contact },
    .user_config       = nullptr,
};

extern const FzConverter kFzKamiOccupancy{
    .family            = FrameFamily::Zcl,
    .cluster           = "msOccupancySensing",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::Command) |
                         type_bit(MessageType::Raw),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // Device → coordinator.
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_kami_occupancy },
    .user_config       = nullptr,
};

}  // namespace zhc::kami
