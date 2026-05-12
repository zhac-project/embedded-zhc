// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m-source: zigbee-herdsman-converters/src/devices/cigol.ts

#include "definitions/cigol/_shared.hpp"

#include <cstdint>
#include <cstring>

namespace zhc::cigol {

namespace {

// ── fz_cigol_input ─────────────────────────────────────────────────
//
// genMultistateInput presentValue (attr 0x0055 = 85). The decoder
// stores the attribute under decimal-string key "85". z2m maps
// 0/1/2/4 → off/single/double/hold; everything else falls back to
// "off" (matches `getFromLookup(value, lookup, "off")`).
bool fz_cigol_input(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");
    if (!v || v->type != ValueType::Uint) return false;

    const char* label = "off";
    switch (v->u) {
        case 0: label = "off";    break;
        case 1: label = "single"; break;
        case 2: label = "double"; break;
        case 4: label = "hold";   break;
        default: label = "off";   break;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("input", o);
    return true;
}

// ── fz_cigol_switch_action_report ──────────────────────────────────
//
// genOnOffSwitchCfg switchActions (attr 0x0010 = 16). z2m lookup
// `{0:"on", 1:"off", 2:"toggle"}` with default "off". Emits key
// `switch_action` (suffixed `_<ep>` by dispatch).
bool fz_cigol_switch_action_report(const DecodedMessage& msg,
                                    const FzConverter&,
                                    const PreparedDefinition&,
                                    RuntimeContext&,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("16");
    if (!v || v->type != ValueType::Uint) return false;

    const char* label = "off";
    switch (v->u) {
        case 0: label = "on";     break;
        case 1: label = "off";    break;
        case 2: label = "toggle"; break;
        default: label = "off";   break;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("switch_action", o);
    return true;
}

// ── tz_cigol_switch_action ─────────────────────────────────────────
//
// Writes `switchActions` (genOnOffSwitchCfg cluster 0x0007, attr
// 0x0010, ENUM8). z2m uses `{on:0, off:1, toggle:2}` and lower-cases
// the input. Frame is the standard profile-wide writeAttributes:
//   [0] FC=0x10 (profile-wide + disable default response)
//   [1] TSN placeholder
//   [2] cmd 0x02 (writeAttributes)
//   [3..4] attr-id 0x0010 LE
//   [5] data type 0x30 (ENUM8)
//   [6] value byte
bool tz_cigol_switch_action(std::string_view key,
                             const Value& input,
                             const TzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             std::span<std::uint8_t> out_frame,
                             std::size_t& out_size) {
    out_size = 0;
    if (key != "switch_action") return false;
    if (input.type != ValueType::StringRef || !input.str) return false;

    auto ieq = [](const char* s, const char* target) {
        while (*s && *target) {
            char a = *s++;
            char b = *target++;
            if (a >= 'A' && a <= 'Z') a = static_cast<char>(a + ('a' - 'A'));
            if (b >= 'A' && b <= 'Z') b = static_cast<char>(b + ('a' - 'A'));
            if (a != b) return false;
        }
        return *s == 0 && *target == 0;
    };

    std::uint8_t val;
    if      (ieq(input.str, "on"))     val = 0;
    else if (ieq(input.str, "off"))    val = 1;
    else if (ieq(input.str, "toggle")) val = 2;
    else return false;

    if (out_frame.size() < 7) return false;
    out_frame[0] = 0x10;     // FC: profile-wide + disable default response
    out_frame[1] = 0x00;     // TSN — adapter patches
    out_frame[2] = 0x02;     // writeAttributes
    out_frame[3] = 0x10;     // attr 0x0010 LE
    out_frame[4] = 0x00;
    out_frame[5] = 0x30;     // data type ENUM8
    out_frame[6] = val;
    out_size = 7;
    return true;
}

}  // namespace

extern const FzConverter kFzCigolInput{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_cigol_input },
    .user_config       = nullptr,
};

extern const FzConverter kFzCigolSwitchActionReport{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOffSwitchCfg",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_cigol_switch_action_report },
    .user_config       = nullptr,
};

extern const TzConverter kTzCigolSwitchAction{
    .key         = "switch_action",
    .cluster     = "genOnOffSwitchCfg",
    .cluster_id  = 0x0007,
    .command_id  = 0x02,    // writeAttributes
    .fn          = tz_cigol_switch_action,
    .user_config = nullptr,
};

}  // namespace zhc::cigol
