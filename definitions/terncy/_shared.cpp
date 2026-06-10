// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/terncy/_shared.hpp"

#include <cmath>
#include <cstdint>

namespace zhc::terncy {

// ── kFzTerncyRaw ─────────────────────────────────────────────────────

extern const TerncyRawConfig kTerncyRawActionOnly{
    .emit_action = true, .emit_motion_occupancy = false,
};
extern const TerncyRawConfig kTerncyRawActionAndMotion{
    .emit_action = true, .emit_motion_occupancy = true,
};

namespace {

bool fz_terncy_raw(const DecodedMessage& msg,
                    const FzConverter& self,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // z2m's fzLocal.terncy_raw reads the WHOLE raw frame buffer
    // (msg.data, including the ZCL header). For the AduroSmart cluster
    // the header is manufacturer-specific = 5 bytes
    // [fc, mfg_lo, mfg_hi, tsn, cmd], so z2m's index→ZHC mapping is:
    //   z2m msg.data[4] (discriminator) == the command id  → msg.command_id
    //   z2m msg.data[6] (action value)  == raw_body[1]
    //   z2m msg.data[7] (motion side)   == raw_body[2]
    // ZHC strips the header into raw_body, so we must NOT re-index by
    // the full-frame offsets — that read past the body and the decoder
    // matched nothing on real (~3-byte body) frames.

    // Default config (action only) when no user_config is supplied.
    const auto* cfg = static_cast<const TerncyRawConfig*>(self.user_config);
    if (!cfg) cfg = &kTerncyRawActionOnly;

    const std::uint16_t discriminator = msg.command_id;

    if (discriminator == 0) {
        if (!cfg->emit_action) return false;
        if (msg.raw_body.size() < 2) return false;
        const std::uint8_t v = msg.raw_body[1];
        const char* label = nullptr;
        switch (v) {
            case 1: label = "single";    break;
            case 2: label = "double";    break;
            case 3: label = "triple";    break;
            case 4: label = "quadruple"; break;
            default: return false;
        }
        Value action{}; action.type = ValueType::StringRef;
        action.str = label;
        out.put("action", action);
        return true;
    }

    if (discriminator == 4) {
        if (!cfg->emit_motion_occupancy) return false;
        if (msg.raw_body.size() < 3) return false;
        const std::uint8_t v = msg.raw_body[2];
        const char* side = nullptr;
        switch (v) {
            case 5:  side = "right"; break;
            case 7:  side = "right"; break;
            case 40: side = "left";  break;
            case 56: side = "left";  break;
            default: return false;
        }
        // occupancy_with_timeout shape: emit boolean occupancy plus
        // the side label. The shadow layer / WebUI applies the
        // device's no-occupancy timeout from upstream config.
        Value occ{}; occ.type = ValueType::Bool; occ.b = true;
        out.put("occupancy", occ);

        Value sideRef{}; sideRef.type = ValueType::StringRef;
        sideRef.str = side;
        out.put("action_side", sideRef);
        out.put("side", sideRef);   // legacy alias retained by z2m
        return true;
    }

    return false;
}

}  // namespace

extern const FzConverter kFzTerncyRawAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificClusterAduroSmart",
    .type_mask         = type_bit(MessageType::Command) |
                         type_bit(MessageType::Raw),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // Device-originated raw frames carry the server→client direction bit
    // (z2m wire byte fc=0x0D has bit3 set), like the Orvibo scene remotes.
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_terncy_raw },
    .user_config       = &kTerncyRawActionOnly,
};

extern const FzConverter kFzTerncyRawActionMotion{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificClusterAduroSmart",
    .type_mask         = type_bit(MessageType::Command) |
                         type_bit(MessageType::Raw),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // Device-originated raw frames carry the server→client direction bit
    // (z2m wire byte fc=0x0D has bit3 set), like the Orvibo scene remotes.
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_terncy_raw },
    .user_config       = &kTerncyRawActionAndMotion,
};

// ── kFzTerncyKnob ────────────────────────────────────────────────────

namespace {

bool fz_terncy_knob(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Attribute 0x001b == 27 in decimal-string form (parser convention).
    const Value* v = msg.payload.find("27");
    if (!v) return false;

    std::int64_t raw = 0;
    if      (v->type == ValueType::Int)  raw = v->i;
    else if (v->type == ValueType::Uint) raw = static_cast<std::int64_t>(v->u);
    else return false;

    Value action{}; action.type = ValueType::StringRef;
    action.str = "rotate";
    out.put("action", action);

    Value dir{}; dir.type = ValueType::StringRef;
    dir.str = (raw > 0) ? "clockwise" : "counterclockwise";
    out.put("action_direction", dir);

    const std::int64_t magnitude = (raw < 0) ? -raw : raw;
    Value num{}; num.type = ValueType::Float;
    num.f = static_cast<float>(magnitude) / 12.0f;
    out.put("action_number", num);
    return true;
}

}  // namespace

extern const FzConverter kFzTerncyKnob{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificClusterAduroSmart",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_terncy_knob },
    .user_config       = nullptr,
};

// ── kFzTerncyContact ─────────────────────────────────────────────────

namespace {

bool fz_terncy_contact(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genBinaryInput presentValue is attr 0x0055 → "85".
    const Value* v = msg.payload.find("85");
    if (!v) return false;

    bool b;
    if      (v->type == ValueType::Bool) b = v->b;
    else if (v->type == ValueType::Uint) b = (v->u != 0);
    else if (v->type == ValueType::Int)  b = (v->i != 0);
    else return false;

    Value c{}; c.type = ValueType::Bool; c.b = b;
    out.put("contact", c);
    return true;
}

}  // namespace

extern const FzConverter kFzTerncyContact{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBinaryInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_terncy_contact },
    .user_config       = nullptr,
};

// ── kFzTerncyTempScale10 ─────────────────────────────────────────────

namespace {

bool fz_terncy_temp_scale10(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // msTemperatureMeasurement attr 0x0000 == "0".
    const Value* v = msg.payload.find("0");
    if (!v) return false;

    std::int64_t raw = 0;
    if      (v->type == ValueType::Int)  raw = v->i;
    else if (v->type == ValueType::Uint) raw = static_cast<std::int64_t>(v->u);
    else return false;

    Value t{}; t.type = ValueType::Float;
    t.f = static_cast<float>(raw) / 10.0f;
    out.put("temperature", t);
    return true;
}

}  // namespace

extern const FzConverter kFzTerncyTempScale10{
    .family            = FrameFamily::Zcl,
    .cluster           = "msTemperatureMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_terncy_temp_scale10 },
    .user_config       = nullptr,
};

}  // namespace zhc::terncy
