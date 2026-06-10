// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Shared J.XUAN vendor converters.
// z2m-source: zigbee-herdsman-converters/src/devices/jxuan.ts +
//             converters/fromZigbee.ts.

#include "definitions/jxuan/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::jxuan {
namespace {

// z2m clickMapping {0:release, 1:single, 2:double, 3:hold} over the
// value of attribute id 1 on the manuSpecificJxuan (0xFE05) cluster.
const char* wsz01_action_label(std::uint64_t code) {
    switch (code) {
        case 0: return "release";
        case 1: return "single";
        case 2: return "double";
        case 3: return "hold";
        default: return nullptr;   // unknown code → publish nothing
    }
}

bool fz_wsz01_action(const DecodedMessage& msg, const FzConverter&,
                      const PreparedDefinition&, RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attributeReport on 0xFE05: the decoder keys the (unknown) attr id 1
    // by its decimal form "1", mirroring z2m's `msg.data["1"]`.
    const Value* v = msg.payload.find("1");
    if (!v || v->type != ValueType::Uint) return false;

    const char* label = wsz01_action_label(v->u);
    if (!label) return false;

    Value a{};
    a.type = ValueType::StringRef;
    a.str  = label;   // static-storage literal — outlives dispatch
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzWsz01Action{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificJxuan",
    .type_mask         = type_bit(MessageType::AttributeReport),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_wsz01_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::jxuan
