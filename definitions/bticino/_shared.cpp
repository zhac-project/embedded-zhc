// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/bticino/_shared.hpp"

#include <cstdint>

namespace zhc::devices::bticino {

// ── kFzK4003CBinaryInput ─────────────────────────────────────────────
//
// genBinaryInput presentValue is attr 0x0055 → decimal-string key "85".
// z2m: isOn = Boolean(presentValue);
//      return {action: isOn ? "on":"off", state: isOn ? "ON":"OFF"};

namespace {

bool fz_k4003c_binary_input(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");
    if (!v) return false;

    bool is_on;
    if      (v->type == ValueType::Bool) is_on = v->b;
    else if (v->type == ValueType::Uint) is_on = (v->u != 0);
    else if (v->type == ValueType::Int)  is_on = (v->i != 0);
    else return false;

    Value a{}; a.type = ValueType::StringRef; a.str = is_on ? "on" : "off";
    out.put("action", a);

    Value s{}; s.type = ValueType::Bool; s.b = is_on;
    out.put("state", s);
    return true;
}

}  // namespace

extern const FzConverter kFzK4003CBinaryInput{
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
    .fn                = { .zcl_fn = fz_k4003c_binary_input },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::bticino
