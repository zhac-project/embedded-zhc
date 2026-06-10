// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Securifi Almond Click button decoder. The button reports clicks on
// ssIasAce `commandArm`, reusing the `armmode` byte as a click-type
// code. Mirrors z2m `fz.almond_click`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/securifi.ts.
#include "definitions/securifi/_shared.hpp"

namespace zhc::securifi {

namespace {

// z2m lookup: {3:"single", 0:"double", 2:"long"}. armmode is the first
// byte of the ssIasAce commandArm body.
bool fz_almond_click(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.empty()) return false;
    const char* press = nullptr;
    switch (msg.raw_body[0]) {
        case 3: press = "single"; break;
        case 0: press = "double"; break;
        case 2: press = "long";   break;
        default: return false;   // z2m drops unknown arm modes
    }
    Value a{}; a.type = ValueType::StringRef; a.str = press;
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzAlmondClick{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasAce",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,            // commandArm
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_almond_click },
    .user_config       = nullptr,
};

}  // namespace zhc::securifi
