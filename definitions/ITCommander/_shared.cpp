// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared IT Commander vendor converters.
//
// `fz.itcmdr_clicks` (genMultistateInput 0x0012, PresentValue attr
// 0x0055) → `action` string. The auto-port mis-wired the ITCMDR_Click
// button as a controllable genOnOff switch (`kFzOnOff` + `kTzOnOff` +
// a `state` StateSet expose), so every button press produced a dead
// `state` toggle and the real multi-click actions were dropped. This
// converter mirrors z2m's presentValue → action lookup exactly.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             (itcmdr_clicks).
#include "definitions/ITCommander/_shared.hpp"

namespace zhc::ITCommander {

namespace {

// z2m lookup: {0:"hold", 1:"single", 2:"double", 3:"triple",
//              4:"quadruple", 255:"release"}; default → "many".
const char* clicks_to_action(std::uint64_t clicks) {
    switch (clicks) {
        case 0:   return "hold";
        case 1:   return "single";
        case 2:   return "double";
        case 3:   return "triple";
        case 4:   return "quadruple";
        case 255: return "release";
        default:  return "many";
    }
}

bool fz_itcmdr_clicks(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0055 — PresentValue (decimal key "85").
    const Value* v = msg.payload.find("85");
    if (v == nullptr) return false;
    std::uint64_t clicks;
    if      (v->type == ValueType::Uint) clicks = v->u;
    else if (v->type == ValueType::Int)  clicks = static_cast<std::uint64_t>(v->i);
    else return false;
    Value o{};
    o.type = ValueType::StringRef;
    o.str  = clicks_to_action(clicks);
    out.put("action", o);
    return true;
}

}  // namespace

extern const FzConverter kFzItcmdrClicks{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_itcmdr_clicks },
    .user_config       = nullptr,
};

}  // namespace zhc::ITCommander
