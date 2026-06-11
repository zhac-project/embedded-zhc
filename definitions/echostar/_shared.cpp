// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Echostar converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/echostar.ts
//             (fz.SAGE206612_state).

#include "definitions/echostar/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::echostar {

namespace {

// z2m `SAGE206612_state` verb map (the SAGE doorbell, two buttons):
//   commandOn  (0x01) -> "bell1"
//   commandOff (0x00) -> "bell2"
bool fz_sage_bell_action(const DecodedMessage& msg,
                         const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const char* label = nullptr;
    switch (msg.command_id) {
        case 0x01: label = "bell1"; break;  // commandOn
        case 0x00: label = "bell2"; break;  // commandOff
        default: return false;
    }

    Value a{};
    a.type = ValueType::StringRef;
    a.str  = label;
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzSageBellAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_sage_bell_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::echostar
