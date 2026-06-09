// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Easyiot converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/easyiot.ts
//             (fzLocal.easyiot_action).

#include "definitions/easyiot/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::easyiot {

namespace {

// z2m `easyiot_action` verb remap (note: NOT the standard on/off/toggle):
//   commandOff    (0x00) -> "long"
//   commandOn     (0x01) -> "double"
//   commandToggle (0x02) -> "single"
enum class Verb : std::uint8_t { Long = 0, Double, Single, Count };

// Indexed by [src_endpoint - 1][Verb]. 8 buttons x 3 verbs. Static
// rodata; the string pointers outlive any FixedPayload that borrows them
// via Value::StringRef. z2m always prefixes the button (buttonMapping
// covers ep1..ep8 for every WB0x model — there is no un-prefixed case),
// so we mirror that and always emit "<button>_<verb>".
constexpr const char* const kEpLabels[8][static_cast<std::size_t>(Verb::Count)] = {
    {"1_long", "1_double", "1_single"},
    {"2_long", "2_double", "2_single"},
    {"3_long", "3_double", "3_single"},
    {"4_long", "4_double", "4_single"},
    {"5_long", "5_double", "5_single"},
    {"6_long", "6_double", "6_single"},
    {"7_long", "7_double", "7_single"},
    {"8_long", "8_double", "8_single"},
};

bool fz_easyiot_action(const DecodedMessage& msg,
                       const FzConverter&,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Verb verb;
    switch (msg.command_id) {
        case 0x00: verb = Verb::Long;   break;  // commandOff
        case 0x01: verb = Verb::Double; break;  // commandOn
        case 0x02: verb = Verb::Single; break;  // commandToggle
        default: return false;
    }

    const std::uint8_t ep = msg.src_endpoint;
    if (ep < 1 || ep > 8) return false;

    Value a{};
    a.type = ValueType::StringRef;
    a.str  = kEpLabels[ep - 1][static_cast<std::size_t>(verb)];
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzEasyiotAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_easyiot_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::easyiot
