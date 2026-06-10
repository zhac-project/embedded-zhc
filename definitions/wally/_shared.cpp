// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Wally vendor converters. See `_shared.hpp` for the per-endpoint IAS
// routing rationale.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             U02I007C01_contact, U02I007C01_water_leak.

#include "_shared.hpp"

#include <cstddef>
#include <cstdint>

namespace zhc::devices::wally {

using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::type_bit;

namespace {

// Extract zoneStatus bit 0 from a commandStatusChangeNotification
// payload. ZCL header = 3 bytes (FC, TSN, CmdID), or 5 when
// manufacturer-specific; zoneStatus is the first u16 (LE) after it.
bool zone_bit0(const DecodedMessage& msg, bool& out_bit) {
    const std::size_t hdr = msg.manufacturer_specific ? 5u : 3u;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
    out_bit = (status & 0x0001) != 0;
    return true;
}

void put_bool(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key,
              bool value) {
    Value v{};
    v.type = ValueType::Bool;
    v.b    = value;
    out.put(key, v);
}

// endpoint 1: contact = !(zoneStatus bit0).
bool fz_contact_ep1(const DecodedMessage& msg, const FzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool bit = false;
    if (!zone_bit0(msg, bit)) return false;
    put_bool(out, "contact", !bit);
    return true;
}

// endpoint 2: water_leak = (zoneStatus bit0).
bool fz_water_leak_ep2(const DecodedMessage& msg, const FzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool bit = false;
    if (!zone_bit0(msg, bit)) return false;
    put_bool(out, "water_leak", bit);
    return true;
}

}  // namespace

extern const FzConverter kFzContactEp1{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = 1,      // z2m gates contact to endpoint 1
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_contact_ep1 },
    .user_config       = nullptr,
};

extern const FzConverter kFzWaterLeakEp2{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = 2,      // z2m gates water_leak to endpoint 2
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_water_leak_ep2 },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::wally
