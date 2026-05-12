// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m-source: zigbee-herdsman-converters/src/devices/candeo.ts
//
// See _shared.hpp for slice rationale.

#include "definitions/candeo/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

#include <cstring>

namespace zhc::candeo {

namespace {

using ::zhc::DecodedMessage;
using ::zhc::FixedPayload;
using ::zhc::FzConverter;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::TzConverter;
using ::zhc::Value;
using ::zhc::ValueType;

// ── external_switch_type (genBasic 0x8803) decoder ────────────────────

bool fz_external_switch_type(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // 0x8803 = 34819 decimal.
    const Value* v = msg.payload.find("34819");
    if (!v || v->type != ValueType::Uint) return false;
    const char* label = nullptr;
    switch (v->u) {
        case 0: label = "momentary"; break;
        case 1: label = "toggle";    break;
        default: return false;
    }
    Value o{};
    o.type = ValueType::StringRef;
    o.str  = label;
    out.put("external_switch_type", o);
    return true;
}

// ── external_switch_type encoder (manu-specific WriteAttributes) ──────

bool tz_external_switch_type(std::string_view key,
                             const Value& input,
                             const TzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             std::span<std::uint8_t> out_frame,
                             std::size_t& out_size) {
    out_size = 0;
    if (key != "external_switch_type") return false;

    std::uint8_t val = 0xFF;
    if (input.type == ValueType::StringRef && input.str) {
        if (std::strcmp(input.str, "momentary") == 0)      val = 0;
        else if (std::strcmp(input.str, "toggle") == 0)    val = 1;
        else return false;
    } else if (input.type == ValueType::Uint) {
        if (input.u > 1) return false;
        val = static_cast<std::uint8_t>(input.u);
    } else {
        return false;
    }

    // Manu-specific WriteAttributes:
    //   FC = 0x14  (b0 cluster-specific=0, b2 manu-spec=1, b4 disable-default-resp=1)
    //   manufacturer_code u16 LE
    //   TSN  (filled by adapter)
    //   CmdID 0x02
    //   attr 0x8803 LE | type 0x30 (enum8) | value u8
    if (out_frame.size() < 10) return false;
    out_frame[0] = 0x14;
    out_frame[1] = static_cast<std::uint8_t>(kSwitchTypeMfgCode & 0xFF);
    out_frame[2] = static_cast<std::uint8_t>((kSwitchTypeMfgCode >> 8) & 0xFF);
    out_frame[3] = 0;       // TSN — adapter overwrites
    out_frame[4] = 0x02;    // WriteAttributes command id
    out_frame[5] = 0x03;    // 0x8803 LE
    out_frame[6] = 0x88;
    out_frame[7] = 0x30;    // enum8
    out_frame[8] = val;
    out_size = 9;
    return true;
}

// ── RD1P knob press (genOnOff cluster commands) ──────────────────────

bool fz_rd1p_knob_press(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const char* label = nullptr;
    switch (msg.command_id) {
        case 0x00: label = "double_pressed"; break;
        case 0x01: label = "pressed";        break;
        case 0x02: label = "held";           break;
        case 0x03: label = "released";       break;
        default:   return false;
    }
    Value o{};
    o.type = ValueType::StringRef;
    o.str  = label;
    out.put("action", o);
    return true;
}

// ── RD1P knob rotation (genLevelCtrl cluster commands) ──────────────

bool fz_rd1p_knob_rotation(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // direction byte (payload[0]) — 0 = up/right, 1 = down/left.
    // For "stop" commands there's no payload, so we fall back to the
    // generic stopped_rotating label.
    const char* label = nullptr;
    switch (msg.command_id) {
        case 0x05: {  // moveWithOnOff
            std::uint8_t dir = msg.raw_data.size() > 0 ? msg.raw_data[0] : 0;
            label = (dir == 1) ? "started_rotating_left" : "started_rotating_right";
            break;
        }
        case 0x06: {  // stepWithOnOff
            std::uint8_t dir = msg.raw_data.size() > 0 ? msg.raw_data[0] : 0;
            label = (dir == 1) ? "rotating_left" : "rotating_right";
            break;
        }
        case 0x03:    // stop
        case 0x07:    // stopWithOnOff
            label = "stopped_rotating";
            break;
        default:
            return false;
    }
    Value o{};
    o.type = ValueType::StringRef;
    o.str  = label;
    out.put("action", o);
    return true;
}

}  // namespace

extern const FzConverter kFzExternalSwitchType{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_external_switch_type },
    .user_config       = nullptr,
};

extern const TzConverter kTzExternalSwitchType{
    .key         = "external_switch_type",
    .cluster     = "genBasic",
    .cluster_id  = 0x0000,
    .command_id  = 0x02,        // WriteAttributes
    .fn          = &tz_external_switch_type,
    .user_config = nullptr,
};

extern const FzConverter kFzRd1pKnobPress{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = &fz_rd1p_knob_press },
    .user_config       = nullptr,
};

extern const FzConverter kFzRd1pKnobRotation{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = &fz_rd1p_knob_rotation },
    .user_config       = nullptr,
};

}  // namespace zhc::candeo
