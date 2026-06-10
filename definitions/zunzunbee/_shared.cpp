// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Zunzunbee vendor converters — see _shared.hpp for the wire-shape
// rationale. z2m-source: zunzunbee.ts (fzLocal.fzZunzunbeeSlateSwitchIAS).

#include "definitions/zunzunbee/_shared.hpp"

namespace zhc::zunzunbee {

namespace {

// Pull the u16 zoneStatus out of whichever wire shape arrived (mirrors
// z2m which reads `msg.data.zoneStatus`/`zonestatus`). Returns false
// when neither carries it.
//   * commandStatusChangeNotification (cmd 0x00): zoneStatus:u16 LE is
//     the first body field after the ZCL header.
//   * attributeReport / readResponse: zoneStatus is attr 0x0002, which
//     the parser surfaces under payload key "2".
bool read_zone_status(const DecodedMessage& msg, std::uint16_t& status) {
    if (msg.type == MessageType::Command) {
        const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
        if (msg.raw_data.size() < hdr + 2) return false;
        status = static_cast<std::uint16_t>(msg.raw_data[hdr]) |
                 (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
        return true;
    }
    const Value* v = msg.payload.find("2");
    if (!v || v->type != ValueType::Uint) return false;
    status = static_cast<std::uint16_t>(v->u);
    return true;
}

// Static-storage action labels — index [button-1][press_type] where
// press_type 0 = short_press, 1 = long_press. The StringRef pointer must
// outlive dispatch; string literals satisfy that.
const char* const kActions[8][2] = {
    {"button_1_short_press", "button_1_long_press"},
    {"button_2_short_press", "button_2_long_press"},
    {"button_3_short_press", "button_3_long_press"},
    {"button_4_short_press", "button_4_long_press"},
    {"button_5_short_press", "button_5_long_press"},
    {"button_6_short_press", "button_6_long_press"},
    {"button_7_short_press", "button_7_long_press"},
    {"button_8_short_press", "button_8_long_press"},
};

// bits 1..8 one-hot → button index 0..7. Mirrors z2m's
// {2:1,4:2,8:3,16:4,32:5,64:6,128:7,256:8} lookup on (zoneStatus & 0x01FE).
bool masked_to_button(std::uint16_t masked, int& button_idx) {
    switch (masked) {
        case 2:   button_idx = 0; return true;
        case 4:   button_idx = 1; return true;
        case 8:   button_idx = 2; return true;
        case 16:  button_idx = 3; return true;
        case 32:  button_idx = 4; return true;
        case 64:  button_idx = 5; return true;
        case 128: button_idx = 6; return true;
        case 256: button_idx = 7; return true;
        default:  return false;
    }
}

bool fz_slate_switch(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t status = 0;
    if (!read_zone_status(msg, status)) return false;

    // Bit 0 encodes press type (1 = long_press, 0 = short_press).
    const int press = (status & 0x0001) ? 1 : 0;

    // Bits 1..8 encode the button number (one-hot). Unknown mask = ignore.
    int button_idx = 0;
    if (!masked_to_button(static_cast<std::uint16_t>(status & 0x01FE), button_idx)) {
        return false;
    }

    Value v{};
    v.type = ValueType::StringRef;
    v.str  = kActions[button_idx][press];
    out.put("action", v);
    return true;
}

}  // namespace

extern const FzConverter kFzZunzunbeeSlateSwitch{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command) |
                         type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_slate_switch },
    .user_config       = nullptr,
};

}  // namespace zhc::zunzunbee
