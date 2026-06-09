// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// PushOk Hardware shared converters. See `_shared.hpp` for the contract.
//
// z2m-source: zigbee-herdsman-converters/src/devices/pushok.ts
#include "definitions/pushok/_shared.hpp"

namespace zhc::devices::pushok {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;

namespace {

// genBinaryInput presentValue is attr 0x0055; the runtime keys attribute
// ids as decimal strings, so 0x55 → "85". z2m maps presentValue 0x01 →
// "ON" (contact closed = true) and 0x00 → "OFF" (open = false); the
// embedded def exposes `contact` as Binary, so we emit a bool.
bool fz_pushok_contact(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");   // 0x0055 presentValue
    if (!v) return false;
    bool closed = false;
    if (v->type == ValueType::Uint)      closed = (v->u != 0);
    else if (v->type == ValueType::Int)  closed = (v->i != 0);
    else if (v->type == ValueType::Bool) closed = v->b;
    else return false;
    Value o{};
    o.type = ValueType::Bool;
    o.b    = closed;
    out.put("contact", o);
    return true;
}

}  // namespace

extern const FzConverter kFzPushokContact{
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
    .fn                = { .zcl_fn = fz_pushok_contact },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::pushok
