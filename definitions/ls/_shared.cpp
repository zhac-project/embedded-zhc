// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m-source: zigbee-herdsman-converters/src/devices/ls.ts

#include "definitions/ls/_shared.hpp"

namespace zhc::ls {

namespace {

bool fz_ls_command_on_double(const DecodedMessage&,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = "on_double";
    out.put("action", v);
    return true;
}

bool fz_ls_command_off_double(const DecodedMessage&,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = "off_double";
    out.put("action", v);
    return true;
}

}  // namespace

extern const FzConverter kFzLsCommandOnDouble{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x07,   // commandOnWithRecallGlobalScene
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ls_command_on_double },
    .user_config       = nullptr,
};

extern const FzConverter kFzLsCommandOffDouble{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x40,   // commandOffWithEffect
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ls_command_off_double },
    .user_config       = nullptr,
};

}  // namespace zhc::ls
