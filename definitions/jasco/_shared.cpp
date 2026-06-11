// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Jasco shared converters — see _shared.hpp for rationale.
//
// z2m-source: jasco.ts #43095 via converters/fromZigbee.ts
//             command_on_state / command_off_state.
#include "definitions/jasco/_shared.hpp"

namespace zhc::devices::jasco {
namespace {

// genOnOff commandOn (0x01) → state = true. Mirrors z2m
// fz.command_on_state, which returns {state: "ON"}; the embedded-zhc
// canonical `state` form is a Bool (kFzOnOff emits Bool), so we converge
// the command path onto the same key/type.
bool fz_cmd_on_state(const ::zhc::DecodedMessage&,
                     const ::zhc::FzConverter&,
                     const ::zhc::PreparedDefinition&,
                     ::zhc::RuntimeContext&,
                     ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::Bool;
    v.b    = true;
    out.put("state", v);
    return true;
}

// genOnOff commandOff (0x00) → state = false. Mirrors z2m
// fz.command_off_state ({state: "OFF"}).
bool fz_cmd_off_state(const ::zhc::DecodedMessage&,
                      const ::zhc::FzConverter&,
                      const ::zhc::PreparedDefinition&,
                      ::zhc::RuntimeContext&,
                      ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::Bool;
    v.b    = false;
    out.put("state", v);
    return true;
}

}  // namespace

const ::zhc::FzConverter kFzCommandOnState{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x01,   // ZCL genOnOff cmd On
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_on_state },
    .user_config       = nullptr,
};

const ::zhc::FzConverter kFzCommandOffState{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x00,   // ZCL genOnOff cmd Off
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_off_state },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::jasco
