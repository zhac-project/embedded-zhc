// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared IMOU (Dahua) converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/imou.ts
//             (`imouAlarmButton()` + ZGA1-EN `m.iasZoneAlarm`) +
//             lib/modernExtend.ts `iasZoneAlarm`.

#include "definitions/imou/_shared.hpp"

#include <cstddef>
#include <cstdint>

namespace zhc::devices::imou {

namespace {

// Pull the 16-bit zoneStatus out of an IAS Zone Status Change
// Notification (cmd 0x00). Layout after the ZCL header:
//   zoneStatus:u16 LE, extendedStatus:u8, zoneId:u8, delay:u16.
// The header is 3 bytes (fc, tsn, cmd) or 5 when manufacturer-specific.
// Returns false when the frame is too short to carry zoneStatus.
bool read_zone_status(const DecodedMessage& msg, std::uint16_t& status) {
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    status = static_cast<std::uint16_t>(msg.raw_data[hdr]) |
             (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
    return true;
}

}  // namespace

// ── ZGA1-EN gas alarm ─────────────────────────────────────────────────
bool fz_imou_gas_alarm(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t status = 0;
    if (!read_zone_status(msg, status)) return false;

    Value v{}; v.type = ValueType::Bool;
    v.b = (status & (1u << 0)) != 0; out.put("gas_alarm_1", v);
    v.b = (status & (1u << 1)) != 0; out.put("gas_alarm_2", v);
    v.b = (status & (1u << 2)) != 0; out.put("tamper",      v);
    v.b = (status & (1u << 8)) != 0; out.put("test",        v);
    return true;
}

extern const FzConverter kFzImouGasAlarm{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_imou_gas_alarm },
    .user_config       = nullptr,
};

// ── ZE1-EN wireless switch (button) ───────────────────────────────────
bool fz_imou_alarm_button(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t status = 0;
    if (!read_zone_status(msg, status)) return false;
    if (status != 2) return false;  // z2m: only zoneStatus === 2 → press

    Value a{}; a.type = ValueType::StringRef; a.str = "press";
    out.put("action", a);
    return true;
}

extern const FzConverter kFzImouAlarmButton{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_imou_alarm_button },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::imou
