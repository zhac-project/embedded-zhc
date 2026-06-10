// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Qmotion vendor converters. See `_shared.hpp` for reference.

#include "_shared.hpp"

namespace zhc::qmotion {

using ::zhc::FixedPayload;
using ::zhc::FzConverter;
using ::zhc::Direction;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::DecodedMessage;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;

// ── fz_identify_action (genIdentify 0x0003) ─────────────────────────
//
// z2m `fz.identify` emits a fixed `action: "identify"` on every
// genIdentify attributeReport / readResponse. No payload inspection.
bool fz_identify_action(const DecodedMessage&,
                         const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{};
    v.type = ValueType::StringRef;
    v.str  = "identify";
    out.put("action", v);
    return true;
}

extern const FzConverter kFzIdentifyAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genIdentify",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_identify_action },
    .user_config       = nullptr,
};

}  // namespace zhc::qmotion
