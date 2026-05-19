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

bool emit_from_entry(const TuyaDpMapEntry& e,
                      const Value& raw,
                      RuntimeContext& ctx,
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
            // Schedule day: 4 × [h, m, t*2]. Some devices reuse the same DP
            // for schedule frames and a short non-schedule value (e.g. TRV26
            // DP17 carries either open_window_time or schedule_monday). Use
            // content shape to disambiguate: a schedule-day entry MUST see a
            // 12+ byte BytesRef payload, otherwise it has to abstain so the
            // sibling non-schedule entry on the same DP can claim the value.
            if (e.flags & kTuyaDpFlagScheduleDay) {
                if (raw.type != ValueType::BytesRef ||
                    raw.bytes.size() < 12) {
                    return false;
                }
                // Per-dispatch arena: a 4-period TRV schedule arriving
                // as four back-to-back DP frames previously overwrote
                // earlier slots in a 4-entry static ring. Per-dispatch
                // lifetime is the right shape — the StringRef only
                // needs to outlive the DispatchResult merge.
                char tmp[64];
                const auto* b = raw.bytes.data();
                const int n = std::snprintf(tmp, sizeof(tmp),
                    "%02u:%02u/%.1f %02u:%02u/%.1f "
                    "%02u:%02u/%.1f %02u:%02u/%.1f",
                    b[0], b[1], b[2] / 2.0,
                    b[3], b[4], b[5] / 2.0,
                    b[6], b[7], b[8] / 2.0,
                    b[9], b[10], b[11] / 2.0);
                if (n <= 0) return false;
                const std::size_t slen =
                    static_cast<std::size_t>(n) >= sizeof(tmp)
                        ? sizeof(tmp) - 1
                        : static_cast<std::size_t>(n);
                const char* s = ctx.alloc_str(tmp, slen);
                if (!s) return false;
                Value v{}; v.type = ValueType::StringRef; v.str = s;
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
                         RuntimeContext& ctx,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = static_cast<const TuyaDatapointMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    bool emitted = false;
    for (const auto& rec : dps) {
        Value val{};
        bool decoded = false;
        // Multiple entries may share a dp_id (intentional fan-out, e.g.
        // TRV60 DP36 = frost_protection + scale_protection, or content-
        // disjoint pairs like TRV26 DP17 = open_window_time + schedule_monday).
        // Decode the wire payload once, then fire every matching entry —
        // emit_from_entry will return false for entries whose decoder
        // doesn't match the payload shape so they abstain.
        for (std::uint8_t i = 0; i < map->count; ++i) {
            const auto& entry = map->entries[i];
            if (entry.dp_id != rec.dp_id) continue;
            if (!decoded) {
                if (!decode_tuya_dp(rec, val)) break;  // malformed — skip DP
                decoded = true;
            }
            if (emit_from_entry(entry, val, ctx, out)) emitted = true;
        }
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
    // Match foundation.cpp's `format_decimal_key` shape exactly — if
    // the parser ever changes how it stringifies unknown attr ids,
    // both sites move together. 5 digits + NUL fits a u16 worst case.
    char buf[8];
    const int n = std::snprintf(buf, sizeof(buf), "%u",
                                  static_cast<unsigned>(cfg.attr_id));
    if (n <= 0 || static_cast<std::size_t>(n) >= sizeof(buf)) return false;

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

// ── Unified Tuya action converter ──────────────────────────────────────
//
// One decode path covering every cmd a Tuya scene switch, button remote
// or bound controller can send on cluster genOnOff:
//
//   cmd 0x00 / 0x01 / 0x02 — off / on / toggle (bound-controller frames)
//   cmd 0xFD `tuyaAction`  — byte 0/1/2 → single / double / hold
//   cmd 0xFC `tuyaAction2` — byte 0/1   → rotate_right / rotate_left
//
// MUTUALLY EXCLUSIVE with `fz_tuya_switch_scene` (which also matches
// cmd 0xFD on genOnOff). Pick one or the other per device, never both.
//
// Output format is selected by `TuyaActionConfig::prefix_with_endpoint`,
// stored in `FzConverter::user_config`:
//
//   flat   — emit `"single"`, `"off"`, `"rotate_right"`, … (no prefix)
//   per-ep — emit `"1_single"`, `"2_off"`, `"3_rotate_right"`, …
//
// Two pre-baked converter symbols are exported (`kFzTuyaActionFlat`,
// `kFzTuyaActionPerEp`). Three legacy symbols
// (`kFzTuyaOnOffAction`, `kFzTuyaMultiAction`, `kFzTuyaButtonAction`)
// are kept as separate FzConverter objects pointing at the same
// underlying function with the historically-correct config so existing
// auto-generated device files compile unchanged.

namespace {

enum class ActKind : std::uint8_t {
    Off = 0, On, Toggle, Single, Double, Hold, RotateRight, RotateLeft,
    Count
};

constexpr const char* const kBareLabels[
    static_cast<std::size_t>(ActKind::Count)] = {
    "off", "on", "toggle", "single", "double", "hold",
    "rotate_right", "rotate_left",
};

// Indexed by [src_endpoint - 1][ActKind]. 8 endpoints × 8 kinds.
// Static rodata, ~512 B; the strings outlive any FixedPayload that
// borrows them via Value::StringRef.
constexpr const char* const kEpLabels[8][
    static_cast<std::size_t>(ActKind::Count)] = {
    {"1_off","1_on","1_toggle","1_single","1_double","1_hold","1_rotate_right","1_rotate_left"},
    {"2_off","2_on","2_toggle","2_single","2_double","2_hold","2_rotate_right","2_rotate_left"},
    {"3_off","3_on","3_toggle","3_single","3_double","3_hold","3_rotate_right","3_rotate_left"},
    {"4_off","4_on","4_toggle","4_single","4_double","4_hold","4_rotate_right","4_rotate_left"},
    {"5_off","5_on","5_toggle","5_single","5_double","5_hold","5_rotate_right","5_rotate_left"},
    {"6_off","6_on","6_toggle","6_single","6_double","6_hold","6_rotate_right","6_rotate_left"},
    {"7_off","7_on","7_toggle","7_single","7_double","7_hold","7_rotate_right","7_rotate_left"},
    {"8_off","8_on","8_toggle","8_single","8_double","8_hold","8_rotate_right","8_rotate_left"},
};

struct TuyaActionConfig {
    bool prefix_with_endpoint;
};

constexpr TuyaActionConfig kCfgActFlat  { /*prefix=*/false };
constexpr TuyaActionConfig kCfgActPerEp { /*prefix=*/true  };

// ── Retransmit dedup ──────────────────────────────────────────────
//
// Tuya scene-switch buttons retransmit each press 2-3× with the SAME
// transaction-sequence byte. Without dedup, rules fire 2-3× per press.
// z2m solves this with `utils.hasAlreadyProcessedMessage` keyed on
// `(ieee, tsn)` inside an ~500 ms window. We mirror the shape but key
// on `RuntimeContext::device_index` with an 800 ms window — Tuya retx
// spacing exceeds typical Zigbee retry intervals.
//
// State lives on `DeviceRuntimeState` so each device has its own slot
// (no cross-device interference, no test-order coupling). Devices
// without a backing store (host fixtures with no RuntimeStore wired)
// skip dedup and surface every frame — same fallback as `device_index
// == 0` (synthetic test frames).

constexpr std::uint32_t kDedupWindowMs = 800;

bool is_action_retransmit(RuntimeContext& ctx,
                           std::uint8_t   tsn,
                           std::uint32_t  now_ms) {
    if (ctx.device_index == 0) return false;
    DeviceRuntimeState* st = ctx.device_state();
    if (!st) return false;
    if (st->tuya_action_seen) {
        const std::uint32_t age = now_ms - st->tuya_action_last_ms;
        if (st->tuya_action_last_tsn == tsn && age < kDedupWindowMs) {
            // Refresh timestamp so a stuck retx burst stays suppressed
            // even past the initial window.
            st->tuya_action_last_ms = now_ms;
            return true;
        }
    }
    st->tuya_action_last_tsn = tsn;
    st->tuya_action_last_ms  = now_ms;
    st->tuya_action_seen     = true;
    return false;
}

}  // namespace

// Test-only: per-device state lives on DeviceRuntimeState which the
// test harness already resets between cases — this hook stays as a
// no-op so existing test_ts0044.cpp forward declarations keep
// compiling. Once those tests are updated to call DeviceRuntimeState
// reset directly, this can be removed.
void reset_action_dedup_cache_for_test() {}

bool fz_tuya_action(const DecodedMessage& msg, const FzConverter& self,
                     const PreparedDefinition&,
                     RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* cfg =
        reinterpret_cast<const TuyaActionConfig*>(self.user_config);
    if (!cfg) return false;

    // Dedup retx before any decode work — same (device, tsn) within
    // window returns "matched but no output", so the dispatcher won't
    // log it as unhandled and the rules engine sees no `action` field.
    if (is_action_retransmit(ctx,
                              msg.transaction_sequence,
                              ctx.now())) {
        return true;
    }

    ActKind kind;
    switch (msg.command_id) {
        case 0x00: kind = ActKind::Off;    break;
        case 0x01: kind = ActKind::On;     break;
        case 0x02: kind = ActKind::Toggle; break;
        case 0xFD:
        case 0xFC: {
            // Skip ZCL header: 3 bytes (fc,tsn,cmd) + 2 for manu-code.
            const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
            if (msg.raw_data.size() < hdr + 1) return false;
            const std::uint8_t value = msg.raw_data[hdr];
            if (msg.command_id == 0xFD) {
                switch (value) {
                    case 0: kind = ActKind::Single; break;
                    case 1: kind = ActKind::Double; break;
                    case 2: kind = ActKind::Hold;   break;
                    default: return false;
                }
            } else {  // 0xFC tuyaAction2 — rotary remotes
                switch (value) {
                    case 0: kind = ActKind::RotateRight; break;
                    case 1: kind = ActKind::RotateLeft;  break;
                    default: return false;
                }
            }
            break;
        }
        default: return false;
    }

    const auto idx = static_cast<std::size_t>(kind);
    const char* label = nullptr;
    if (cfg->prefix_with_endpoint) {
        const std::uint8_t ep = msg.src_endpoint;
        if (ep < 1 || ep > 8) return false;
        label = kEpLabels[ep - 1][idx];
    } else {
        label = kBareLabels[idx];
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

#define ZHC_TUYA_ACTION_CVT(var, cfg_ptr)                                 \
    extern const FzConverter var{                                          \
        .family            = FrameFamily::Zcl,                             \
        .cluster           = "genOnOff",                                   \
        .type_mask         = type_bit(MessageType::Command),               \
        .command_id        = WILDCARD_CMD_ID,                              \
        .attr_id           = WILDCARD_ATTR_ID,                             \
        .endpoint          = WILDCARD_ENDPOINT,                            \
        .frame_flags_mask  = 0,                                            \
        .frame_flags_value = 0,                                            \
        .direction         = Direction::ClientToServer,                    \
        .fn                = { .zcl_fn = fz_tuya_action },                 \
        .user_config       = cfg_ptr,                                      \
    }

// Canonical names — preferred for new defs.
ZHC_TUYA_ACTION_CVT(kFzTuyaActionFlat,   &kCfgActFlat);
ZHC_TUYA_ACTION_CVT(kFzTuyaActionPerEp,  &kCfgActPerEp);

// Legacy names — kept as separate FzConverter instances pointing at the
// same unified function with the config each name historically implied:
//   kFzTuyaOnOffAction  was bare on/off/toggle  → flat
//   kFzTuyaMultiAction  was bare single/double  → flat
//   kFzTuyaButtonAction was "<ep>_<click>"      → per-ep
// Auto-generated device files still reference these by name.
ZHC_TUYA_ACTION_CVT(kFzTuyaOnOffAction,  &kCfgActFlat);
ZHC_TUYA_ACTION_CVT(kFzTuyaMultiAction,  &kCfgActFlat);
ZHC_TUYA_ACTION_CVT(kFzTuyaButtonAction, &kCfgActPerEp);

// MUTUALLY EXCLUSIVE with `fz_tuya_action` (which also matches cmd
// 0xFD on genOnOff). Wire only one of these per device:
//   * scene pickers (e.g. TS0601 scene switches) → `kFzTuyaSwitchScene`
//   * button presses (single/double/hold) → `kFzTuyaAction*`
// Wiring both produces conflicting `action` keys (last-writer-wins via
// the dispatcher merge, but the device's intent is ambiguous).
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

// manuSpecificTuya3 (0xE001) switchType: ENUM8 attr 0xD030.
constexpr ::zhc::generic::ZclWriteLookup kSwitchTypeLut[] = {
    {"toggle", 0}, {"state", 1}, {"momentary", 2},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecTuyaSwitchType{
    "switch_type", 0xD030, 0x30, 0, kSwitchTypeLut, 3,
};

// fz: manuSpecificTuya3 (0xE001) attr 0xD030 (53296) attribute report
// → emit `switch_type` as StringRef. Mirrors the tuyaTz lookup
// (toggle=0/state=1/momentary=2).
bool fz_tuya_switch_type(const DecodedMessage& msg, const FzConverter&,
                          const PreparedDefinition&, RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("53296");  // 0xD030
    if (!v || v->type != ValueType::Uint) return false;
    const char* label = nullptr;
    switch (v->u) {
        case 0: label = "toggle"; break;
        case 1: label = "state"; break;
        case 2: label = "momentary"; break;
        default: return false;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("switch_type", o);
    return true;
}

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

// manuSpecificTuya3 (cluster 0xE001) switchType write.
extern const TzConverter kTzTuyaSwitchType{
    .key         = "switch_type",
    .cluster     = "manuSpecificTuya2",     // ZHC name; upstream Tuya3
    .cluster_id  = 0xE001,
    .command_id  = 0x02,                    // Write Attributes
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecTuyaSwitchType,
};

extern const FzConverter kFzTuyaSwitchType{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificTuya2",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_tuya_switch_type },
    .user_config       = nullptr,
};

}  // namespace zhc::tuya
