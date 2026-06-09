// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Shared Siglis (zigfred) vendor converters.
// z2m-source: zigbee-herdsman-converters/src/devices/siglis.ts +
//             converters/fromZigbee.ts.

#include "definitions/siglis/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::siglis {
namespace {

// Static-storage action strings, indexed [button][type]. Emitting a
// pointer into this table keeps the `action` StringRef valid past the
// dispatch call (the FixedPayload only stores the char*).
//
// button: 0..3 → button_1..button_4   (z2m `buttonLookup`)
// type:   0..3 → release/single/double/hold (z2m `actionLookup`)
constexpr const char* kActionTable[4][4] = {
    {"button_1_release", "button_1_single", "button_1_double", "button_1_hold"},
    {"button_2_release", "button_2_single", "button_2_double", "button_2_hold"},
    {"button_3_release", "button_3_single", "button_3_double", "button_3_hold"},
    {"button_4_release", "button_4_single", "button_4_double", "button_4_hold"},
};

// commandSiglisZigfredButtonEvent body (post-ZCL-header):
//   button:u8 | type:u8 | duration:u16 LE.
// We read from `raw_body` (the decoder's post-header span) so we never
// have to re-derive the manufacturer-specific 5-byte header offset.
bool fz_zigfred_button_event(const DecodedMessage& msg, const FzConverter&,
                              const PreparedDefinition&, RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Guard: this command id (0x02) collides with genOnOff Toggle, which
    // is NOT manufacturer-specific. The siglis frame always is.
    if (!msg.manufacturer_specific) return false;
    if (msg.raw_body.size() < 2) return false;  // button + type minimum

    const std::uint8_t button = msg.raw_body[0];
    const std::uint8_t type   = msg.raw_body[1];
    if (button > 3 || type > 3) return false;    // unknown → publish nothing

    Value a{};
    a.type = ValueType::StringRef;
    a.str  = kActionTable[button][type];          // static storage
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzZigfredButtonEvent{
    .family            = FrameFamily::Zcl,
    // Cluster name is documentation-only here: the decoder leaves
    // `DecodedMessage::cluster` null (id→name mapping is a dispatch-layer
    // concern), so cluster_match() falls through to the command-id /
    // type / direction filters below. Kept for parity readability.
    .cluster           = "manuSpecificSiglisZigfred",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x02,   // siglisZigfredButtonEvent
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // Button events flow device (server) → coordinator (client).
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_zigfred_button_event },
    .user_config       = nullptr,
};

}  // namespace zhc::siglis
