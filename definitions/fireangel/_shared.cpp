// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared FireAngel CO converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/fireangel.ts

#include "definitions/fireangel/_shared.hpp"

#include <cstddef>
#include <cstdint>

namespace zhc::fireangel {

namespace {

// Read the 16-bit zoneStatus from a ssIasZone status-change notification.
// Payload after the ZCL header is: zoneStatus:u16 LE, extStatus:u8,
// zoneId:u8, delay:u16. FireAngel sends no manufacturer code.
bool read_zone_status(const ::zhc::DecodedMessage& msg, std::uint16_t& status) {
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    status = static_cast<std::uint16_t>(msg.raw_data[hdr]) |
             (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
    return true;
}

// z2m fz.W2_module_carbon_monoxide: carbon_monoxide = (zoneStatus & (1<<8)) > 8.
// Bit 8 is 0x100 (>8 always holds when set), i.e. a plain bit-8 test.
bool fz_w2_carbon_monoxide(const ::zhc::DecodedMessage& msg,
                           const ::zhc::FzConverter&,
                           const ::zhc::PreparedDefinition&,
                           ::zhc::RuntimeContext&,
                           ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t status = 0;
    if (!read_zone_status(msg, status)) return false;
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::Bool;
    v.b    = (status & (1u << 8)) != 0;
    out.put("carbon_monoxide", v);
    return true;
}

// z2m fzLocal.fireangel_co_test: testActive = (zoneStatus & (1<<5)) ||
// (zoneStatus & (1<<9)). The 8 s auto-clear timeout is not modelled host-side;
// we emit the instantaneous flag.
bool fz_fireangel_co_test(const ::zhc::DecodedMessage& msg,
                          const ::zhc::FzConverter&,
                          const ::zhc::PreparedDefinition&,
                          ::zhc::RuntimeContext&,
                          ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t status = 0;
    if (!read_zone_status(msg, status)) return false;
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::Bool;
    v.b    = ((status & (1u << 5)) != 0) || ((status & (1u << 9)) != 0);
    out.put("test", v);
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzW2ModuleCarbonMonoxide{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_w2_carbon_monoxide },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzFireangelCoTest{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_fireangel_co_test },
    .user_config       = nullptr,
};

}  // namespace zhc::fireangel
