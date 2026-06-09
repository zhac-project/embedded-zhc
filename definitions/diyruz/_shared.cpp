// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// DIYRuZ shared converters. See `_shared.hpp` for the contract.
//
// z2m-source: zigbee-herdsman-converters/src/devices/diyruz.ts
#include "definitions/diyruz/_shared.hpp"

namespace zhc::devices::diyruz {

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

// genOnOff onOff is attr 0x0000; the runtime keys attribute ids as decimal
// strings, so 0x0000 → "0". z2m's fz.diyruz_contact returns
// `{contact: msg.data.onOff !== 0}` — the embedded def exposes `contact`
// as Binary, so we emit a bool (closed/true when onOff != 0).
bool fz_diyruz_contact(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // 0x0000 onOff
    if (!v) return false;
    bool closed = false;
    if (v->type == ValueType::Bool)      closed = v->b;
    else if (v->type == ValueType::Uint) closed = (v->u != 0);
    else if (v->type == ValueType::Int)  closed = (v->i != 0);
    else return false;
    Value o{};
    o.type = ValueType::Bool;
    o.b    = closed;
    out.put("contact", o);
    return true;
}

}  // namespace

extern const FzConverter kFzDiyruzContact{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_diyruz_contact },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::diyruz
