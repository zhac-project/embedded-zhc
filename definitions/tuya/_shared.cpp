// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Tuya converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts
//             (tuya.modernExtend, fz.tuya_*).

#include "definitions/tuya/_shared.hpp"
#include "definitions/_generic/_shared.hpp"  // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"

namespace zhc::tuya {

// Cluster-specific command ids on manuSpecificTuya (0xEF00).
namespace {
constexpr std::uint16_t kCmdMcuSyncTime = 0x24;
}  // namespace

bool fz_tuya_mcu_sync_time(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext& ctx,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    if (msg.command_id != kCmdMcuSyncTime) return false;
    if (ctx.tuya_sync_time) {
        ctx.tuya_sync_time(ctx.device_index, msg.transaction_sequence);
    }
    return true;
}

extern const FzConverter kFzTuyaMcuSyncTime{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = kCmdMcuSyncTime,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_tuya_mcu_sync_time },
    .user_config       = nullptr,
};

// ── fz_tuya_datapoints ─────────────────────────────────────────────

namespace {

const TuyaDpMapEntry* lookup_entry(const TuyaDatapointMap& map,
                                     std::uint8_t dp_id) {
    for (std::uint8_t i = 0; i < map.count; ++i) {
        if (map.entries[i].dp_id == dp_id) return &map.entries[i];
    }
    return nullptr;
}

bool emit_from_entry(const TuyaDpMapEntry& e,
                      const Value& raw,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    switch (e.type) {
        case TuyaDpType::Enum: {
            if (!e.enum_table || e.enum_count == 0) {
                out.put(e.out_key, raw);
                return true;
            }
            if (raw.type != ValueType::Uint) return false;
            for (std::uint8_t j = 0; j < e.enum_count; ++j) {
                if (e.enum_table[j].value == raw.u) {
                    Value v{}; v.type = ValueType::StringRef;
                    v.str = e.enum_table[j].label;
                    out.put(e.out_key, v);
                    return true;
                }
            }
            return false;  // unmapped enum value
        }
        case TuyaDpType::Numeric: {
            if (raw.type != ValueType::Int) return false;
            std::int64_t v_int = raw.i;
            if (e.flags & kTuyaDpFlagInvertPosition) {
                v_int = 100 - v_int;
            }
            // Float divisor takes precedence (e.g. ZVG1 fl-oz → L
            // via /33.814). Default 0.0 falls through to int path.
            if (e.divisor_f != 0.0f) {
                Value v{}; v.type = ValueType::Float;
                v.f = static_cast<float>(v_int) / e.divisor_f;
                out.put(e.out_key, v);
                return true;
            }
            const std::int32_t d = e.divisor == 0 ? 1 : e.divisor;
            if (d == 1) {
                Value v{}; v.type = ValueType::Int; v.i = v_int;
                out.put(e.out_key, v);
            } else if (d < 0) {
                Value v{}; v.type = ValueType::Int;
                v.i = v_int * static_cast<std::int64_t>(-d);
                out.put(e.out_key, v);
            } else {
                Value v{}; v.type = ValueType::Float;
                v.f = static_cast<float>(v_int) / static_cast<float>(d);
                out.put(e.out_key, v);
            }
            return true;
        }
        case TuyaDpType::Bool: {
            if (raw.type != ValueType::Bool) return false;
            Value v = raw;
            if (e.flags & kTuyaDpFlagInvertBool) v.b = !v.b;
            out.put(e.out_key, v);
            return true;
        }
        default:
            // Schedule day: 4 × [h, m, t*2]. Emit formatted string.
            if ((e.flags & kTuyaDpFlagScheduleDay) &&
                raw.type == ValueType::BytesRef &&
                raw.bytes.size() >= 12) {
                static char scratch[4][64];
                static std::uint8_t head = 0;
                char* dst = scratch[head];
                head = (head + 1) % 4;
                const auto* b = raw.bytes.data();
                std::snprintf(dst, sizeof(scratch[0]),
                    "%02u:%02u/%.1f %02u:%02u/%.1f "
                    "%02u:%02u/%.1f %02u:%02u/%.1f",
                    b[0], b[1], b[2] / 2.0,
                    b[3], b[4], b[5] / 2.0,
                    b[6], b[7], b[8] / 2.0,
                    b[9], b[10], b[11] / 2.0);
                Value v{}; v.type = ValueType::StringRef; v.str = dst;
                out.put(e.out_key, v);
                return true;
            }
            out.put(e.out_key, raw);
            return true;
    }
}

}  // namespace

bool fz_tuya_datapoints(std::span<const TuyaDpRecord> dps,
                         const DecodedMessage&,
                         const FzConverter& self,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = static_cast<const TuyaDatapointMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    bool emitted = false;
    for (const auto& rec : dps) {
        const auto* entry = lookup_entry(*map, rec.dp_id);
        if (!entry) continue;                     // silent-drop unknown DP

        Value val{};
        if (!decode_tuya_dp(rec, val)) continue;  // malformed — skip

        if (emit_from_entry(*entry, val, out)) emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzTuyaDatapoints{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = fz_tuya_datapoints },
    .user_config       = nullptr,
};

// ── Non-DP Tuya attribute converters ───────────────────────────────

namespace {

struct EnumAttrConfig {
    std::uint16_t        attr_id;
    const char*          out_key;
    const TuyaEnumEntry* table;
    std::uint8_t         count;
};

bool decode_enum_attr(const DecodedMessage& msg,
                      const EnumAttrConfig& cfg,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    char buf[8];
    int  n = 0;
    std::uint16_t v = cfg.attr_id;
    if (v == 0) buf[n++] = '0';
    else {
        char tmp[8]; int t = 0;
        while (v > 0 && t < 7) { tmp[t++] = '0' + (v % 10); v /= 10; }
        while (t > 0) buf[n++] = tmp[--t];
    }
    buf[n] = '\0';

    const Value* found = msg.payload.find(buf);
    if (!found) return false;

    std::uint64_t raw;
    if (found->type == ValueType::Uint)     raw = found->u;
    else if (found->type == ValueType::Int) raw = static_cast<std::uint64_t>(found->i);
    else return false;

    for (std::uint8_t i = 0; i < cfg.count; ++i) {
        if (cfg.table[i].value == raw) {
            Value o{}; o.type = ValueType::StringRef;
            o.str = cfg.table[i].label;
            out.put(cfg.out_key, o);
            return true;
        }
    }
    return false;
}

constexpr TuyaEnumEntry kPowerOnBehaviorTable[] = {
    {0, "off"}, {1, "on"}, {2, "toggle"}, {3, "previous"},
};
constexpr EnumAttrConfig kPowerOnBehaviorCfg{
    0x8002, "power_on_behavior", kPowerOnBehaviorTable, 4,
};

constexpr TuyaEnumEntry kIndicatorModeTable[] = {
    {0, "off"}, {1, "off_on"}, {2, "on_off"}, {3, "on"},
};
constexpr EnumAttrConfig kIndicatorModeCfg{
    0x8001, "indicator_mode", kIndicatorModeTable, 4,
};

constexpr TuyaEnumEntry kOperationModeTable[] = {
    {0, "command"}, {1, "event"},
};
constexpr EnumAttrConfig kOperationModeCfg{
    0x8004, "operation_mode", kOperationModeTable, 2,
};

}  // namespace

bool fz_tuya_power_on_behavior(const DecodedMessage& msg,
                                 const FzConverter&,
                                 const PreparedDefinition&,
                                 RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return decode_enum_attr(msg, kPowerOnBehaviorCfg, out);
}

bool fz_tuya_indicator_mode(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return decode_enum_attr(msg, kIndicatorModeCfg, out);
}

bool fz_tuya_operation_mode(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return decode_enum_attr(msg, kOperationModeCfg, out);
}

bool fz_tuya_on_off_action(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const char* label = nullptr;
    switch (msg.command_id) {
        case 0x00: label = "off";    break;
        case 0x01: label = "on";     break;
        case 0x02: label = "toggle"; break;
        default:   return false;
    }
    Value v{}; v.type = ValueType::StringRef; v.str = label;
    out.put("action", v);
    return true;
}

#define ZHC_TUYA_ATTR_CVT(var, cluster_str, fn_ref)                     \
    extern const FzConverter var{                                        \
        .family            = FrameFamily::Zcl,                           \
        .cluster           = cluster_str,                                \
        .type_mask         = type_bit(MessageType::AttributeReport) |    \
                             type_bit(MessageType::ReadResponse),        \
        .command_id        = WILDCARD_CMD_ID,                            \
        .attr_id           = WILDCARD_ATTR_ID,                           \
        .endpoint          = WILDCARD_ENDPOINT,                          \
        .frame_flags_mask  = 0,                                          \
        .frame_flags_value = 0,                                          \
        .direction         = Direction::ServerToClient,                  \
        .fn                = { .zcl_fn = fn_ref },                       \
        .user_config       = nullptr,                                    \
    }

ZHC_TUYA_ATTR_CVT(kFzTuyaPowerOnBehavior, "genOnOff", fz_tuya_power_on_behavior);
ZHC_TUYA_ATTR_CVT(kFzTuyaIndicatorMode,   "genOnOff", fz_tuya_indicator_mode);
ZHC_TUYA_ATTR_CVT(kFzTuyaOperationMode,   "genOnOff", fz_tuya_operation_mode);

extern const FzConverter kFzTuyaOnOffAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_tuya_on_off_action },
    .user_config       = nullptr,
};

bool fz_tuya_switch_scene(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.command_id != 0xFD) return false;
    // Skip ZCL header: 3 bytes (fc,tsn,cmd) + 2 for manu-code when set.
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 1) return false;
    const std::uint8_t value = msg.raw_data[hdr];

    Value a{}; a.type = ValueType::StringRef; a.str = "switch_scene";
    out.put("action", a);

    Value n{}; n.type = ValueType::Uint; n.u = value;
    out.put("action_scene", n);
    return true;
}

extern const FzConverter kFzTuyaSwitchScene{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0xFD,                 // tuyaAction
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_tuya_switch_scene },
    .user_config       = nullptr,
};

bool fz_tuya_multi_action(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // The cmd_id filter lives here (not in the selector) so one
    // converter handles both `tuyaAction` and `tuyaAction2` without
    // duplicating the descriptor.
    if (msg.command_id != 0xFD && msg.command_id != 0xFC) return false;

    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 1) return false;
    const std::uint8_t value = msg.raw_data[hdr];

    const char* label = nullptr;
    if (msg.command_id == 0xFD) {
        switch (value) {
            case 0: label = "single"; break;
            case 1: label = "double"; break;
            case 2: label = "hold";   break;
        }
    } else {  // 0xFC tuyaAction2
        switch (value) {
            case 0: label = "rotate_right"; break;
            case 1: label = "rotate_left";  break;
        }
    }
    if (!label) return false;   // unmapped value — fall through

    Value v{}; v.type = ValueType::StringRef; v.str = label;
    out.put("action", v);
    return true;
}

extern const FzConverter kFzTuyaMultiAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,   // filtered inside fn (0xFD + 0xFC)
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_tuya_multi_action },
    .user_config       = nullptr,
};

// ── kFzIgnoreTuyaSetTime ───────────────────────────────────────────

namespace {
bool fz_ignore_tuya_set_time(const DecodedMessage&,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    return true;
}
}  // namespace

extern const FzConverter kFzIgnoreTuyaSetTime{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x24,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ignore_tuya_set_time },
    .user_config       = nullptr,
};

#undef ZHC_TUYA_ATTR_CVT

// ── Tuya DP writer ────────────────────────────────────────────────

namespace {

const TuyaDpMapEntry* find_entry_by_key(const TuyaDatapointMap& map,
                                         std::string_view key) {
    for (std::uint8_t i = 0; i < map.count; ++i) {
        if (map.entries[i].out_key &&
            key == map.entries[i].out_key) {
            return &map.entries[i];
        }
    }
    return nullptr;
}

bool encode_bool(const TuyaDpMapEntry& e, const Value& in,
                  std::uint8_t out_val[1]) {
    bool b = false;
    if      (in.type == ValueType::Bool) b = in.b;
    else if (in.type == ValueType::Uint) b = in.u != 0;
    else if (in.type == ValueType::Int)  b = in.i != 0;
    else return false;
    if (e.flags & kTuyaDpFlagInvertBool) b = !b;
    out_val[0] = b ? 1 : 0;
    return true;
}

bool encode_numeric(const TuyaDpMapEntry& e, const Value& in,
                     std::uint8_t out_val[4]) {
    std::int64_t raw = 0;
    // Float divisor: input is Float → multiply by divisor_f and round.
    // Reverse of decode `raw / divisor_f`.
    if (e.divisor_f != 0.0f) {
        float in_f = 0.0f;
        if      (in.type == ValueType::Float) in_f = in.f;
        else if (in.type == ValueType::Int)   in_f = static_cast<float>(in.i);
        else if (in.type == ValueType::Uint)  in_f = static_cast<float>(in.u);
        else return false;
        raw = static_cast<std::int64_t>(in_f * e.divisor_f + 0.5f);
    } else {
        if      (in.type == ValueType::Int)   raw = in.i;
        else if (in.type == ValueType::Uint)  raw = static_cast<std::int64_t>(in.u);
        else if (in.type == ValueType::Float) raw = static_cast<std::int64_t>(in.f);
        else return false;
        const std::int32_t d = e.divisor == 0 ? 1 : e.divisor;
        if      (d > 1) raw = raw * d;                            // reverse fz divide
        else if (d < 0) raw = raw / static_cast<std::int64_t>(-d); // reverse fz multiply
    }
    const std::int32_t v = static_cast<std::int32_t>(raw);
    out_val[0] = static_cast<std::uint8_t>((v >> 24) & 0xFF);
    out_val[1] = static_cast<std::uint8_t>((v >> 16) & 0xFF);
    out_val[2] = static_cast<std::uint8_t>((v >>  8) & 0xFF);
    out_val[3] = static_cast<std::uint8_t>( v        & 0xFF);
    return true;
}

bool encode_enum(const TuyaDpMapEntry& e, const Value& in,
                  std::uint8_t out_val[1]) {
    if (in.type == ValueType::Uint) {
        out_val[0] = static_cast<std::uint8_t>(in.u);
        return true;
    }
    if (in.type == ValueType::Int) {
        out_val[0] = static_cast<std::uint8_t>(in.i);
        return true;
    }
    if (in.type == ValueType::StringRef && in.str && e.enum_table) {
        for (std::uint8_t i = 0; i < e.enum_count; ++i) {
            if (e.enum_table[i].label &&
                std::strcmp(e.enum_table[i].label, in.str) == 0) {
                out_val[0] = static_cast<std::uint8_t>(e.enum_table[i].value);
                return true;
            }
        }
    }
    return false;
}

}  // namespace

bool tz_tuya_datapoints(std::string_view key,
                         const Value& input,
                         const TzConverter& self,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         std::span<std::uint8_t> out,
                         std::size_t& out_size) {
    out_size = 0;
    const auto* map = static_cast<const TuyaDatapointMap*>(self.user_config);
    if (!map) return false;
    const TuyaDpMapEntry* entry = find_entry_by_key(*map, key);
    if (!entry) return false;

    std::uint8_t val[4];
    std::uint16_t val_len = 0;
    switch (entry->type) {
        case TuyaDpType::Bool:
            if (!encode_bool(*entry, input, val)) return false;
            val_len = 1;
            break;
        case TuyaDpType::Numeric:
            if (!encode_numeric(*entry, input, val)) return false;
            val_len = 4;
            break;
        case TuyaDpType::Enum:
            if (!encode_enum(*entry, input, val)) return false;
            val_len = 1;
            break;
        default:
            return false;   // String / Raw / Bitmap — not yet
    }

    // Total size: 3 ZCL header + 2 tuya seq + 1 dp_id + 1 dp_type +
    //             2 len_be + val_len.
    const std::size_t total = 3 + 2 + 1 + 1 + 2 + val_len;
    if (out.size() < total) return false;

    out[0] = 0x01;                               // fc: cluster-specific c→s
    out[1] = 0x00;                               // tsn — platform patches
    out[2] = 0x00;                               // cmd: setData
    out[3] = 0x00; out[4] = 0x01;                // tuya seq (big-endian, 1)
    out[5] = entry->dp_id;
    out[6] = static_cast<std::uint8_t>(entry->type);
    out[7] = 0x00;                               // len_be high
    out[8] = static_cast<std::uint8_t>(val_len); // len_be low
    for (std::uint16_t i = 0; i < val_len; ++i) out[9 + i] = val[i];

    out_size = total;
    return true;
}

// Prototype TzConverter — device cpps copy the shape and point
// `user_config` at the device's TuyaDatapointMap (same instance used
// by the fz side).
extern const TzConverter kTzTuyaDatapointsProto{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,                   // setData
    .fn          = &tz_tuya_datapoints,
    .user_config = nullptr,
};

// ── Canonical Tuya non-DP writes ────────────────────────────────
namespace {

constexpr ::zhc::generic::ZclWriteLookup kPowerOnLut[] = {
    {"off", 0}, {"on", 1}, {"toggle", 2}, {"previous", 3},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecPowerOnBehavior{
    "power_on_behavior", 0x8002, 0x30, 0, kPowerOnLut, 4,
};

constexpr ::zhc::generic::ZclWriteLookup kOpModeLut[] = {
    {"control_relay", 0}, {"decoupled", 1},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOperationMode{
    "operation_mode", 0x8004, 0x30, 0, kOpModeLut, 2,
};

constexpr ::zhc::generic::ZclWriteLookup kIndicatorLut[] = {
    {"off", 0}, {"on_when_off", 1}, {"on_when_on", 2},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecIndicatorMode{
    "indicator_mode", 0x8001, 0x30, 0, kIndicatorLut, 3,
};

constexpr ::zhc::generic::ZclWriteSpec kSpecChildLock{
    "child_lock", 0x8000, 0x10, 0, nullptr, 0,
};

}  // namespace

#define ZHC_TUYA_TZ(var, spec_ref, key_str)                           \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = "genOnOff",                                    \
        .cluster_id  = 0x0006,                                        \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    };
ZHC_TUYA_TZ(kTzTuyaPowerOnBehavior, kSpecPowerOnBehavior, "power_on_behavior")
ZHC_TUYA_TZ(kTzTuyaOperationMode,   kSpecOperationMode,   "operation_mode")
ZHC_TUYA_TZ(kTzTuyaIndicatorMode,   kSpecIndicatorMode,   "indicator_mode")
ZHC_TUYA_TZ(kTzTuyaChildLock,       kSpecChildLock,       "child_lock")
#undef ZHC_TUYA_TZ

}  // namespace zhc::tuya
