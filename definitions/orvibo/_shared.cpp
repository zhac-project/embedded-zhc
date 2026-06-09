// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/orvibo/_shared.hpp"

#include "definitions/_generic/_shared.hpp"

namespace zhc::orvibo {
namespace {

// 0xFF00 attr 0x0001 uint8: 1 = off, 2 = previous (no `on`).
//
// z2m-source: orvibo.ts `orviboSwitchPowerOnBehavior` —
//   const powerOnLookup2: {[k: string]: number} = {off: 1, previous: 2};
//
// herdsman writes this without a manuCode because the cluster itself
// is already vendor-private (declared via deviceAddCustomCluster
// without a manufacturerCode field).
constexpr ::zhc::generic::ZclWriteLookup kPowerOnBehaviorLookup[] = {
    {"off",      1},
    {"previous", 2},
};

constexpr ::zhc::generic::ZclWriteSpec kSpecPowerOnBehavior{
    "power_on_behavior",
    /* attr_id           = */ 0x0001,
    /* attr_type         = */ 0x20,            // ZCL uint8
    /* manufacturer_code = */ 0x0000,
    /* lookup            = */ kPowerOnBehaviorLookup,
    /* lookup_count      = */ sizeof(kPowerOnBehaviorLookup) /
                              sizeof(kPowerOnBehaviorLookup[0]),
};

constexpr const char* kPowerOnBehaviorEnum[] = {"off", "previous"};

// ── Scene-remote raw-action decode ──────────────────────────────────
//
// Static "button_<n>_<verb>" literal table. Rows are button index 1..7,
// columns are the verb (click / hold / release). Indexing avoids any
// per-dispatch string allocation; the StringRef points at static data
// that outlives the DispatchResult.
constexpr const char* kOrviboActionTable[7][3] = {
    {"button_1_click", "button_1_hold", "button_1_release"},
    {"button_2_click", "button_2_hold", "button_2_release"},
    {"button_3_click", "button_3_hold", "button_3_release"},
    {"button_4_click", "button_4_hold", "button_4_release"},
    {"button_5_click", "button_5_hold", "button_5_release"},
    {"button_6_click", "button_6_hold", "button_6_release"},
    {"button_7_click", "button_7_hold", "button_7_release"},
};

// Action byte → column index in kOrviboActionTable. -1 = unrecognised.
int orvibo_action_column(std::uint8_t action_byte) {
    switch (action_byte) {
        case 0: return 0;   // click
        case 2: return 1;   // hold
        case 3: return 2;   // release
        default: return -1;
    }
}

// Shared decode. `button_index` is the already-resolved 1-based button
// number (0 = unrecognised). z2m's documented wire places the button
// byte at raw_body[0] and the action byte at raw_body[2] (post 3-byte
// ZCL header). Emits `action` = "button_<n>_<verb>"; returns false on
// any unrecognised byte so a non-orvibo raw frame is left unclaimed.
bool orvibo_raw_decode(const ::zhc::DecodedMessage& msg, std::uint8_t button_index,
                       ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (button_index < 1 || button_index > 7) return false;
    if (msg.raw_body.size() < 3) return false;
    const int col = orvibo_action_column(msg.raw_body[2]);
    if (col < 0) return false;

    ::zhc::Value a{};
    a.type = ::zhc::ValueType::StringRef;
    a.str = kOrviboActionTable[button_index - 1][col];
    out.put("action", a);
    return true;
}

// orvibo_raw_1 button lookup: {3→1, 11→2, 7→3, 15→4}.
std::uint8_t orvibo_raw1_button(std::uint8_t b) {
    switch (b) {
        case 3:  return 1;
        case 11: return 2;
        case 7:  return 3;
        case 15: return 4;
        default: return 0;
    }
}

bool fz_orvibo_raw1(const ::zhc::DecodedMessage& msg, const ::zhc::FzConverter&,
                    const ::zhc::PreparedDefinition&, ::zhc::RuntimeContext&,
                    ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.empty()) return false;
    return orvibo_raw_decode(msg, orvibo_raw1_button(msg.raw_body[0]), out);
}

// orvibo_raw_2 button lookup is the identity {1→1 … 7→7}.
bool fz_orvibo_raw2(const ::zhc::DecodedMessage& msg, const ::zhc::FzConverter&,
                    const ::zhc::PreparedDefinition&, ::zhc::RuntimeContext&,
                    ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.empty()) return false;
    const std::uint8_t b = msg.raw_body[0];
    return orvibo_raw_decode(msg, (b >= 1 && b <= 7) ? b : 0, out);
}

}  // namespace

extern const ::zhc::Expose kExposePowerOnBehaviorOrvibo{
    /* name        = */ "power_on_behavior",
    /* type        = */ ::zhc::ExposeType::Enum,
    /* access      = */ ::zhc::Access::StateSet,
    /* unit        = */ nullptr,
    /* description = */ nullptr,
    /* enum_values = */ kPowerOnBehaviorEnum,
    /* enum_count  = */ 2,
    /* category    = */ ::zhc::ExposeCategory::Config,
};

extern const ::zhc::TzConverter kTzOrviboPowerOnBehavior{
    /* key         = */ "power_on_behavior",
    /* cluster     = */ "manuSpecificOrvibo2",
    /* cluster_id  = */ 0xFF00,
    /* command_id  = */ 0x02,                  // writeAttributes
    /* fn          = */ &::zhc::generic::tz_zcl_write_attr,
    /* user_config = */ &kSpecPowerOnBehavior,
};

extern const ::zhc::FzConverter kFzOrviboRaw1Action{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "manuSpecificOrvibo",   // id 0x0017 (informational; msg.cluster is id-routed)
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_orvibo_raw1 },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzOrviboRaw2Action{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "manuSpecificOrvibo",   // id 0x0017 (informational; msg.cluster is id-routed)
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_orvibo_raw2 },
    .user_config       = nullptr,
};

}  // namespace zhc::orvibo
