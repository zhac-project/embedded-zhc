// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: EasyAccess (Datek) door-lock converters.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             `easycode_action`, `lock`;
//             src/converters/toZigbee.ts `easycode_auto_relock`,
//             `lock_sound_volume`; src/lib/constants.ts `lockSoundVolume`.

#include <cstring>

#include "definitions/easyaccess/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyaccess {

namespace {

// z2m constants.lockSoundVolume = ["silent_mode","low_volume","high_volume"].
constexpr const char* kSoundVolumeLabels[] = {
    "silent_mode", "low_volume", "high_volume",
};

// ── easycode_action (RAW closuresDoorLock command) ──────────────────
//
// z2m reads the full frame buffer (msg.data); ez exposes the identical
// buffer as raw_data. Index [4] = primary event code, index [3] =
// secondary (source) code, exactly as z2m's `easycode_action`:
//   lookup = {13:"lock", 14:"zigbee_unlock", 3:"rfid_unlock",
//             0:"keypad_unlock"}
//   if data[4] in {lock, zigbee_unlock} -> action = data[4]
//   else                                -> action = lookup[data[3]]
const char* easycode_lookup(std::uint8_t code) {
    switch (code) {
        case 13: return "lock";
        case 14: return "zigbee_unlock";
        case 3:  return "rfid_unlock";
        case 0:  return "keypad_unlock";
        default: return nullptr;
    }
}

bool fz_easycode_action(const ::zhc::DecodedMessage& msg,
                         const ::zhc::FzConverter&,
                         const ::zhc::PreparedDefinition&,
                         ::zhc::RuntimeContext&,
                         ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_data.size() < 5) return false;
    const char* primary = easycode_lookup(msg.raw_data[4]);
    const char* action = nullptr;
    if (primary &&
        (std::strcmp(primary, "lock") == 0 ||
         std::strcmp(primary, "zigbee_unlock") == 0)) {
        action = primary;
    } else {
        action = easycode_lookup(msg.raw_data[3]);
    }
    if (!action) return false;
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::StringRef;
    v.str = action;
    out.put("action", v);
    return true;
}

// ── sound_volume attribute report (closuresDoorLock 0x0024 ENUM8) ────
bool fz_easycode_sound_volume(const ::zhc::DecodedMessage& msg,
                               const ::zhc::FzConverter&,
                               const ::zhc::PreparedDefinition&,
                               ::zhc::RuntimeContext&,
                               ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("36");  // attr 0x0024 soundVolume
    if (!v || v->type != ::zhc::ValueType::Uint) return false;
    if (v->u >= (sizeof(kSoundVolumeLabels) / sizeof(kSoundVolumeLabels[0])))
        return false;
    ::zhc::Value o{};
    o.type = ::zhc::ValueType::StringRef;
    o.str = kSoundVolumeLabels[v->u];
    out.put("sound_volume", o);
    return true;
}

// ── sound_volume writer (vendor labels) ─────────────────────────────
constexpr ::zhc::generic::ZclWriteLookup kSoundVolumeLut[] = {
    {"silent_mode", 0}, {"low_volume", 1}, {"high_volume", 2},
};
constexpr ::zhc::generic::ZclWriteSpec kSoundVolumeSpec{
    "sound_volume", 0x0024, 0x30, 0,
    kSoundVolumeLut,
    sizeof(kSoundVolumeLut) / sizeof(kSoundVolumeLut[0]),
    0,
};

// ── auto_relock writer: bool → autoRelockTime (0x0023) 1/0 ──────────
constexpr ::zhc::generic::ZclWriteSpec kAutoRelockSpec{
    "auto_relock", 0x0023, 0x23, 0,  // 0x23 = UINT32, z2m writes autoRelockTime
    nullptr, 0, 0,
};

}  // namespace

extern const ::zhc::FzConverter kFzEasycodeAction{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_easycode_action },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzEasycodeSoundVolume{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_easycode_sound_volume },
    .user_config       = nullptr,
};

extern const ::zhc::TzConverter kTzEasycodeSoundVolume{
    .key         = "sound_volume",
    .cluster     = "closuresDoorLock",
    .cluster_id  = 0x0101,
    .command_id  = 0x02,  // writeAttributes
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSoundVolumeSpec,
};

extern const ::zhc::TzConverter kTzEasycodeAutoRelock{
    .key         = "auto_relock",
    .cluster     = "closuresDoorLock",
    .cluster_id  = 0x0101,
    .command_id  = 0x02,  // writeAttributes
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kAutoRelockSpec,
};

}  // namespace zhc::devices::easyaccess
