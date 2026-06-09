// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Shared SmartThings / Samsung vendor converters.
// z2m-source: zigbee-herdsman-converters/src/devices/smartthings.ts +
//             converters/fromZigbee.ts.

#include "definitions/smartthings/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::smartthings {
namespace {

// commandStatusChangeNotification payload (after ZCL header):
//   zoneStatus:u16 LE, extendedStatus:u8, zoneId:u8, delay:u16.
// z2m maps the whole zoneStatus value through this table → `action`.
// (Mirrors fz.command_status_change_notification_action's lookup
//  {0:"off", 1:"single", 2:"double", 3:"hold"}.)
const char* button_action_label(std::uint16_t status) {
    switch (status) {
        case 0: return "off";
        case 1: return "single";
        case 2: return "double";
        case 3: return "hold";
        default: return nullptr;   // unknown code → publish nothing
    }
}

bool fz_st_button_action(const DecodedMessage& msg, const FzConverter&,
                          const PreparedDefinition&, RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);

    const char* label = button_action_label(status);
    if (!label) return false;

    Value v{};
    v.type = ValueType::StringRef;
    v.str  = label;   // static-storage literal — outlives dispatch
    out.put("action", v);
    return true;
}

}  // namespace

extern const FzConverter kFzStButtonAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // ZoneStatusChangeNotification flows server (device) → client.
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_st_button_action },
    .user_config       = nullptr,
};

}  // namespace zhc::smartthings
