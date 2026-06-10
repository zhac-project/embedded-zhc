// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/profalux/_shared.hpp"

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::profalux {
namespace {

// ZCL frame-control for a cluster-specific c->s command, default-response
// suppressed (same convention as the iluminize / keen_home encoders).
constexpr std::uint8_t kCmdFc = 0x11;

// 0..255 -> 0..100 (round-half-up). Matches z2m mapNumberRange semantics.
constexpr std::uint32_t level_to_pct(std::uint32_t lvl) {
    if (lvl > 255) lvl = 255;
    return (lvl * 100u + 127u) / 255u;
}

// 0..100 -> 0..255.
constexpr std::uint32_t pct_to_level(std::uint32_t pct) {
    if (pct > 100) pct = 100;
    return (pct * 255u + 50u) / 100u;
}

// ── fz.cover_position_via_brightness ─────────────────────────────────
// genLevelCtrl currentLevel (0..255) -> position (0..100) + derived
// OPEN/CLOSE state.
bool fz_position_via_brightness(const ::zhc::DecodedMessage& msg,
                                const ::zhc::FzConverter&,
                                const ::zhc::PreparedDefinition&,
                                ::zhc::RuntimeContext&,
                                ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("0");  // attr 0x0000 currentLevel
    if (!v || v->type != ::zhc::ValueType::Uint) return false;
    const std::uint32_t pct = level_to_pct(static_cast<std::uint32_t>(v->u));

    ::zhc::Value pos{}; pos.type = ::zhc::ValueType::Uint; pos.u = pct;
    out.put("position", pos);

    ::zhc::Value st{}; st.type = ::zhc::ValueType::StringRef;
    st.str = (pct > 0) ? "OPEN" : "CLOSE";
    out.put("state", st);
    return true;
}

// ── fz.cover_state_via_onoff ─────────────────────────────────────────
// genOnOff onOff -> `state` "OPEN" (onOff==1) / "CLOSE".
bool fz_state_via_onoff(const ::zhc::DecodedMessage& msg,
                        const ::zhc::FzConverter&,
                        const ::zhc::PreparedDefinition&,
                        ::zhc::RuntimeContext&,
                        ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("0");  // attr 0x0000 onOff
    if (!v) return false;
    bool on;
    if (v->type == ::zhc::ValueType::Bool)      on = v->b;
    else if (v->type == ::zhc::ValueType::Uint) on = v->u != 0;
    else return false;

    ::zhc::Value st{}; st.type = ::zhc::ValueType::StringRef;
    st.str = on ? "OPEN" : "CLOSE";
    out.put("state", st);
    return true;
}

// ── tz.cover_via_brightness (numeric path) ───────────────────────────
// `position` (0..100) -> genLevelCtrl moveToLevelWithOnOff (cmd 0x04):
// level u8, transtime u16 LE.
bool tz_position_via_brightness(std::string_view key, const ::zhc::Value& input,
                                const ::zhc::TzConverter&,
                                const ::zhc::PreparedDefinition&, ::zhc::RuntimeContext&,
                                std::span<std::uint8_t> out_frame,
                                std::size_t& out_size) {
    out_size = 0;
    if (key != "position") return false;
    if (input.type != ::zhc::ValueType::Uint || input.u > 100) return false;

    if (out_frame.size() < 6) return false;
    out_frame[0] = kCmdFc;
    out_frame[1] = 0x00;   // TSN — adapter patches.
    out_frame[2] = 0x04;
    out_frame[3] = static_cast<std::uint8_t>(pct_to_level(static_cast<std::uint32_t>(input.u)));
    out_frame[4] = 0x00;   // transtime LSB
    out_frame[5] = 0x00;   // transtime MSB
    out_size = 6;
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzNsav061PositionViaBrightness{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_position_via_brightness },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzNsav061StateViaOnOff{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_state_via_onoff },
    .user_config       = nullptr,
};

extern const ::zhc::TzConverter kTzNsav061PositionViaBrightness{
    .key          = "position",
    .cluster      = "genLevelCtrl",
    .cluster_id   = 0x0008,
    .command_id   = 0x04,   // moveToLevelWithOnOff
    .fn           = tz_position_via_brightness,
    .user_config  = nullptr,
};

}  // namespace zhc::devices::profalux
