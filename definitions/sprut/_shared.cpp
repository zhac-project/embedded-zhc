// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Sprut.device converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/sprut.ts
//
// Bed.box uses presentValue (0x0055) reads on genMultistateInput
// (0x0012) and genMultistateOutput (0x0013). Drivent extends
// closuresWindowCovering with manuSpec attrs 0x6605..0x660B and a
// set of cluster-specific commands (resetLimit/openLimit/closeLimit/
// resetBlock), all tagged with mfg code 26214 (0x6666).

#include "definitions/sprut/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

#include <cstdint>
#include <cstring>

namespace zhc::sprut {

namespace {

// Decimal-string keys for the attribute IDs the sprut Fz decoders care
// about (the foundation parser stores attr ids as decimal strings).
constexpr const char* kKey_PresentValue = "85";    // 0x0055
constexpr const char* kKey_LiftPosition = "8";     // 0x0008
constexpr const char* kKey_DriveState   = "26121"; // 0x6609 enum8
constexpr const char* kKey_BlockedJam   = "26122"; // 0x660A bool
constexpr const char* kKey_BlockedMany  = "26123"; // 0x660B bool
constexpr const char* kKey_Speed        = "26118"; // 0x6606 u8
constexpr const char* kKey_Calibrate    = "26117"; // 0x6605 bool
constexpr const char* kKey_Wifi         = "26112"; // 0x6600 bool

// Pull a Value's integer payload regardless of underlying type. Mirrors
// the eurotronic helper.
bool to_uint(const Value& in, std::uint32_t& out) {
    switch (in.type) {
        case ValueType::Int:   out = static_cast<std::uint32_t>(in.i); return true;
        case ValueType::Uint:  out = static_cast<std::uint32_t>(in.u); return true;
        case ValueType::Float: out = static_cast<std::uint32_t>(in.f); return true;
        case ValueType::Bool:  out = in.b ? 1u : 0u;                   return true;
        default: return false;
    }
}

// ── fz_sprut_multistate ────────────────────────────────────────────
//
// Emits "presentValue" for both genMultistateInput and -Output. The
// dispatch endpoint-suffix step rewrites this to "presentValue_<label>"
// (e.g. "presentValue_head_control") so the per-EP exposes line up with
// the names z2m uses for the same physical knob.
//
// presentValue is reported as either Uint (most attrs) or — per the
// ZCL spec — a single-precision float. We accept both.
bool fz_multistate_present(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find(kKey_PresentValue);
    if (!v) return false;
    Value o{};
    switch (v->type) {
        case ValueType::Uint:
            o.type = ValueType::Uint; o.u = v->u; break;
        case ValueType::Int:
            o.type = ValueType::Int;  o.i = v->i; break;
        case ValueType::Float:
            // Bed.box reports integer percentages, but spec allows float.
            o.type = ValueType::Int;
            o.i   = static_cast<std::int64_t>(v->f);
            break;
        default:
            return false;
    }
    out.put("presentValue", o);
    return true;
}

}  // namespace

extern const FzConverter kFzSprutMultistateInput{
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
    .fn                = { .zcl_fn = fz_multistate_present },
    .user_config       = nullptr,
};

extern const FzConverter kFzSprutMultistateOutput{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateOutput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_multistate_present },
    .user_config       = nullptr,
};

// ── fz_sprut_drivent_covering ──────────────────────────────────────
//
// One decoder for every closuresWindowCovering attr report from
// Drivent. Emits "position" for the standard 0x0008, plus the manuSpec
// readouts: "drive_state" enum (0/closing 1/opening 2/stopped),
// "blocked_jam" bool, "blocked_many" bool, "speed" u8, "calibrate"
// bool. The drive_state enum is mapped to a string here so the SPA
// gets the same labels z2m exposes.
namespace {

bool fz_drivent_covering(const DecodedMessage& msg,
                          const FzConverter&,
                          const PreparedDefinition&,
                          RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    if (const Value* v = msg.payload.find(kKey_LiftPosition)) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("position", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_DriveState)) {
        std::uint32_t raw = 0;
        if (to_uint(*v, raw)) {
            Value o{}; o.type = ValueType::StringRef;
            switch (raw) {
                case 0: o.str = "closing"; break;
                case 1: o.str = "opening"; break;
                case 2: o.str = "stopped"; break;
                default: o.str = "stopped"; break;
            }
            out.put("drive_state", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_BlockedJam)) {
        std::uint32_t raw = 0;
        if (to_uint(*v, raw)) {
            Value o{}; o.type = ValueType::StringRef;
            o.str = raw ? "ON" : "OFF";
            out.put("blocked_jam", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_BlockedMany)) {
        std::uint32_t raw = 0;
        if (to_uint(*v, raw)) {
            Value o{}; o.type = ValueType::StringRef;
            o.str = raw ? "ON" : "OFF";
            out.put("blocked_many", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_Speed)) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("speed", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_Calibrate)) {
        std::uint32_t raw = 0;
        if (to_uint(*v, raw)) {
            Value o{}; o.type = ValueType::StringRef;
            o.str = raw ? "ON" : "OFF";
            out.put("calibrate", o); emitted = true;
        }
    }
    return emitted;
}

bool fz_sprut_wifi(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (const Value* v = msg.payload.find(kKey_Wifi)) {
        std::uint32_t raw = 0;
        if (to_uint(*v, raw)) {
            Value o{}; o.type = ValueType::StringRef;
            o.str = raw ? "ON" : "OFF";
            out.put("wifi", o);
            return true;
        }
    }
    return false;
}

}  // namespace

extern const FzConverter kFzSprutDriventCovering{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_drivent_covering },
    .user_config       = nullptr,
};

extern const FzConverter kFzSprutWifi{
    .family            = FrameFamily::Zcl,
    .cluster           = nullptr,        // cluster 0x6600 has no canonical name
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_sprut_wifi },
    .user_config       = nullptr,
};

// ── tz_sprut_cluster_cmd ───────────────────────────────────────────
//
// Emits `fc | mfg_lo | mfg_hi | tsn | cmd_id` with no payload bytes.
// fc = 0x05 → bit 0 (cluster-specific) + bit 2 (manu-specific). Used
// by every named command from sprut.ts's `sendCommand` helper.
//
// `self.command_id` carries the cluster command id; `self.user_config`
// points to a SprutCommandSpec with the input key.
bool tz_sprut_cluster_cmd(std::string_view key,
                           const Value&,
                           const TzConverter& self,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           std::span<std::uint8_t> out_frame,
                           std::size_t& out_size) {
    out_size = 0;
    const auto* spec = static_cast<const SprutCommandSpec*>(self.user_config);
    if (!spec || !spec->key) return false;
    if (key != spec->key) return false;

    const std::size_t total = 1 + 2 + 1 + 1;  // fc + mfg + tsn + cmd
    if (out_frame.size() < total) return false;
    out_frame[0] = 0x05;
    out_frame[1] = static_cast<std::uint8_t>(kSprutMfgCode & 0xFF);
    out_frame[2] = static_cast<std::uint8_t>((kSprutMfgCode >> 8) & 0xFF);
    out_frame[3] = 0x00;                                            // tsn
    out_frame[4] = static_cast<std::uint8_t>(self.command_id & 0xFF);
    out_size = total;
    return true;
}

namespace {
constexpr SprutCommandSpec kSpec_ResetLimit { "reset_limit"  };
constexpr SprutCommandSpec kSpec_OpenLimit  { "open_limit"   };
constexpr SprutCommandSpec kSpec_CloseLimit { "close_limit"  };
constexpr SprutCommandSpec kSpec_ResetBlock { "reset_block"  };
}  // namespace

extern const TzConverter kTzSprutResetLimit{
    .key         = "reset_limit",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x0000,
    .fn          = tz_sprut_cluster_cmd,
    .user_config = &kSpec_ResetLimit,
};
extern const TzConverter kTzSprutOpenLimit{
    .key         = "open_limit",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x0001,
    .fn          = tz_sprut_cluster_cmd,
    .user_config = &kSpec_OpenLimit,
};
extern const TzConverter kTzSprutCloseLimit{
    .key         = "close_limit",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x0002,
    .fn          = tz_sprut_cluster_cmd,
    .user_config = &kSpec_CloseLimit,
};
extern const TzConverter kTzSprutResetBlock{
    .key         = "reset_block",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x0003,
    .fn          = tz_sprut_cluster_cmd,
    .user_config = &kSpec_ResetBlock,
};

// ── manuSpec writeAttributes (speed, calibrate, wifi) ──────────────
//
// All three route through the data-driven generic encoder with mfg=0x6666.

namespace {

constexpr ::zhc::generic::ZclWriteLookup kOnOffLookup[] = {
    { "OFF", 0 },
    { "ON",  1 },
};

constexpr ::zhc::generic::ZclWriteSpec kSpec_Speed{
    "speed",
    /*attr_id=*/0x6606,
    /*attr_type=*/0x20,                         // UINT8
    /*manufacturer_code=*/kSprutMfgCode,
    nullptr, 0,
};

constexpr ::zhc::generic::ZclWriteSpec kSpec_Calibrate{
    "calibrate",
    /*attr_id=*/0x6605,
    /*attr_type=*/0x10,                         // BOOLEAN
    /*manufacturer_code=*/kSprutMfgCode,
    kOnOffLookup,
    static_cast<std::uint8_t>(sizeof(kOnOffLookup) / sizeof(kOnOffLookup[0])),
};

constexpr ::zhc::generic::ZclWriteSpec kSpec_Wifi{
    "wifi",
    /*attr_id=*/0x6600,
    /*attr_type=*/0x10,                         // BOOLEAN
    /*manufacturer_code=*/kSprutMfgCode,
    kOnOffLookup,
    static_cast<std::uint8_t>(sizeof(kOnOffLookup) / sizeof(kOnOffLookup[0])),
};

}  // namespace

extern const TzConverter kTzSprutSpeed{
    .key         = "speed",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x02,                        // writeAttributes
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Speed,
};
extern const TzConverter kTzSprutCalibrate{
    .key         = "calibrate",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x02,
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Calibrate,
};
extern const TzConverter kTzSprutWifi{
    .key         = "wifi",
    .cluster     = nullptr,                     // cluster 0x6600 (no name)
    .cluster_id  = 0x6600,
    .command_id  = 0x02,
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Wifi,
};

}  // namespace zhc::sprut
