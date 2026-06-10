// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/multir/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::multir {

namespace {

// ── MIR-SO100 SOS button ────────────────────────────────────────────
// z2m fzLocal.MIRSO100: cluster ssIasZone, type "raw". The device sends a
// raw cluster-specific command whose first payload byte (z2m msg.data[3])
// encodes the press kind:
//     0   → single
//     1   → double
//     128 → hold
// Any other value is dropped (z2m switch has no default).
bool fz_multir_sos_action(const ::zhc::DecodedMessage& msg,
                          const ::zhc::FzConverter&,
                          const ::zhc::PreparedDefinition&,
                          ::zhc::RuntimeContext&,
                          ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // ZCL cluster-specific header: [fc] [seq] [cmd] (3 B), or
    // [fc] [mc_lo] [mc_hi] [seq] [cmd] (5 B) when manufacturer-specific.
    // z2m's msg.data[3] is the first payload byte = raw_data[hdr].
    const std::size_t hdr = msg.manufacturer_specific ? 5u : 3u;
    if (msg.raw_data.size() < hdr + 1) return false;
    const std::uint8_t code = msg.raw_data[hdr];

    const char* label = nullptr;
    switch (code) {
        case 0:   label = "single"; break;
        case 1:   label = "double"; break;
        case 128: label = "hold";   break;
        default:  return false;
    }
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::StringRef;
    v.str  = label;
    out.put("action", v);
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzMultirSosAction{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_multir_sos_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::multir
