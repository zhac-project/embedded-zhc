// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: JetHome shared converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/jethome.ts
#include "definitions/jethome/_shared.hpp"

namespace zhc::devices::jethome {

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

// ── kFzJetHomeMultiStateAction ─────────────────────────────────────
//
// genMultistateInput.presentValue (attr 0x0055) → "action" string.
// z2m actionLookup maps {0,256,512,1024}→release, {1,257,513,1025}→single,
// {2,258,514,1026}→double, {3,259,515,1027}→triple, {4,260,516,1028}→hold.
// All four channel blocks share the same low-byte gesture mapping; the
// channel itself is carried by the source endpoint, which the dispatch
// layer turns into the `action_in1`/`_in2`/`_in3` suffix via endpoint_map.
// Unknown values fall through (no emit), mirroring z2m's getFromLookup
// raising on a missing key.
bool fz_jethome_multistate_action(const DecodedMessage& msg,
                                   const ::zhc::FzConverter&,
                                   const PreparedDefinition&,
                                   RuntimeContext&,
                                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // 0x0055 = 85.
    const Value* v = msg.payload.find("85");
    if (!v || v->type != ValueType::Uint) return false;
    const char* label = nullptr;
    switch (v->u & 0xFFu) {
        case 0: label = "release"; break;
        case 1: label = "single";  break;
        case 2: label = "double";  break;
        case 3: label = "triple";  break;
        case 4: label = "hold";    break;
        default: return false;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("action", o);
    return true;
}

}  // namespace

const ::zhc::FzConverter kFzJetHomeMultiStateAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateInput",
    .type_mask         = ::zhc::type_bit(MessageType::AttributeReport) |
                         ::zhc::type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_jethome_multistate_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::jethome
