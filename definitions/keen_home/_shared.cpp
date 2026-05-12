// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/keen_home/_shared.hpp"

#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::keen_home {
namespace {

// ZCL frame-control byte for a cluster-specific c→s command, default-
// response suppressed. Matches z2m's convention.
constexpr std::uint8_t kCmdFc = 0x11;

// 0..255  →  0..100 (round-half-up via integer math).
constexpr std::uint32_t level_to_pct(std::uint32_t lvl) {
    if (lvl > 255) lvl = 255;
    return (lvl * 100u + 127u) / 255u;
}

// 0..100  →  0..255.
constexpr std::uint32_t pct_to_level(std::uint32_t pct) {
    if (pct > 100) pct = 100;
    return (pct * 255u + 50u) / 100u;
}

bool fz_keen_cover_position(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genLevelCtrl currentLevel — attr 0x0000 → key "0".
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint32_t pct = level_to_pct(static_cast<std::uint32_t>(v->u));

    Value pos{}; pos.type = ValueType::Uint; pos.u = pct;
    out.put("position", pos);

    Value st{}; st.type = ValueType::StringRef;
    st.str = (pct > 0) ? "OPEN" : "CLOSE";
    out.put("state", st);
    return true;
}

bool tz_keen_cover_position(std::string_view key, const Value& input,
                             const TzConverter&,
                             const PreparedDefinition&, RuntimeContext&,
                             std::span<std::uint8_t> out_frame,
                             std::size_t& out_size) {
    out_size = 0;

    // Resolve to a 0..100 position number.
    std::uint32_t pct = 0;
    bool stop = false;

    if (key == "position") {
        if (input.type != ValueType::Uint || input.u > 100) return false;
        pct = static_cast<std::uint32_t>(input.u);
    } else if (key == "state") {
        if (input.type != ValueType::StringRef || !input.str) return false;
        if      (std::strcmp(input.str, "OPEN")  == 0) pct = 100;
        else if (std::strcmp(input.str, "CLOSE") == 0) pct = 0;
        else if (std::strcmp(input.str, "STOP")  == 0) stop = true;
        else return false;
    } else {
        return false;
    }

    if (stop) {
        // genLevelCtrl stop (cmd 0x03), no payload.
        if (out_frame.size() < 3) return false;
        out_frame[0] = kCmdFc;
        out_frame[1] = 0x00;     // TSN — adapter patches.
        out_frame[2] = 0x03;     // stop
        out_size = 3;
        return true;
    }

    // genLevelCtrl moveToLevelWithOnOff (cmd 0x04) — payload: level u8,
    // transtime u16 LE. 6 bytes total.
    if (out_frame.size() < 6) return false;
    out_frame[0] = kCmdFc;
    out_frame[1] = 0x00;     // TSN — adapter patches.
    out_frame[2] = 0x04;     // moveToLevelWithOnOff
    out_frame[3] = static_cast<std::uint8_t>(pct_to_level(pct));
    out_frame[4] = 0x00;     // transtime LSB
    out_frame[5] = 0x00;     // transtime MSB
    out_size = 6;
    return true;
}

}  // namespace

extern const FzConverter kFzKeenCoverPosition{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_keen_cover_position },
    .user_config       = nullptr,
};

extern const TzConverter kTzKeenCoverPosition{
    .key          = nullptr,   // claims "position" + "state"
    .cluster      = "genLevelCtrl",
    .cluster_id   = 0x0008,
    .command_id   = 0x04,      // moveToLevelWithOnOff (most common)
    .fn           = tz_keen_cover_position,
    .user_config  = nullptr,
};

}  // namespace zhc::devices::keen_home
