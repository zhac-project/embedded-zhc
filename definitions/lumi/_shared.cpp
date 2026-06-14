// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared lumi converters.
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts

#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/foundation.hpp"

namespace zhc::lumi {

namespace {

constexpr std::uint16_t ATTR_LUMI_BASIC = 0xFF01;

// `3000 - 2850` mV range — z2m's default CR2032 curve.
constexpr std::uint32_t BATT_MV_LO = 2850;
constexpr std::uint32_t BATT_MV_HI = 3000;

std::uint8_t voltage_to_pct(std::uint64_t mv) {
    if (mv >= BATT_MV_HI) return 100;
    if (mv <= BATT_MV_LO) return 0;
    return static_cast<std::uint8_t>(
        ((mv - BATT_MV_LO) * 100) / (BATT_MV_HI - BATT_MV_LO));
}

}  // namespace

bool fz_lumi_basic(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Xiaomi packs everything into attr 0xFF01 (= "65281" decimal key
    // when no static map was supplied to the decoder).
    const Value* v = msg.payload.find("65281");
    if (!v || v->type != ValueType::BytesRef) return false;

    // Parse into `msg.mi_struct_arena` (mutable) to keep this function
    // off the hot task-stack path. The key scratch still lives locally
    // (~192 B) since MI-struct keys must outlive this call.
    auto& tlv = msg.mi_struct_arena;
    tlv = FixedPayload<ZHC_MI_STRUCT_CAP>{};
    char key_scratch[8 * ZHC_MI_STRUCT_CAP];
    if (!parse_mi_struct(v->bytes, key_scratch, sizeof(key_scratch), tlv)) {
        return false;
    }

    bool emitted_anything = false;
    for (std::uint8_t i = 0; i < tlv.count; ++i) {
        const auto& kv = tlv.items[i];

        // Tag 0x01 — battery voltage in millivolts (u16).
        if (std::strcmp(kv.key, "1") == 0 &&
            kv.value.type == ValueType::Uint) {
            Value voltage{}; voltage.type = ValueType::Uint;
            voltage.u = kv.value.u;
            out.put("voltage", voltage);

            Value battery{}; battery.type = ValueType::Uint;
            battery.u = voltage_to_pct(kv.value.u);
            out.put("battery", battery);
            emitted_anything = true;
            continue;
        }

        // Tag 0x03 — device temperature in °C (i8).
        if (std::strcmp(kv.key, "3") == 0 &&
            kv.value.type == ValueType::Int) {
            Value tv{}; tv.type = ValueType::Int; tv.i = kv.value.i;
            out.put("device_temperature", tv);
            emitted_anything = true;
            continue;
        }

        // Tag 0x05 — power outage counter (u16). z2m reports `value - 1`
        // (lib/lumi.ts numericAttributes2Payload, case "5"). Tag 0x04 is
        // mode_switch and only on wall-switch models — it yields nothing for
        // WXKG01LM — so the old tag-4-no-offset read was both the wrong tag and
        // missing the -1. Guard the unsigned subtraction against a 0 input.
        if (std::strcmp(kv.key, "5") == 0 &&
            kv.value.type == ValueType::Uint) {
            Value cv{}; cv.type = ValueType::Uint;
            cv.u = (kv.value.u > 0) ? (kv.value.u - 1) : 0;
            out.put("power_outage_count", cv);
            emitted_anything = true;
            continue;
        }
    }
    return emitted_anything;
}

extern const FzConverter kFzLumiBasic{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_basic },
    .user_config       = nullptr,
};

// ── Multistate action mapper ────────────────────────────────────────

namespace {
constexpr LumiActionEntry kDefaultLumiActionEntries[] = {
    {0,   "hold"},
    {1,   "single"},
    {2,   "double"},
    {3,   "triple"},
    {255, "release"},
};
}  // namespace

extern const LumiActionMap kDefaultLumiActionMap{
    .entries = kDefaultLumiActionEntries,
    .count   = sizeof(kDefaultLumiActionEntries) /
               sizeof(kDefaultLumiActionEntries[0]),
};

bool fz_lumi_action_multistate(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition&,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");  // 0x0055 → "85"
    if (!v || v->type != ValueType::Uint) return false;

    // Per-device action map lives in user_config; fallback to the
    // canonical 5-entry map when a device doesn't supply one.
    const auto* map = static_cast<const LumiActionMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) map = &kDefaultLumiActionMap;

    const char* label = nullptr;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        if (map->entries[i].value == v->u) {
            label = map->entries[i].action;
            break;
        }
    }
    if (!label) return false;

    Value action{}; action.type = ValueType::StringRef; action.str = label;
    out.put("action", action);

    if (auto* st = ctx.device_state()) {
        st->counter += 1;
        Value clicks{}; clicks.type = ValueType::Uint; clicks.u = st->counter;
        out.put("click_count", clicks);
    }
    return true;
}

bool fz_lumi_action_WXKG01LM(const DecodedMessage& msg,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext& ctx,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Click-count variant: attr 0x8000 present → map directly.
    if (const Value* count = msg.payload.find("32768")) {
        std::uint64_t n = 0;
        if (count->type == ValueType::Uint)      n = count->u;
        else if (count->type == ValueType::Int)  n = static_cast<std::uint64_t>(count->i);
        else return false;

        const char* label;
        switch (n) {
            case 1:  label = "single";    break;
            case 2:  label = "double";    break;
            case 3:  label = "triple";    break;
            case 4:  label = "quadruple"; break;
            default: label = "many";      break;
        }
        Value a{}; a.type = ValueType::StringRef; a.str = label;
        out.put("action", a);
        return true;
    }

    // Press / release state machine on attr 0x0000.
    const Value* onoff = msg.payload.find("0");
    if (!onoff) return false;
    std::uint64_t state;
    if (onoff->type == ValueType::Bool)      state = onoff->b ? 1 : 0;
    else if (onoff->type == ValueType::Uint) state = onoff->u;
    else return false;

    auto* st = ctx.device_state();
    const std::uint32_t now = ctx.now();
    // Reserve 0 as "no press recorded"; bias all timestamps by 1 so a
    // wall-clock of 0 still counts as an active press.
    const std::uint32_t press_mark = now == 0 ? 1 : now;

    if (state == 0) {
        if (st) st->press_start_ms = press_mark;
        // Claim the frame even though we emit nothing — the press-down
        // edge is part of our state machine, not an unhandled event.
        // Returning true keeps the dispatch layer from logging
        // `(no match)` for the half of each press we intentionally
        // consume silently.
        return true;
    }
    if (state == 1) {
        if (!st || st->press_start_ms == 0) return false;
        const std::uint32_t dur = press_mark - st->press_start_ms;
        st->press_start_ms = 0;

        Value action{}; action.type = ValueType::StringRef;
        action.str = (dur >= 1000) ? "hold" : "single";
        out.put("action", action);

        Value duration{}; duration.type = ValueType::Uint; duration.u = dur;
        out.put("duration", duration);
        return true;
    }
    // state outside {0,1} without a click-count attr — bad frame.
    return false;
}

extern const FzConverter kFzLumiActionWXKG01LM{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_action_WXKG01LM },
    .user_config       = nullptr,
};

extern const FzConverter kFzLumiActionMultistate{
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
    .fn                = { .zcl_fn = fz_lumi_action_multistate },
    .user_config       = nullptr,
};

// ── Xiaomi Mi Cube (MFKZQ01LM) action decoders ─────────────────────
//
// z2m-source: lumi.ts `lumi_action_multistate` + `lumi_action_analog`.

bool fz_lumi_cube_multistate(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");    // attr 0x0055
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint64_t value = v->u;

    const char* label = nullptr;
    std::int64_t side_v = -1;
    std::int64_t from_v = -1;
    std::int64_t to_v   = -1;

    if      (value == 0) label = "shake";
    else if (value == 1) label = "throw";
    else if (value == 2) label = "wakeup";
    else if (value == 3) label = "fall";
    else if (value >= 512) { label = "tap";     side_v = value - 512; }
    else if (value >= 256) { label = "slide";   side_v = value - 256; }
    else if (value >= 128) { label = "flip180"; side_v = value - 128; }
    else if (value >= 64) {
        label  = "flip90";
        from_v = (value - 64) / 8;
        to_v   = value % 8;
        side_v = to_v;
    }
    if (!label) return false;

    Value a{}; a.type = ValueType::StringRef; a.str = label;
    out.put("action", a);
    if (side_v >= 0) {
        Value s{}; s.type = ValueType::Int; s.i = side_v;
        out.put("side", s);
    }
    if (from_v >= 0) {
        Value f{}; f.type = ValueType::Int; f.i = from_v;
        out.put("action_from_side", f);
        out.put("from_side", f);
    }
    if (to_v >= 0) {
        Value t{}; t.type = ValueType::Int; t.i = to_v;
        out.put("action_to_side", t);
        out.put("to_side", t);
        out.put("action_side", t);
    }
    return true;
}

extern const FzConverter kFzLumiCubeMultistate{
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
    .fn                = { .zcl_fn = fz_lumi_cube_multistate },
    .user_config       = nullptr,
};

bool fz_lumi_cube_analog(const DecodedMessage& msg,
                          const FzConverter&,
                          const PreparedDefinition&,
                          RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");    // attr 0x0055 (float)
    if (!v || v->type != ValueType::Float) return false;

    // Quantise to 1/100° so downstream key matches z2m (Math.floor * 100).
    const float raw = v->f;
    const float quant = static_cast<float>(
        static_cast<std::int64_t>(raw * 100.0f)) / 100.0f;

    Value a{}; a.type = ValueType::StringRef;
    a.str = (raw < 0.0f) ? "rotate_left" : "rotate_right";
    out.put("action", a);

    Value angle{}; angle.type = ValueType::Float; angle.f = quant;
    out.put("angle", angle);
    out.put("action_angle", angle);
    return true;
}

bool fz_lumi_ctpr01_multistate(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");    // attr 0x0055
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint64_t value = v->u;

    const char* label = nullptr;
    std::int64_t side_v = -1;
    std::int64_t from_v = -1;

    if      (value == 0) label = "shake";
    else if (value == 1) label = "throw";
    else if (value == 2) label = "1_min_inactivity";
    else if (value == 4) label = "hold";
    else if (value >= 1024) { label = "flip_to_side"; side_v = value - 1023; }
    else if (value >= 512)  { label = "tap";          side_v = value - 511;  }
    else if (value >= 256)  { label = "slide";        side_v = value - 255;  }
    else if (value >= 128)  {
        label  = "flip180";
        side_v = static_cast<std::int64_t>(value) - 127;
        from_v = 7 - static_cast<std::int64_t>(value) + 127;
    } else if (value >= 64) {
        label  = "flip90";
        side_v = (value % 8) + 1;
        from_v = ((value - 64) / 8) + 1;
    }
    if (!label) return false;

    Value a{}; a.type = ValueType::StringRef; a.str = label;
    out.put("action", a);
    if (side_v >= 0) {
        Value s{}; s.type = ValueType::Int; s.i = side_v;
        out.put("side", s);
    }
    if (from_v >= 0) {
        Value f{}; f.type = ValueType::Int; f.i = from_v;
        out.put("action_from_side", f);
    }
    return true;
}

extern const FzConverter kFzLumiCTPR01Multistate{
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
    .fn                = { .zcl_fn = fz_lumi_ctpr01_multistate },
    .user_config       = nullptr,
};

// Opple per-endpoint action. Uses a single-task static scratch for the
// composed `button_<ep>_<label>` key since FixedPayload stores string
// pointers. The zcl_attr task is the only caller, so reentrancy is
// guaranteed.
namespace {
constexpr std::size_t kOppleScratchCount = 4;
constexpr std::size_t kOppleScratchLen   = 32;
char g_opple_scratch[kOppleScratchCount][kOppleScratchLen];
std::uint8_t g_opple_scratch_head = 0;

const char* opple_compose(std::uint8_t endpoint, const char* label) {
    char* dst = g_opple_scratch[g_opple_scratch_head];
    g_opple_scratch_head = (g_opple_scratch_head + 1) % kOppleScratchCount;
    std::snprintf(dst, kOppleScratchLen, "button_%u_%s",
                  static_cast<unsigned>(endpoint), label);
    return dst;
}
}  // namespace

bool fz_lumi_action_opple(const DecodedMessage& msg,
                           const FzConverter& self,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");
    if (!v || v->type != ValueType::Uint) return false;

    const auto* map = static_cast<const LumiActionMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) map = &kDefaultLumiActionMap;

    const char* label = nullptr;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        if (map->entries[i].value == v->u) {
            label = map->entries[i].action;
            break;
        }
    }
    if (!label) return false;

    const char* composed = opple_compose(msg.src_endpoint, label);
    Value a{}; a.type = ValueType::StringRef; a.str = composed;
    out.put("action", a);
    return true;
}

// ── lumi_power_analog — z2m `lumi_power` ──────────────────────────
//
// `kFzLumiPower` (above in this file — pre-existing) decodes the
// Aqara haElectricalMeasurement path (voltage/current/power). z2m's
// own `lumi_power` is a different beast — just presentValue on
// genAnalogInput → `power`. Wire this alongside for devices that
// report via that cluster too.

bool fz_lumi_power_analog(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");   // 0x0055 presentValue
    if (!v) return false;
    Value o{};
    if (v->type == ValueType::Float) {
        o.type = ValueType::Float; o.f = v->f;
    } else if (v->type == ValueType::Int) {
        o.type = ValueType::Int;   o.i = v->i;
    } else if (v->type == ValueType::Uint) {
        o.type = ValueType::Uint;  o.u = v->u;
    } else {
        return false;
    }
    out.put("power", o);
    return true;
}

extern const FzConverter kFzLumiPowerAnalog{
    .family            = FrameFamily::Zcl,
    .cluster           = "genAnalogInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_power_analog },
    .user_config       = nullptr,
};

// ── lumi_manu — manuSpecificLumi (0xFCC0) attr dispatcher ────────
//
// Named `kFzLumiManuSpecific` to avoid clashing with the historical
// `kFzLumiSpecific` in this file (which decodes the 0xFF01 MI-struct
// on genBasic — legacy naming). This one matches z2m's actual
// `lumi_specific` converter on cluster manuSpecificLumi (0xFCC0).

bool fz_lumi_manu_specific(const DecodedMessage& msg,
                            const FzConverter& self,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = static_cast<const LumiSpecificMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    bool emitted = false;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        const auto& e = map->entries[i];
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        Value o{};
        switch (e.type) {
            case LumiSpecificType::Raw:
                o = *v;
                break;
            case LumiSpecificType::Bool: {
                bool b = false;
                if      (v->type == ValueType::Bool) b = v->b;
                else if (v->type == ValueType::Uint) b = v->u != 0;
                else if (v->type == ValueType::Int)  b = v->i != 0;
                else continue;
                o.type = ValueType::Bool; o.b = b;
                break;
            }
            case LumiSpecificType::Enum: {
                if (v->type != ValueType::Uint || !e.enum_table) continue;
                const char* label = nullptr;
                for (std::uint8_t j = 0; j < e.enum_count; ++j) {
                    if (e.enum_table[j].value == v->u) {
                        label = e.enum_table[j].label; break;
                    }
                }
                if (!label) continue;
                o.type = ValueType::StringRef; o.str = label;
                break;
            }
        }
        out.put(e.out_key, o);
        emitted = true;
    }
    return emitted;
}

// ── tz_lumi_manu_write ────────────────────────────────────────────

namespace {

std::size_t attr_type_value_len(std::uint8_t t) {
    switch (t) {
        case 0x10: return 1;   // boolean
        case 0x20: return 1;   // u8
        case 0x21: return 2;   // u16
        case 0x22: return 3;   // u24
        case 0x23: return 4;   // u32
        case 0x28: return 1;   // s8
        case 0x29: return 2;   // s16
        default:   return 0;   // unsupported in v1
    }
}

bool coerce_input(const Value& in,
                   const LumiManuWriteSpec& s,
                   std::uint32_t& out) {
    if (in.type == ValueType::Bool) { out = in.b ? 1 : 0; return true; }
    if (in.type == ValueType::Uint) { out = static_cast<std::uint32_t>(in.u); return true; }
    if (in.type == ValueType::Int)  { out = static_cast<std::uint32_t>(in.i); return true; }
    if (in.type == ValueType::StringRef && in.str && s.lookup) {
        for (std::uint8_t i = 0; i < s.lookup_count; ++i) {
            if (s.lookup[i].label &&
                std::strcmp(s.lookup[i].label, in.str) == 0) {
                out = s.lookup[i].value; return true;
            }
        }
    }
    return false;
}

}  // namespace

bool tz_lumi_manu_write(std::string_view key,
                         const Value& input,
                         const TzConverter& self,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         std::span<std::uint8_t> out,
                         std::size_t& out_size) {
    out_size = 0;
    const auto* spec = static_cast<const LumiManuWriteSpec*>(self.user_config);
    if (!spec) return false;
    if (spec->key && key != spec->key) return false;

    std::uint32_t v = 0;
    if (!coerce_input(input, *spec, v)) return false;

    const std::size_t vlen = attr_type_value_len(spec->attr_type);
    if (vlen == 0) return false;

    // fc=0x14 + manu_code:2 + tsn + cmd:1 + attr_id:2 + type:1 + value:vlen
    const std::size_t total = 1 + 2 + 1 + 1 + 2 + 1 + vlen;
    if (out.size() < total) return false;

    std::size_t p = 0;
    out[p++] = 0x14;                                    // fc
    out[p++] = static_cast<std::uint8_t>(spec->manufacturer_code & 0xFF);
    out[p++] = static_cast<std::uint8_t>((spec->manufacturer_code >> 8) & 0xFF);
    out[p++] = 0x00;                                    // tsn placeholder
    out[p++] = 0x02;                                    // cmd writeAttributes
    out[p++] = static_cast<std::uint8_t>(spec->attr_id & 0xFF);
    out[p++] = static_cast<std::uint8_t>((spec->attr_id >> 8) & 0xFF);
    out[p++] = spec->attr_type;
    for (std::size_t i = 0; i < vlen; ++i) {
        out[p++] = static_cast<std::uint8_t>((v >> (i * 8)) & 0xFF);
    }
    out_size = total;
    return true;
}

// ── Canonical manu-specific maps per sensor family ───────────────

namespace {

// Shared across RTCGQ13/14/15/16LM (and siblings). z2m:
//   0x010C (268) motion_sensitivity enum {low:1, medium:2, high:3}
//   0x0102 (258) detection_period seconds
//   0x0152 (338) trigger_indicator bool (LED feedback)
//   0x0144 (324) motion_sensitivity_alt (some variants mirror 0x010C)
constexpr LumiSpecificEnumEntry kManuMotionSensitivityLut[] = {
    { 1, "low" }, { 2, "medium" }, { 3, "high" },
};
constexpr LumiSpecificEntry kManuMotionEntries[] = {
    { 0x010C, "motion_sensitivity", LumiSpecificType::Enum,
      kManuMotionSensitivityLut, 3 },
    { 0x0102, "detection_period",   LumiSpecificType::Raw,  nullptr, 0 },
    { 0x0152, "trigger_indicator",  LumiSpecificType::Bool, nullptr, 0 },
};

// Shared across MCCGQ13..19LM contact sensors. z2m:
//   0x00FF (255) contact bool (some variants)
//   0x0142 (322) power_outage_count
//   0x0120 (288) detect_time / buffer
constexpr LumiSpecificEntry kManuContactEntries[] = {
    { 0x00FF, "contact",            LumiSpecificType::Bool, nullptr, 0 },
    { 0x0142, "power_outage_count", LumiSpecificType::Raw,  nullptr, 0 },
    { 0x0120, "detection_delay",    LumiSpecificType::Raw,  nullptr, 0 },
};

// Shared across WSDCGQ11/12/14LM temp/humidity (T1 E1). z2m:
//   0x010C (268) sensitivity_calibration (some variants)
//   0x014B (331) humidity_alarm_threshold
constexpr LumiSpecificEntry kManuTempHumEntries[] = {
    { 0x014B, "humidity_alarm_threshold", LumiSpecificType::Raw, nullptr, 0 },
};

// Shared across SJCGQ12/13LM water leak. z2m emits `temperature`
// diagnostic + flood bool on the modern cluster.
constexpr LumiSpecificEntry kManuWaterLeakEntries[] = {
    { 0x010C, "flood_event", LumiSpecificType::Bool, nullptr, 0 },
};

}  // namespace

extern const LumiSpecificMap kLumiManuMapMotion{
    kManuMotionEntries,
    sizeof(kManuMotionEntries) / sizeof(kManuMotionEntries[0]),
};
extern const LumiSpecificMap kLumiManuMapContact{
    kManuContactEntries,
    sizeof(kManuContactEntries) / sizeof(kManuContactEntries[0]),
};
extern const LumiSpecificMap kLumiManuMapTempHum{
    kManuTempHumEntries,
    sizeof(kManuTempHumEntries) / sizeof(kManuTempHumEntries[0]),
};
extern const LumiSpecificMap kLumiManuMapWaterLeak{
    kManuWaterLeakEntries,
    sizeof(kManuWaterLeakEntries) / sizeof(kManuWaterLeakEntries[0]),
};

// Macro to stamp out canonical converters over each family map.
#define ZHC_LUMI_MANU_CONVERTER(var, map_ref)                         \
    extern const FzConverter var{                                     \
        .family            = FrameFamily::Zcl,                        \
        .cluster           = "manuSpecificLumi",                      \
        .type_mask         = type_bit(MessageType::AttributeReport) | \
                             type_bit(MessageType::ReadResponse),     \
        .command_id        = WILDCARD_CMD_ID,                         \
        .attr_id           = WILDCARD_ATTR_ID,                        \
        .endpoint          = WILDCARD_ENDPOINT,                       \
        .frame_flags_mask  = 0, .frame_flags_value = 0,               \
        .direction         = Direction::ServerToClient,               \
        .fn                = { .zcl_fn = &fz_lumi_manu_specific },    \
        .user_config       = &map_ref,                                \
    };
ZHC_LUMI_MANU_CONVERTER(kFzLumiManuMotion,    kLumiManuMapMotion)
ZHC_LUMI_MANU_CONVERTER(kFzLumiManuContact,   kLumiManuMapContact)
ZHC_LUMI_MANU_CONVERTER(kFzLumiManuTempHum,   kLumiManuMapTempHum)
ZHC_LUMI_MANU_CONVERTER(kFzLumiManuWaterLeak, kLumiManuMapWaterLeak)
#undef ZHC_LUMI_MANU_CONVERTER

// Canonical tz specs + TzConverters for Lumi manu-specific writes.
namespace {
constexpr ::zhc::generic::ZclWriteSpec kSpecPowerOutageMemory{
    "power_outage_memory", 0x0201, 0x10, 0x115F, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedDisabledNight{
    "led_disabled_night",  0x0203, 0x10, 0x115F, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecButtonLock{
    "button_lock",         0x0200, 0x10, 0x115F, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecFlipIndicator{
    "flip_indicator_light", 0x00F5, 0x10, 0x115F, nullptr, 0,
};
}  // namespace

#define ZHC_LUMI_TZ(var, spec_ref, key_str)                           \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = "manuSpecificLumi",                            \
        .cluster_id  = 0xFCC0,                                        \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    };
ZHC_LUMI_TZ(kTzLumiPowerOutageMemory,  kSpecPowerOutageMemory,  "power_outage_memory")
ZHC_LUMI_TZ(kTzLumiLedDisabledNight,   kSpecLedDisabledNight,   "led_disabled_night")
ZHC_LUMI_TZ(kTzLumiButtonLock,         kSpecButtonLock,         "button_lock")
ZHC_LUMI_TZ(kTzLumiFlipIndicatorLight, kSpecFlipIndicator,      "flip_indicator_light")
#undef ZHC_LUMI_TZ

// (fz_lumi_curtain_position already exists lower in this file with
// the ZHC_LUMI_ATTRREPORT_CONVERTER macro — don't duplicate.)

// IAS Zone — attr 0x0002 zoneStatus. Bit 0 = alarm. Output key is
// passed via `user_config` as `const char*` (e.g. "water_leak").
bool fz_lumi_ias_alarm(const DecodedMessage& msg,
                        const FzConverter& self,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const char* key = static_cast<const char*>(self.user_config);
    if (!key) return false;
    const Value* v = msg.payload.find("2");   // 0x0002 zoneStatus
    if (!v) return false;
    std::uint64_t status = 0;
    if      (v->type == ValueType::Uint) status = v->u;
    else if (v->type == ValueType::Int)  status = static_cast<std::uint64_t>(v->i);
    else return false;
    Value o{}; o.type = ValueType::Bool; o.b = (status & 1) != 0;
    out.put(key, o);
    return true;
}

#define ZHC_LUMI_IAS(var, key_str)                                    \
    extern const FzConverter var{                                     \
        .family            = FrameFamily::Zcl,                        \
        .cluster           = "ssIasZone",                             \
        .type_mask         = type_bit(MessageType::AttributeReport) | \
                             type_bit(MessageType::ReadResponse),     \
        .command_id        = WILDCARD_CMD_ID,                         \
        .attr_id           = WILDCARD_ATTR_ID,                        \
        .endpoint          = WILDCARD_ENDPOINT,                       \
        .frame_flags_mask  = 0, .frame_flags_value = 0,               \
        .direction         = Direction::ServerToClient,               \
        .fn                = { .zcl_fn = &fz_lumi_ias_alarm },        \
        .user_config       = const_cast<char*>(key_str),              \
    };
ZHC_LUMI_IAS(kFzLumiWaterLeak, "water_leak")
ZHC_LUMI_IAS(kFzLumiSmoke,     "smoke")
ZHC_LUMI_IAS(kFzLumiGas,       "gas")
#undef ZHC_LUMI_IAS

extern const FzConverter kFzLumiManuSpecific{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificLumi",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_manu_specific },
    .user_config       = nullptr,
};

extern const FzConverter kFzLumiCubeAnalog{
    .family            = FrameFamily::Zcl,
    .cluster           = "genAnalogInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_cube_analog },
    .user_config       = nullptr,
};

// ── Occupancy mapper ────────────────────────────────────────────────

bool fz_occupancy(const DecodedMessage& msg,
                   const FzConverter&,
                   const PreparedDefinition&,
                   RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");  // attr 0x0000 → "0"
    if (!v) return false;
    bool occupied = false;
    if (v->type == ValueType::Bool) occupied = v->b;
    else if (v->type == ValueType::Uint) occupied = v->u != 0;
    else return false;
    Value o{}; o.type = ValueType::Bool; o.b = occupied;
    out.put("occupancy", o);
    return true;
}

extern const FzConverter kFzOccupancy{
    .family            = FrameFamily::Zcl,
    .cluster           = "msOccupancySensing",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_occupancy },
    .user_config       = nullptr,
};

// ── fz_lumi_specific ────────────────────────────────────────────────

namespace {

bool parse_decimal(const char* s, std::uint32_t& out) {
    if (!s || !*s) return false;
    std::uint32_t n = 0;
    for (; *s; ++s) {
        if (*s < '0' || *s > '9') return false;
        n = n * 10 + static_cast<std::uint32_t>(*s - '0');
    }
    out = n;
    return true;
}

}  // namespace

bool fz_lumi_specific(const DecodedMessage& msg,
                       const FzConverter&,
                       const PreparedDefinition& def,
                       RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Without a TagMap we can't resolve any tag — bail. Loudly: this
    // state is a porter bug and should be caught at review, not
    // silently tolerated.
    const auto* map = static_cast<const LumiTagMap*>(def.meta);
    if (!map || !map->entries || map->count == 0) return false;

    const Value* v = msg.payload.find("65281");         // attr 0xFF01
    if (!v || v->type != ValueType::BytesRef) return false;

    auto& tlv = msg.mi_struct_arena;
    tlv = FixedPayload<ZHC_MI_STRUCT_CAP>{};
    char key_scratch[8 * ZHC_MI_STRUCT_CAP];
    if (!parse_mi_struct(v->bytes, key_scratch, sizeof(key_scratch), tlv)) {
        return false;
    }

    bool emitted = false;
    for (std::uint8_t i = 0; i < tlv.count; ++i) {
        const auto& kv = tlv.items[i];

        std::uint32_t tag{};
        if (!parse_decimal(kv.key, tag)) continue;

        for (std::uint8_t j = 0; j < map->count; ++j) {
            if (map->entries[j].tag != tag) continue;

            std::int64_t raw;
            if      (kv.value.type == ValueType::Uint) raw = static_cast<std::int64_t>(kv.value.u);
            else if (kv.value.type == ValueType::Int)  raw = kv.value.i;
            else if (kv.value.type == ValueType::Bool) raw = kv.value.b ? 1 : 0;
            else break;

            const std::uint32_t d = map->entries[j].divisor == 0
                ? 1 : map->entries[j].divisor;
            if (d == 1) {
                Value o{}; o.type = ValueType::Int; o.i = raw;
                out.put(map->entries[j].key, o);
            } else {
                Value o{}; o.type = ValueType::Float;
                o.f = static_cast<float>(raw) / static_cast<float>(d);
                out.put(map->entries[j].key, o);
            }
            emitted = true;
            break;
        }
    }
    return emitted;
}

extern const FzConverter kFzLumiSpecific{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_specific },
    .user_config       = nullptr,
};

// ── fz_lumi_power ───────────────────────────────────────────────────

namespace {

bool read_raw(const Value* v, std::int64_t& out) {
    if (!v) return false;
    if      (v->type == ValueType::Uint) { out = static_cast<std::int64_t>(v->u); return true; }
    else if (v->type == ValueType::Int)  { out = v->i;                            return true; }
    return false;
}

void emit_scaled(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
                  const char* key, std::int64_t raw, std::uint32_t div) {
    const std::uint32_t d = div == 0 ? 1 : div;
    if (d == 1) {
        Value o{}; o.type = ValueType::Int; o.i = raw;
        out.put(key, o);
    } else {
        Value o{}; o.type = ValueType::Float;
        o.f = static_cast<float>(raw) / static_cast<float>(d);
        out.put(key, o);
    }
}

}  // namespace

bool fz_lumi_power(const DecodedMessage& msg,
                    const FzConverter& self,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Null user_config → fall back to pass-through across every field.
    static constexpr LumiPowerCalibration kPassThrough{1, 1, 1};
    const auto* cal = static_cast<const LumiPowerCalibration*>(self.user_config);
    if (!cal) cal = &kPassThrough;

    bool emitted = false;
    std::int64_t raw;

    if (read_raw(msg.payload.find("1285"), raw)) {         // 0x0505
        emit_scaled(out, "voltage", raw, cal->voltage_div);
        emitted = true;
    }
    if (read_raw(msg.payload.find("1288"), raw)) {         // 0x0508
        emit_scaled(out, "current", raw, cal->current_div);
        emitted = true;
    }
    if (read_raw(msg.payload.find("1291"), raw)) {         // 0x050B
        emit_scaled(out, "power",   raw, cal->power_div);
        emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzLumiPower{
    .family            = FrameFamily::Zcl,
    .cluster           = "haElectricalMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_power },
    .user_config       = nullptr,
};

// ── fz_lumi_electricity_meter ───────────────────────────────────────

bool fz_lumi_electricity_meter(const DecodedMessage& msg,
                                 const FzConverter& self,
                                 const PreparedDefinition&,
                                 RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    static constexpr LumiMeterCalibration kPassThrough{1};
    const auto* cal = static_cast<const LumiMeterCalibration*>(self.user_config);
    if (!cal) cal = &kPassThrough;

    std::int64_t raw;
    if (!read_raw(msg.payload.find("0"), raw)) return false;  // attr 0x0000
    emit_scaled(out, "energy", raw, cal->energy_div);
    return true;
}

extern const FzConverter kFzLumiElectricityMeter{
    .family            = FrameFamily::Zcl,
    .cluster           = "seMetering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_electricity_meter },
    .user_config       = nullptr,
};

// ── Remaining shared converters ────────────────────────────────────

// Shared selector for ServerToClient attribute reports on a named cluster.
#define ZHC_LUMI_ATTRREPORT_CONVERTER(varname, cluster_str, fn_ref)      \
    extern const FzConverter varname{                                     \
        .family            = FrameFamily::Zcl,                            \
        .cluster           = cluster_str,                                 \
        .type_mask         = type_bit(MessageType::AttributeReport) |     \
                             type_bit(MessageType::ReadResponse),         \
        .command_id        = WILDCARD_CMD_ID,                             \
        .attr_id           = WILDCARD_ATTR_ID,                            \
        .endpoint          = WILDCARD_ENDPOINT,                           \
        .frame_flags_mask  = 0,                                           \
        .frame_flags_value = 0,                                           \
        .direction         = Direction::ServerToClient,                   \
        .fn                = { .zcl_fn = fn_ref },                        \
        .user_config       = nullptr,                                     \
    }

// ── fz_lumi_contact ────────────────────────────────────────────────

bool fz_lumi_contact(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000
    if (!v) return false;

    bool open;
    if      (v->type == ValueType::Bool) open = !v->b;
    else if (v->type == ValueType::Uint) open = v->u == 0;
    else return false;

    Value contact{}; contact.type = ValueType::Bool; contact.b = !open;
    out.put("contact", contact);
    return true;
}

ZHC_LUMI_ATTRREPORT_CONVERTER(kFzLumiContact, "genOnOff", fz_lumi_contact);

// ── fz_lumi_on_off ─────────────────────────────────────────────────

bool fz_lumi_on_off(const DecodedMessage& msg,
                     const FzConverter& self,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000
    if (!v) return false;

    bool state;
    if      (v->type == ValueType::Bool) state = v->b;
    else if (v->type == ValueType::Uint) state = v->u != 0;
    else return false;

    // `label` in each DeviceEndpointLabel is the full output key
    // (e.g. "state_left"). Avoids heap / stack-buffer lifetime issues
    // because FixedPayload stores the pointer as-is.
    const char* key = "state";
    if (const auto* map = static_cast<const DeviceEndpointLabels*>(self.user_config)) {
        for (std::uint8_t i = 0; i < map->count; ++i) {
            if (map->entries[i].endpoint == msg.src_endpoint) {
                key = map->entries[i].label;
                break;
            }
        }
    }
    Value s{}; s.type = ValueType::Bool; s.b = state;
    out.put(key, s);
    return true;
}

ZHC_LUMI_ATTRREPORT_CONVERTER(kFzLumiOnOff, "genOnOff", fz_lumi_on_off);

// ── fz_lumi_action ─────────────────────────────────────────────────

bool fz_lumi_action(const DecodedMessage& msg,
                     const FzConverter& self,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x8000 (decimal 32768) u8 click count.
    const Value* v = msg.payload.find("32768");
    if (!v) return false;

    std::uint64_t raw;
    if      (v->type == ValueType::Uint) raw = v->u;
    else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
    else return false;

    const auto* map = static_cast<const LumiActionMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) map = &kDefaultLumiActionMap;

    const char* label = nullptr;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        if (map->entries[i].value == raw) {
            label = map->entries[i].action;
            break;
        }
    }
    if (!label) return false;

    Value action{}; action.type = ValueType::StringRef; action.str = label;
    out.put("action", action);
    return true;
}

ZHC_LUMI_ATTRREPORT_CONVERTER(kFzLumiAction, "genOnOff", fz_lumi_action);

// ── fz_lumi_operation_mode_basic ────────────────────────────────────

bool fz_lumi_operation_mode_basic(const DecodedMessage& msg,
                                    const FzConverter& self,
                                    const PreparedDefinition&,
                                    RuntimeContext&,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0xFF22 (decimal 65314) u8 — matches z2m `fp1e` / switches.
    const Value* v = msg.payload.find("65314");
    if (!v) return false;

    std::uint64_t raw;
    if      (v->type == ValueType::Uint) raw = v->u;
    else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
    else return false;

    const auto* map = static_cast<const LumiActionMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    for (std::uint8_t i = 0; i < map->count; ++i) {
        if (map->entries[i].value == raw) {
            Value o{}; o.type = ValueType::StringRef;
            o.str = map->entries[i].action;
            out.put("operation_mode", o);
            return true;
        }
    }
    return false;
}

ZHC_LUMI_ATTRREPORT_CONVERTER(kFzLumiOperationModeBasic, "genBasic",
                              fz_lumi_operation_mode_basic);

// ── fz_lumi_curtain_position ────────────────────────────────────────

bool fz_lumi_curtain_position(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0055 (decimal 85) — presentValue on genAnalogOutput.
    // Can arrive as Float (0x39) or u16 (0x21 / 0x29). Emit clipped 0..100.
    const Value* v = msg.payload.find("85");
    if (!v) return false;

    float pct;
    if      (v->type == ValueType::Float) pct = v->f;
    else if (v->type == ValueType::Uint)  pct = static_cast<float>(v->u);
    else if (v->type == ValueType::Int)   pct = static_cast<float>(v->i);
    else return false;

    if (pct < 0.0f)   pct = 0.0f;
    if (pct > 100.0f) pct = 100.0f;

    Value o{}; o.type = ValueType::Float; o.f = pct;
    out.put("position", o);
    return true;
}

ZHC_LUMI_ATTRREPORT_CONVERTER(kFzLumiCurtainPosition, "genAnalogOutput",
                              fz_lumi_curtain_position);

// ── fz_lumi_door_lock_report ────────────────────────────────────────

bool fz_lumi_door_lock_report(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 lockState
    if (!v) return false;

    std::uint64_t raw;
    if      (v->type == ValueType::Uint) raw = v->u;
    else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
    else return false;

    const char* label;
    switch (raw) {
        case 0: label = "not_fully_locked"; break;
        case 1: label = "unlocked";         break;
        case 2: label = "locked";           break;
        default: return false;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("lock_state", o);
    return true;
}

ZHC_LUMI_ATTRREPORT_CONVERTER(kFzLumiDoorLockReport, "closuresDoorLock",
                              fz_lumi_door_lock_report);

#undef ZHC_LUMI_ATTRREPORT_CONVERTER

// ── Shared attribute-reporting templates ───────────────────────────
//
// See _shared.hpp for the per-array z2m provenance. Field order:
//   { endpoint, cluster_id, attr_id, attr_type, min_s, max_s,
//     reportable_change, manufacturer_code }

// reporting.onOff → genOnOff 0x0006 / onOff 0x0000, bool, 0..3600s, rc 0.
const ReportingSpec kReportsLumiOnOff[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
};
const std::uint8_t kReportsLumiOnOffCount =
    static_cast<std::uint8_t>(sizeof(kReportsLumiOnOff) /
                              sizeof(kReportsLumiOnOff[0]));

// reporting.onOff + reporting.deviceTemperature.
//   genOnOff         0x0006 / 0x0000  bool  0..3600s   rc 0
//   genDeviceTempCfg 0x0002 / 0x0000  s16   300..3600s rc 1
const ReportingSpec kReportsLumiOnOffDevTemp[] = {
    {1, 0x0006, 0x0000, 0x10,   0, 3600, 0, 0},
    {1, 0x0002, 0x0000, 0x29, 300, 3600, 1, 0},
};
const std::uint8_t kReportsLumiOnOffDevTempCount =
    static_cast<std::uint8_t>(sizeof(kReportsLumiOnOffDevTemp) /
                              sizeof(kReportsLumiOnOffDevTemp[0]));

// reporting.onOff + reporting.currentSummDelivered (energy only — no V/I/P).
//   genOnOff   0x0006 / 0x0000  bool  0..3600s  rc 0
//   seMetering 0x0702 / 0x0000  u48   5..3600s  rc 257
const ReportingSpec kReportsLumiOnOffEnergy[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600,   0, 0},
    {1, 0x0702, 0x0000, 0x25, 5, 3600, 257, 0},
};
const std::uint8_t kReportsLumiOnOffEnergyCount =
    static_cast<std::uint8_t>(sizeof(kReportsLumiOnOffEnergy) /
                              sizeof(kReportsLumiOnOffEnergy[0]));

}  // namespace zhc::lumi
