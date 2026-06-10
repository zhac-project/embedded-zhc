// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo custom cluster converters.
//
// Livolo builds its TI0001-* devices on top of the standard
// genPowerCfg (0x0001) cluster, but uses raw cluster-specific
// commands to carry a tiny DP-style payload. Byte positions are
// magic numbers straight from z2m's `livolo_*_state` converters.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             `livolo_hygrometer_state`, `livolo_illuminance_state`.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::livolo {

// Livolo hygrometer: DP at byte 10, value at byte 13.
//   DP 0x0E (14) → temperature (°C, u8 raw)
//   DP 0x0C (12) → humidity    (%,  u8 raw)
bool fz_livolo_hygrometer(const ::zhc::DecodedMessage& msg,
                           const ::zhc::FzConverter&,
                           const ::zhc::PreparedDefinition&,
                           ::zhc::RuntimeContext&,
                           ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_data.size() < 14) return false;
    const std::uint8_t dp  = msg.raw_data[10];
    const std::uint8_t val = msg.raw_data[13];
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::Uint;
    v.u    = val;
    if (dp == 14)       { out.put("temperature", v); return true; }
    else if (dp == 12)  { out.put("humidity",    v); return true; }
    return false;
}

extern const ::zhc::FzConverter kFzLivoloHygrometer{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_livolo_hygrometer },
    .user_config       = nullptr,
};

// Livolo illuminance+noise: DP at byte 12, value at byte 13.
//   DP 0x0D (13) → illuminance (lx, u8 raw)
//   DP 0x0E (14) → noise_level (enum u8) + noise_detected (bool, >2)
bool fz_livolo_illuminance(const ::zhc::DecodedMessage& msg,
                            const ::zhc::FzConverter&,
                            const ::zhc::PreparedDefinition&,
                            ::zhc::RuntimeContext&,
                            ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_data.size() < 14) return false;
    const std::uint8_t dp  = msg.raw_data[12];
    const std::uint8_t val = msg.raw_data[13];
    ::zhc::Value v{};
    v.type = ::zhc::ValueType::Uint;
    v.u    = val;
    if (dp == 13) {
        out.put("illuminance", v);
        return true;
    }
    if (dp == 14) {
        ::zhc::Value det{};
        det.type = ::zhc::ValueType::Bool;
        det.b = val > 2;
        out.put("noise_detected", det);
        out.put("noise_level", v);
        return true;
    }
    return false;
}

extern const ::zhc::FzConverter kFzLivoloIlluminance{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_livolo_illuminance },
    .user_config       = nullptr,
};

// ── Magic-header switch/pir decoders ────────────────────────────────
//
// All these read the [0x7A 0xD1] header at data[0..1], gate on
// data[10] == 7, and pull the status byte from data[14]. Slight
// variants pull different bit-mask sets.

namespace {

bool livolo_header_ok(const ::zhc::DecodedMessage& msg) {
    return msg.raw_data.size() >= 15 &&
           msg.raw_data[0] == 0x7A &&
           msg.raw_data[1] == 0xD1 &&
           msg.raw_data[10] == 7;
}

void put_bool(::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
              const char* key, bool b) {
    ::zhc::Value v{}; v.type = ::zhc::ValueType::Bool; v.b = b;
    out.put(key, v);
}

bool fz_livolo_state_1g(const ::zhc::DecodedMessage& msg,
                          const ::zhc::FzConverter&,
                          const ::zhc::PreparedDefinition&,
                          ::zhc::RuntimeContext&,
                          ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_header_ok(msg)) return false;
    put_bool(out, "state", (msg.raw_data[14] & 0x01) != 0);
    return true;
}

bool fz_livolo_state_2g(const ::zhc::DecodedMessage& msg,
                          const ::zhc::FzConverter&,
                          const ::zhc::PreparedDefinition&,
                          ::zhc::RuntimeContext&,
                          ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_header_ok(msg)) return false;
    const std::uint8_t s = msg.raw_data[14];
    put_bool(out, "state_left",  (s & 0x01) != 0);
    put_bool(out, "state_right", (s & 0x02) != 0);
    return true;
}

bool fz_livolo_state_4g(const ::zhc::DecodedMessage& msg,
                          const ::zhc::FzConverter&,
                          const ::zhc::PreparedDefinition&,
                          ::zhc::RuntimeContext&,
                          ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_header_ok(msg)) return false;
    const std::uint8_t s = msg.raw_data[14];
    put_bool(out, "state_left",         (s & 0x01) != 0);
    put_bool(out, "state_right",        (s & 0x02) != 0);
    put_bool(out, "state_bottom_left",  (s & 0x04) != 0);
    put_bool(out, "state_bottom_right", (s & 0x08) != 0);
    return true;
}

bool fz_livolo_pir(const ::zhc::DecodedMessage& msg,
                    const ::zhc::FzConverter&,
                    const ::zhc::PreparedDefinition&,
                    ::zhc::RuntimeContext&,
                    ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_header_ok(msg)) return false;
    put_bool(out, "occupancy", (msg.raw_data[14] & 0x01) != 0);
    return true;
}

#define ZHC_LIVOLO_FZ_RAW(NAME, FN_NAME)                                       \
    extern const ::zhc::FzConverter NAME{                                      \
        .family            = ::zhc::FrameFamily::Zcl,                          \
        .cluster           = "genPowerCfg",                                    \
        .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |    \
                             ::zhc::type_bit(::zhc::MessageType::Raw),         \
        .command_id        = ::zhc::WILDCARD_CMD_ID,                           \
        .attr_id           = ::zhc::WILDCARD_ATTR_ID,                          \
        .endpoint          = ::zhc::WILDCARD_ENDPOINT,                         \
        .frame_flags_mask  = 0,                                                \
        .frame_flags_value = 0,                                                \
        .direction         = ::zhc::Direction::ServerToClient,                 \
        .fn                = { .zcl_fn = FN_NAME },                            \
        .user_config       = nullptr,                                          \
    }

}  // namespace

ZHC_LIVOLO_FZ_RAW(kFzLivoloNewSwitchState1Gang, fz_livolo_state_1g);
ZHC_LIVOLO_FZ_RAW(kFzLivoloNewSwitchState2Gang, fz_livolo_state_2g);
ZHC_LIVOLO_FZ_RAW(kFzLivoloNewSwitchState4Gang, fz_livolo_state_4g);
ZHC_LIVOLO_FZ_RAW(kFzLivoloPirState,            fz_livolo_pir);

#undef ZHC_LIVOLO_FZ_RAW

// ── Curtain switch / dimmer / cover decoders ────────────────────────
//
// z2m-source: fromZigbee.ts `livolo_curtain_switch_state`,
//             `livolo_dimmer_state`, `livolo_cover_state`.

namespace {

bool livolo_magic_header(const ::zhc::DecodedMessage& msg) {
    return msg.raw_data.size() >= 2 &&
           msg.raw_data[0] == 0x7A &&
           msg.raw_data[1] == 0xD1;
}

void put_str(::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
             const char* key, const char* s) {
    ::zhc::Value v{}; v.type = ::zhc::ValueType::StringRef; v.str = s;
    out.put(key, v);
}

void put_uint(::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
              const char* key, std::uint64_t u) {
    ::zhc::Value v{}; v.type = ::zhc::ValueType::Uint; v.u = u;
    out.put(key, v);
}

// Curtain switch: gate data[10] in {5,2}, status at data[14].
//   state_left  = (status == 1) ? ON : OFF
//   state_right = (status == 0) ? ON : OFF   (note: non-bitmask polarity)
bool fz_livolo_curtain(const ::zhc::DecodedMessage& msg,
                        const ::zhc::FzConverter&,
                        const ::zhc::PreparedDefinition&,
                        ::zhc::RuntimeContext&,
                        ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_magic_header(msg) || msg.raw_data.size() < 15) return false;
    if (msg.raw_data[10] != 5 && msg.raw_data[10] != 2) return false;
    const std::uint8_t s = msg.raw_data[14];
    put_str(out, "state_left",  s == 1 ? "ON" : "OFF");
    put_str(out, "state_right", s == 0 ? "ON" : "OFF");
    return true;
}

// Dimmer:
//   data[10]==7  -> state = data[14] & 1
//   data[10]==13 -> state = data[13] & 1
//   data[10]==5  -> brightness = map(data[14]*10, 0..1000 -> 0..255)
bool fz_livolo_dimmer(const ::zhc::DecodedMessage& msg,
                       const ::zhc::FzConverter&,
                       const ::zhc::PreparedDefinition&,
                       ::zhc::RuntimeContext&,
                       ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_magic_header(msg) || msg.raw_data.size() < 15) return false;
    const std::uint8_t dp = msg.raw_data[10];
    if (dp == 7) {
        put_str(out, "state", (msg.raw_data[14] & 0x01) ? "ON" : "OFF");
        return true;
    }
    if (dp == 13) {
        put_str(out, "state", (msg.raw_data[13] & 0x01) ? "ON" : "OFF");
        return true;
    }
    if (dp == 5) {
        // value = data[14] * 10 (0..1000) -> brightness 0..255.
        const std::uint32_t value = static_cast<std::uint32_t>(msg.raw_data[14]) * 10u;
        const std::uint32_t bri = value > 1000u ? 255u : (value * 255u) / 1000u;
        put_uint(out, "brightness", bri);
        return true;
    }
    return false;
}

// Cover (roller blind): dp at data[10], reportType at data[12].
//   dp 0x0c/0x0f + reportType 0x04 -> position = 100 - data[13],
//       state OPEN/CLOSE, moving = (dp == 0x0f)
//   dp 0x0c/0x0f + reportType 0x12 -> motor_speed = data[13]
//   dp 0x0c/0x0f + reportType 0x13 -> motor_direction (data[13] < 0x80 ? FORWARD : REVERSE)
bool fz_livolo_cover(const ::zhc::DecodedMessage& msg,
                      const ::zhc::FzConverter&,
                      const ::zhc::PreparedDefinition&,
                      ::zhc::RuntimeContext&,
                      ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!livolo_magic_header(msg) || msg.raw_data.size() < 14) return false;
    const std::uint8_t dp = msg.raw_data[10];
    if (dp != 0x0C && dp != 0x0F) return false;
    const std::uint8_t report_type = msg.raw_data[12];
    if (report_type == 0x04) {
        const int pos = 100 - static_cast<int>(msg.raw_data[13]);
        const std::uint8_t position =
            static_cast<std::uint8_t>(pos < 0 ? 0 : (pos > 100 ? 100 : pos));
        put_uint(out, "position", position);
        put_str(out, "state", position > 0 ? "OPEN" : "CLOSE");
        ::zhc::Value mv{}; mv.type = ::zhc::ValueType::Bool; mv.b = (dp == 0x0F);
        out.put("moving", mv);
        return true;
    }
    if (report_type == 0x12) {
        put_uint(out, "motor_speed", msg.raw_data[13]);
        return true;
    }
    if (report_type == 0x13) {
        put_str(out, "motor_direction",
                msg.raw_data[13] < 0x80 ? "FORWARD" : "REVERSE");
        return true;
    }
    return false;
}

}  // namespace

extern const ::zhc::FzConverter kFzLivoloCurtainState{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_livolo_curtain },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzLivoloDimmerState{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_livolo_dimmer },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzLivoloCoverState{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_livolo_cover },
    .user_config       = nullptr,
};

// ── State encoder (manuSpec writeAttribute mfg 0x1AD2) ──────────────

namespace {

struct LivoloGang {
    const char*    key;
    std::uint8_t   type_byte;   // ZCL "data type" byte (Livolo's gang selector)
    std::uint8_t   on_byte;     // first byte of the 8-byte payload when ON
};

constexpr LivoloGang kGangs[] = {
    {"state",              0x01, 0x01},   // single-gang default
    {"state_left",         0x01, 0x01},
    {"state_right",        0x02, 0x02},
    {"state_bottom_left",  0x04, 0x04},
    {"state_bottom_right", 0x88, 0x08},
};

bool livolo_coerce_on(const ::zhc::Value& in, bool& out) {
    if (in.type == ::zhc::ValueType::Bool) { out = in.b; return true; }
    if (in.type == ::zhc::ValueType::Uint) { out = (in.u != 0); return true; }
    if (in.type == ::zhc::ValueType::Int)  { out = (in.i != 0); return true; }
    if (in.type == ::zhc::ValueType::StringRef && in.str) {
        out = (std::strcmp(in.str, "ON") == 0 ||
               std::strcmp(in.str, "on") == 0 ||
               std::strcmp(in.str, "true") == 0 ||
               std::strcmp(in.str, "1") == 0);
        return true;
    }
    return false;
}

bool tz_livolo_state_fn(std::string_view key, const ::zhc::Value& input,
                         const ::zhc::TzConverter&,
                         const ::zhc::PreparedDefinition&,
                         ::zhc::RuntimeContext&,
                         std::span<std::uint8_t> out_frame,
                         std::size_t& out_size) {
    out_size = 0;
    const LivoloGang* g = nullptr;
    for (const auto& gg : kGangs) {
        if (key == gg.key) { g = &gg; break; }
    }
    if (!g) return false;

    bool on = false;
    if (!livolo_coerce_on(input, on)) return false;
    if (out_frame.size() < 16) return false;

    // FC: profile-wide (00) | mfg (1<<2) | direction=server→client (1<<3)
    //     | disable-default-resp (1<<4) | reservedBits=3 (3<<5) = 0x7C.
    out_frame[0]  = 0x7C;
    // mfg code 0x1AD2 little-endian
    out_frame[1]  = 0xD2;
    out_frame[2]  = 0x1A;
    // tsn fixed 0xE9 (z2m: transactionSequenceNumber=0xe9)
    out_frame[3]  = 0xE9;
    // cmd 0x02 = writeAttributes
    out_frame[4]  = 0x02;
    // attr id 0x0001 little-endian
    out_frame[5]  = 0x01;
    out_frame[6]  = 0x00;
    // gang selector type byte
    out_frame[7]  = g->type_byte;
    // 8-byte value: gang flag for ON, all zero for OFF
    out_frame[8]  = on ? g->on_byte : 0x00;
    out_frame[9]  = 0x00;
    out_frame[10] = 0x00;
    out_frame[11] = 0x00;
    out_frame[12] = 0x00;
    out_frame[13] = 0x00;
    out_frame[14] = 0x00;
    out_frame[15] = 0x00;
    out_size = 16;
    return true;
}

}  // namespace

extern const ::zhc::TzConverter kTzLivoloState{
    .key         = nullptr,           // multi-key dispatcher (matches all "state*")
    .cluster     = "genPowerCfg",
    .cluster_id  = 0x0001,
    .command_id  = 0x02,              // writeAttributes (informational)
    .fn          = tz_livolo_state_fn,
    .user_config = nullptr,
};

}  // namespace zhc::devices::livolo
