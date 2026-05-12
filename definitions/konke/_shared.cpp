// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/konke/_shared.hpp"

#include <cstddef>
#include <cstdint>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::konke {

namespace {

// Emit `action = "<label>"`. Mirrors the helper in _generic/_shared.cpp;
// duplicated here to keep this module self-contained (the upstream copy
// lives in an anonymous namespace).
bool emit_action(const char* label,
                 ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::StringRef;
    v.str  = label;
    out.put("action", v);
    return true;
}

// ── 2AJZ4KPKEY multi-function button ────────────────────────────────
// z2m fz.konke_action: genOnOff attr 0x0000, value 128/129/130 →
// single/double/hold. Other values are dropped.
bool fz_konke_action(const ::zhc::DecodedMessage& msg,
                      const ::zhc::FzConverter&,
                      const ::zhc::PreparedDefinition&,
                      ::zhc::RuntimeContext&,
                      ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("0");  // attr 0x0000
    if (!v) return false;
    std::uint32_t raw;
    if (v->type == ::zhc::ValueType::Uint)      raw = v->u;
    else if (v->type == ::zhc::ValueType::Bool) raw = v->b ? 1u : 0u;
    else                                        return false;
    switch (raw) {
        case 128: return emit_action("single", out);
        case 129: return emit_action("double", out);
        case 130: return emit_action("hold",   out);
        default:  return false;
    }
}

// ── KK-TQ-J01W 4-key scene switch ───────────────────────────────────
// z2m fzLocal.command_recall_konke: genScenes commandRecall (cmd 0x05),
// scene id 241/242/243/244 → hexagon/square/triangle/circle.
// Frame body: u16 group (LE) + u8 sceneId.
bool fz_konke_scene_action(const ::zhc::DecodedMessage& msg,
                             const ::zhc::FzConverter&,
                             const ::zhc::PreparedDefinition&,
                             ::zhc::RuntimeContext&,
                             ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // ZCL cluster-specific header: [fc] [seq] [cmd] (3 B), or
    // [fc] [mc_lo] [mc_hi] [seq] [cmd] (5 B) when manufacturer-specific.
    const std::size_t off = msg.manufacturer_specific ? 5u : 3u;
    if (msg.raw_data.size() < off + 3) return false;
    const std::uint8_t scene = msg.raw_data[off + 2];
    switch (scene) {
        case 241: return emit_action("hexagon",  out);
        case 242: return emit_action("square",   out);
        case 243: return emit_action("triangle", out);
        case 244: return emit_action("circle",   out);
        default:  return false;
    }
}

}  // namespace

extern const ::zhc::FzConverter kFzKonkeAction{
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
    .fn                = { .zcl_fn = fz_konke_action },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzKonkeSceneAction{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x05,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_konke_scene_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::konke
