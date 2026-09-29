// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared lumi converters.
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts

#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <utility>

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
                    const FzConverter& self,
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

    const auto* opts = static_cast<const LumiBasicOpts*>(self.user_config);
    bool emitted_anything = false;
    for (std::uint8_t i = 0; i < tlv.count; ++i) {
        const auto& kv = tlv.items[i];

        // Opt-in tags (LumiBasicOpts). Raw pass-through, like z2m.
        if (opts && kv.value.type != ValueType::BytesRef) {
            const char* key = nullptr;
            if (opts->energy && std::strcmp(kv.key, "149") == 0) key = "energy";
            else if (opts->tag100_key && std::strcmp(kv.key, "100") == 0) key = opts->tag100_key;
            if (key) {
                out.put(key, kv.value);
                emitted_anything = true;
                continue;
            }
        }

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

namespace {
constexpr LumiBasicOpts kLumiBasicEnergyOpts{ .energy = true, .tag100_key = nullptr };
}  // namespace

extern const FzConverter kFzLumiBasicEnergy{
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
    .user_config       = &kLumiBasicEnergyOpts,
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

// z2m lumi_action_multistate, wall-switch branch (see the header).
bool fz_lumi_switch_action(const DecodedMessage& msg, const FzConverter& self,
                           const PreparedDefinition&, RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");   // presentValue 0x0055
    if (!v || v->type != ValueType::Uint) return false;
    const char* action = nullptr;
    for (std::uint8_t i = 0; i < kDefaultLumiActionMap.count && !action; ++i)
        if (kDefaultLumiActionMap.entries[i].value == v->u) action = kDefaultLumiActionMap.entries[i].action;
    if (!action) return false;

    if (const auto* lookup = static_cast<const LumiButtons*>(self.user_config)) {
        const char* button = nullptr;
        for (std::uint8_t i = 0; i < lookup->count && !button; ++i)
            if (lookup->buttons[i].endpoint == msg.src_endpoint) button = lookup->buttons[i].name;
        if (!button) return false;
        char buf[32];
        const int n = std::snprintf(buf, sizeof(buf), "%s_%s", action, button);
        if (n <= 0 || static_cast<std::size_t>(n) >= sizeof(buf)) return false;
        action = ctx.alloc_str(buf, static_cast<std::size_t>(n));
        if (!action) return false;
    }
    Value a{};
    a.type = ValueType::StringRef;
    a.str  = action;
    out.put("action", a);
    return true;
}

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
    if (in.type == ValueType::Float) {   // wire unit as-is: round, never truncate
        if (in.f != in.f) return false;
        out = static_cast<std::uint32_t>(static_cast<std::int64_t>(
                  in.f + (in.f >= 0.0f ? 0.5f : -0.5f)));
        return true;
    }
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
// z2m lumi_socket_button_lock / lumiButtonLock: ON = 0, OFF = 1 (a boolean
// true is ON).
constexpr ::zhc::generic::ZclWriteLookup kButtonLockLut[] = {{"ON", 0}, {"OFF", 1}};
constexpr ::zhc::generic::ZclWriteSpec kSpecButtonLock{
    "button_lock",         0x0200, 0x20, 0x115F, kButtonLockLut, 2,
    ::zhc::generic::kZclWriteFlagInvertBool,
};
// z2m lumiFlipIndicatorLight.
constexpr ::zhc::generic::ZclWriteLookup kFlipIndicatorLut[] = {{"ON", 1}, {"OFF", 0}};
constexpr ::zhc::generic::ZclWriteSpec kSpecFlipIndicator{
    "flip_indicator_light", 0x00F0, 0x20, 0x115F, kFlipIndicatorLut, 2,
};
// z2m lumi_switch_mode_switch.
constexpr ::zhc::generic::ZclWriteLookup kModeSwitchLut[] = {{"anti_flicker_mode", 4}, {"quick_mode", 1}};
constexpr ::zhc::generic::ZclWriteSpec kSpecModeSwitch{
    "mode_switch", 0x0004, 0x21, 0x115F, kModeSwitchLut, 2,
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
ZHC_LUMI_TZ(kTzLumiModeSwitch,         kSpecModeSwitch,         "mode_switch")
#undef ZHC_LUMI_TZ

// tz.on_off for one rocker: its own key, its own endpoint, the plain on/off frame.
namespace {
bool tz_lumi_state_endpoint(std::string_view, const Value& input, const TzConverter& self,
                            const PreparedDefinition& def, RuntimeContext& ctx,
                            std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    return ::zhc::generic::tz_on_off("state", input, self, def, ctx, out_frame, out_size);
}
}  // namespace

#define ZHC_LUMI_STATE_TZ(var, key_str, ep)                              \
    extern const TzConverter var{                                        \
        .key         = key_str,                                          \
        .cluster     = "genOnOff",                                       \
        .cluster_id  = 0x0006,                                           \
        .command_id  = 0x00,                                             \
        .fn          = &tz_lumi_state_endpoint,                          \
        .user_config = nullptr,                                          \
        .endpoint    = ep,                                               \
    };
ZHC_LUMI_STATE_TZ(kTzLumiStateTop,    "state_top",    1)
ZHC_LUMI_STATE_TZ(kTzLumiStateBottom, "state_bottom", 2)
ZHC_LUMI_STATE_TZ(kTzLumiStateLeft,   "state_left",   1)
ZHC_LUMI_STATE_TZ(kTzLumiStateRight,  "state_right",  2)
#undef ZHC_LUMI_STATE_TZ

// ── Aqara's own requests: "may I reset?" and "leave" ────────────────
//
// z2m lumiPreventReset / lumiPreventLeave (lib/lumi.ts). The answer goes out
// through the configure write hook, which the adapter also hands the RX path.

namespace {

bool fz_lumi_prevent_reset(const DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    static constexpr std::uint8_t kAsk[] = {0xAA, 0x10, 0x05, 0x41, 0x87};
    const Value* v = msg.payload.find("65520");   // genBasic 0xFFF0
    if (!v || v->type != ValueType::BytesRef || v->bytes.size() < sizeof(kAsk) ||
        std::memcmp(v->bytes.data(), kAsk, sizeof(kAsk)) != 0) {
        return false;
    }
    // Octet string on the wire: its length, then aa 10 05 41 47 01 01 10 01.
    static constexpr std::uint8_t kStay[] = {0x09, 0xAA, 0x10, 0x05, 0x41,
                                             0x47, 0x01, 0x01, 0x10, 0x01};
    return ctx.configure_write &&
           ctx.configure_write(ctx.device_index, 1, 0x0000, 0xFFF0, 0x41,
                               kStay, sizeof(kStay), 0x115F);
}

bool fz_lumi_prevent_leave(const DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    const Value* v = msg.payload.find("252");     // 0xFCC0 0x00FC
    if (!v || v->type != ValueType::Bool || v->b) return false;
    static constexpr std::uint8_t kStay[] = {0x01};
    return ctx.configure_write &&
           ctx.configure_write(ctx.device_index, 1, 0xFCC0, 0x00FC, 0x10,
                               kStay, sizeof(kStay), 0x115F);
}

}  // namespace

extern const FzConverter kFzLumiPreventReset{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::AttributeReport),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_prevent_reset },
    .user_config       = nullptr,
};

extern const FzConverter kFzLumiPreventLeave{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificLumi",
    .type_mask         = type_bit(MessageType::AttributeReport),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_prevent_leave },
    .user_config       = nullptr,
};

// ── Modern heartbeat: manuSpecificLumi 0x00F7 ───────────────────────

namespace {

// The 0x00F7 tags the heartbeat reads, ascending. z2m walks the decoded struct
// as a JS object, whose integer keys come out in ascending order; that decides
// who wins when two tags feed one key (voltage: 150 after 1).
constexpr std::uint8_t kHbTags[] = {1, 2, 3, 5, 23, 24, 101, 102, 149, 150, 151, 152};
constexpr const char* kHbKeys[] = {"1", "2", "3", "5", "23", "24",
                                   "101", "102", "149", "150", "151", "152"};
static_assert(sizeof(kHbTags) == sizeof(kHbKeys) / sizeof(kHbKeys[0]));

const char* hb_key(std::uint8_t tag) {
    for (std::size_t k = 0; k < sizeof(kHbTags); ++k)
        if (kHbTags[k] == tag) return kHbKeys[k];
    return nullptr;
}

// z2m buffer2DataObject: tag · type · value records, walked while more than
// one byte is left, so a trailing byte is never read as a tag. Integers are
// little-endian except the 64-bit ones, which z2m reads big-endian. 0x42 and
// 0x5F carry 1 and 4 bytes nobody reads; any other unknown type moves one byte
// on and reads the next tag from there; a double (0x3A) is read whole but only
// 4 of its bytes are stepped over. The last record of a tag wins. Where z2m's
// Buffer read would throw on a record cut short, this stops and keeps the rest.
void lumi_buffer_to_data(std::span<const std::uint8_t> b,
                         FixedPayload<ZHC_MI_STRUCT_CAP>& out) {
    std::size_t i = 0;
    while (i + 1 < b.size()) {
        const std::uint8_t tag = b[i], type = b[i + 1];
        std::size_t len = 0;    // value bytes read
        std::size_t step = 0;   // bytes to the next tag, when not 2 + len
        switch (type) {
            case 0x10: case 0x20: case 0x28: len = 1; break;
            case 0x21: case 0x29: len = 2; break;
            case 0x22: case 0x2A: len = 3; break;
            case 0x23: case 0x2B: case 0x39: len = 4; break;
            case 0x24: case 0x2C: len = 5; break;
            case 0x25: case 0x2D: len = 6; break;
            case 0x26: case 0x2E: len = 7; break;
            case 0x27: case 0x2F: len = 8; break;
            case 0x3A: len = 8; step = 6; break;
            case 0x42: step = 3; break;
            case 0x5F: step = 6; break;
            default:   step = 1; break;
        }
        if (len) {
            if (i + 2 + len > b.size()) break;
            const std::uint8_t* p = b.data() + i + 2;
            const bool big_endian = type == 0x27 || type == 0x2F;
            std::uint64_t u = 0;
            for (std::size_t k = 0; k < len; ++k)
                u |= static_cast<std::uint64_t>(p[big_endian ? len - 1 - k : k]) << (8 * k);
            Value v{};
            if (type == 0x39) {
                const auto bits = static_cast<std::uint32_t>(u);
                float f;
                std::memcpy(&f, &bits, sizeof(f));
                v.type = ValueType::Float; v.f = f;
            } else if (type == 0x3A) {
                double d;
                std::memcpy(&d, &u, sizeof(d));
                v.type = ValueType::Float; v.f = static_cast<float>(d);
            } else if (type >= 0x28 && type <= 0x2F) {
                std::int64_t s = static_cast<std::int64_t>(u);
                if (len < 8 && (u >> (8 * len - 1)) & 1) s -= std::int64_t{1} << (8 * len);
                v.type = ValueType::Int; v.i = s;
            } else {
                v.type = ValueType::Uint; v.u = u;   // 0x10 too: z2m reads it as a number
            }
            if (const char* k = hb_key(tag)) {
                bool replaced = false;
                for (std::uint8_t j = 0; j < out.count; ++j)
                    if (out.items[j].key == k) { out.items[j].value = v; replaced = true; }
                if (!replaced) out.put(k, v);
            }
            if (!step) step = 2 + len;
        }
        i += step;
    }
}

double hb_num(const Value& v) {
    switch (v.type) {
        case ValueType::Uint:  return static_cast<double>(v.u);
        case ValueType::Int:   return static_cast<double>(v.i);
        case ValueType::Float: return v.f;
        default:               return 0.0;
    }
}

bool model_is(const char* model, std::initializer_list<const char*> models) {
    for (const char* m : models)
        if (std::strcmp(model, m) == 0) return true;
    return false;
}

// z2m batteryVoltageToPercentage with {min, max}: toPercentage, a rounded
// linear map clamped to 0-100.
Value hb_percent(double mv, const LumiHeartbeatOpts& o) {
    const double lo = o.min_mv, hi = o.max_mv;
    const double c = mv > hi ? hi : (mv < lo ? lo : mv);
    Value v{};
    v.type = ValueType::Uint;
    v.u = static_cast<std::uint64_t>(std::floor((c - lo) / (hi - lo) * 100.0 + 0.5));
    return v;
}

Value hb_float(double d) { Value v{}; v.type = ValueType::Float; v.f = static_cast<float>(d); return v; }
Value hb_int(double d)   { Value v{}; v.type = ValueType::Int;   v.i = static_cast<std::int64_t>(d); return v; }

}  // namespace

bool fz_lumi_heartbeat(const DecodedMessage& msg, const FzConverter& self,
                       const PreparedDefinition& def, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* o = static_cast<const LumiHeartbeatOpts*>(self.user_config);
    const Value* raw = msg.payload.find("247");   // 0x00F7
    if (!o || !raw || raw->type != ValueType::BytesRef) return false;

    auto& tags = msg.mi_struct_arena;
    tags = FixedPayload<ZHC_MI_STRUCT_CAP>{};
    lumi_buffer_to_data(raw->bytes, tags);

    const char* model = def.model ? def.model : "";
    // One slot per published key; a later tag overwrites, as in z2m's payload.
    Value voltage{}, battery{}, temperature{}, outage{}, energy{}, power{}, current{};

    if (o->specific) {
        for (std::size_t k = 0; k < sizeof(kHbTags); ++k) {
            const Value* v = tags.find(kHbKeys[k]);
            if (!v) continue;
            const double d = hb_num(*v);
            switch (kHbTags[k]) {
                case 1:
                    voltage = *v;
                    if (o->tag1_battery) battery = hb_percent(d, *o);
                    break;
                case 2:
                    if (model_is(model, {"JT-BZ-01AQ/A", "JTBZ01AQ"})) outage = hb_int(d - 1);
                    break;
                case 3:
                    // A constant 25 °C on these (z2m issues 11126, 13253).
                    if (!model_is(model, {"WXCJKG11LM", "WXCJKG12LM", "WXCJKG13LM", "MCCGQ14LM",
                                          "GZCGQ01LM", "JY-GZ-01AQ", "JYGZ01AQ", "CTP-R01"}))
                        temperature = *v;
                    break;
                case 5:
                    outage = hb_int(d - 1);
                    break;
                case 101:
                    if (model_is(model, {"ZNJLBL01LM", "ZNCLDJ12LM"})) battery = *v;
                    else if (model_is(model, {"ZNCLBL01LM"}))
                        battery = hb_float(std::floor(d / 2 * 100 + 0.5) / 100);
                    break;
                case 102:
                    if (model_is(model, {"TH-S04D"})) battery = *v;
                    break;
                case 149:
                    energy = model_is(model, {"LLKZMK12LM"}) ? hb_float(d / 1000) : *v;
                    break;
                case 150:
                    if (model_is(model, {"KD-R01D", "WS-K05E"})) voltage = hb_float(d * 0.01);
                    else if (!model_is(model, {"JTYJ-GD-01LM/BW", "JTYJGD01LM"})) voltage = hb_float(d * 0.1);
                    break;
                case 151:
                    current = model_is(model, {"LLKZMK11LM"}) ? *v : hb_float(d * 0.001);
                    break;
                case 152:
                    if (!model_is(model, {"DJT11LM"})) power = *v;
                    break;
            }
        }
    }
    // z2m lumiBattery: a separate converter after lumi_specific, so it wins.
    // It only takes truthy values.
    if (o->lb_volt_tag) {
        if (!o->lb_curve) {
            const char* pk = hb_key(o->lb_pct_tag);
            const Value* p = pk ? tags.find(pk) : nullptr;
            if (p && hb_num(*p) != 0) battery = *p;
        }
        const char* vk = hb_key(o->lb_volt_tag);
        const Value* v = vk ? tags.find(vk) : nullptr;
        if (v && hb_num(*v) != 0) {
            voltage = *v;
            if (o->lb_curve) battery = hb_percent(hb_num(*v), *o);
        }
    }

    bool any = false;
    const std::pair<const char*, const Value*> keys[] = {
        {"voltage", &voltage}, {"battery", &battery}, {"device_temperature", &temperature},
        {"power_outage_count", &outage}, {"energy", &energy}, {"power", &power},
        {"current", &current},
    };
    for (const auto& [key, v] : keys) {
        if (v->type == ValueType::None) continue;
        out.put(key, *v);
        any = true;
    }
    return any;
}

namespace {
constexpr LumiHeartbeatOpts kHeartbeatMains{
    .specific = true, .tag1_battery = false, .lb_volt_tag = 0, .lb_pct_tag = 0,
    .lb_curve = false, .min_mv = 0, .max_mv = 0,
};
constexpr LumiHeartbeatOpts kHeartbeatBattery{
    .specific = true, .tag1_battery = true, .lb_volt_tag = 0, .lb_pct_tag = 0,
    .lb_curve = false, .min_mv = 2850, .max_mv = 3000,
};
constexpr LumiHeartbeatOpts kBatteryOnly{   // lumiBattery({voltageToPercentage: {min: 2850, max: 3000}})
    .specific = false, .tag1_battery = false, .lb_volt_tag = 1, .lb_pct_tag = 1,
    .lb_curve = true, .min_mv = 2850, .max_mv = 3000,
};
}  // namespace

extern const FzConverter kFzLumiHeartbeat        = lumi_heartbeat_converter(&kHeartbeatMains);
extern const FzConverter kFzLumiHeartbeatBattery = lumi_heartbeat_converter(&kHeartbeatBattery);
extern const FzConverter kFzLumiBattery          = lumi_heartbeat_converter(&kBatteryOnly);

// ── operation_mode ──────────────────────────────────────────────────

namespace {
constexpr ::zhc::generic::ZclWriteLookup kOpModeLut[] = {
    {"control_relay", 1}, {"decoupled", 0}};
constexpr ::zhc::generic::ZclWriteLookup kOpModeBasicLut[] = {
    {"control_relay", 0x12}, {"decoupled", 0xFE}};
// control_relay on a rocker's key is that rocker's own relay; the value names
// come first so a read-back finds them.
constexpr ::zhc::generic::ZclWriteLookup kOpModeLeftLut[] = {
    {"control_left_relay", 0x12}, {"control_right_relay", 0x22}, {"decoupled", 0xFE},
    {"control_relay", 0x12}};
constexpr ::zhc::generic::ZclWriteLookup kOpModeRightLut[] = {
    {"control_left_relay", 0x12}, {"control_right_relay", 0x22}, {"decoupled", 0xFE},
    {"control_relay", 0x22}};
constexpr ::zhc::generic::ZclWriteLookup kCommandModeLut[] = {{"command", 0}, {"event", 1}};

constexpr ::zhc::generic::ZclWriteSpec kSpecOpMode{nullptr, 0x0200, 0x20, 0x115F, kOpModeLut, 2};
constexpr ::zhc::generic::ZclWriteSpec kSpecOpModeBasic{nullptr, 0xFF22, 0x20, 0x115F, kOpModeBasicLut, 2};
constexpr ::zhc::generic::ZclWriteSpec kSpecOpModeLeft{nullptr, 0xFF22, 0x20, 0x115F, kOpModeLeftLut, 4};
constexpr ::zhc::generic::ZclWriteSpec kSpecOpModeRight{nullptr, 0xFF23, 0x20, 0x115F, kOpModeRightLut, 4};
constexpr ::zhc::generic::ZclWriteSpec kSpecCommandMode{nullptr, 0x0009, 0x20, 0x115F, kCommandModeLut, 2};

constexpr const ::zhc::generic::ZclWriteSpec* kOpModeSpecs[] = {
    &kSpecOpMode, &kSpecOpModeBasic, &kSpecOpModeLeft, &kSpecOpModeRight, &kSpecCommandMode};
}  // namespace

#define ZHC_LUMI_OPMODE_TZ(var, key_str, cl_name, cl_id, spec, ep)     \
    extern const TzConverter var{                                        \
        .key         = key_str,                                          \
        .cluster     = cl_name,                                          \
        .cluster_id  = cl_id,                                            \
        .command_id  = 0x02,                                             \
        .fn          = &::zhc::generic::tz_zcl_write_attr,               \
        .user_config = &spec,                                            \
        .endpoint    = ep,                                               \
    };
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationMode,           "operation_mode",        "manuSpecificLumi", 0xFCC0, kSpecOpMode, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeLeft,       "operation_mode_left",   "manuSpecificLumi", 0xFCC0, kSpecOpMode, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeCenter,     "operation_mode_center", "manuSpecificLumi", 0xFCC0, kSpecOpMode, 2)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeRight2,     "operation_mode_right",  "manuSpecificLumi", 0xFCC0, kSpecOpMode, 2)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeRight3,     "operation_mode_right",  "manuSpecificLumi", 0xFCC0, kSpecOpMode, 3)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeTop,        "operation_mode_top",    "manuSpecificLumi", 0xFCC0, kSpecOpMode, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeBottom2,    "operation_mode_bottom", "manuSpecificLumi", 0xFCC0, kSpecOpMode, 2)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeBottom3,    "operation_mode_bottom", "manuSpecificLumi", 0xFCC0, kSpecOpMode, 3)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeUp,         "operation_mode_up",     "manuSpecificLumi", 0xFCC0, kSpecOpMode, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeDown,       "operation_mode_down",   "manuSpecificLumi", 0xFCC0, kSpecOpMode, 2)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeL1,         "operation_mode_l1",     "manuSpecificLumi", 0xFCC0, kSpecOpMode, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeL2,         "operation_mode_l2",     "manuSpecificLumi", 0xFCC0, kSpecOpMode, 2)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModePower,      "operation_mode_power",  "manuSpecificLumi", 0xFCC0, kSpecOpMode, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeBright,     "operation_mode_bright", "manuSpecificLumi", 0xFCC0, kSpecOpMode, 2)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeDim,        "operation_mode_dim",    "manuSpecificLumi", 0xFCC0, kSpecOpMode, 3)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeBasic,      "operation_mode",        "genBasic", 0x0000, kSpecOpModeBasic, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeBasicLeft,  "operation_mode_left",   "genBasic", 0x0000, kSpecOpModeLeft, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiOperationModeBasicRight, "operation_mode_right",  "genBasic", 0x0000, kSpecOpModeRight, 1)
ZHC_LUMI_OPMODE_TZ(kTzLumiCommandMode,             "operation_mode",        "manuSpecificLumi", 0xFCC0, kSpecCommandMode, 1)
#undef ZHC_LUMI_OPMODE_TZ

// A report is published under the key of the definition's own writer for its
// attribute, so a read-back lands where the write came from: 0x0200 per button
// (z2m postfixWithEndpointName / enumLookup endpointName), the others from any
// endpoint. Aqara sends these manufacturer-specific, which leaves the frame
// without a cluster name; when there is one it must be the writer's.
namespace {
// z2m lumi_specific's direct cases for a setting: 240 flip_indicator_light =
// value === 1 ? "ON" : "OFF", 513 power_outage_memory and 515 led_disabled_night
// = value === 1, "4" mode_switch by lookup, 512 on a plug button_lock =
// value === 1 ? "OFF" : "ON". The binary ones are Bool here, as ZHAC's exposes
// take them (ON = true).
bool fz_lumi_setting(const DecodedMessage& msg, const TzConverter& t,
                     const ::zhc::generic::ZclWriteSpec* s,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (s != &kSpecPowerOutageMemory && s != &kSpecLedDisabledNight && s != &kSpecFlipIndicator &&
        s != &kSpecButtonLock && s != &kSpecModeSwitch) {
        return false;
    }
    if (msg.cluster && std::strcmp(msg.cluster, t.cluster) != 0) return false;
    char id[8];
    std::snprintf(id, sizeof(id), "%u", static_cast<unsigned>(s->attr_id));
    const Value* v = msg.payload.find(id);
    if (!v) return false;
    std::uint64_t n;
    if (v->type == ValueType::Bool)      n = v->b ? 1 : 0;   // herdsman reads 0x10 as a number
    else if (v->type == ValueType::Uint) n = v->u;
    else return false;
    Value o{};
    if (s == &kSpecModeSwitch) {
        const char* label = nullptr;
        for (std::uint8_t k = 0; k < s->lookup_count && !label; ++k)
            if (s->lookup[k].value == n) label = s->lookup[k].label;
        if (!label) return false;
        o.type = ValueType::StringRef;
        o.str  = label;
    } else {
        o.type = ValueType::Bool;
        o.b    = s == &kSpecButtonLock ? n != 1 : n == 1;
    }
    out.put(t.key, o);
    return true;
}

// user_config non-null: kFzLumiSettings, the settings too.
bool fz_lumi_operation_mode(const DecodedMessage& msg, const FzConverter& self,
                            const PreparedDefinition& def, RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;
    for (std::uint8_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* t = def.to_zigbee[i];
        if (!t) continue;
        const auto* s = static_cast<const ::zhc::generic::ZclWriteSpec*>(t->user_config);
        bool ours = false;
        for (const auto* k : kOpModeSpecs) ours = ours || k == s;
        if (!ours) {
            if (self.user_config && fz_lumi_setting(msg, *t, s, out)) any = true;
            continue;
        }
        if (msg.cluster && std::strcmp(msg.cluster, t->cluster) != 0) continue;
        if (s == &kSpecOpMode && std::strcmp(t->key, "operation_mode") != 0 &&
            t->endpoint != msg.src_endpoint) {
            continue;
        }
        char id[8];
        std::snprintf(id, sizeof(id), "%u", static_cast<unsigned>(s->attr_id));
        const Value* v = msg.payload.find(id);
        if (!v || v->type != ValueType::Uint) continue;
        for (std::uint8_t k = 0; k < s->lookup_count; ++k) {
            if (s->lookup[k].value != v->u) continue;
            Value o{};
            o.type = ValueType::StringRef;
            o.str  = s->lookup[k].label;
            out.put(t->key, o);
            any = true;
            break;
        }
    }
    return any;
}
}  // namespace

extern const FzConverter kFzLumiOperationMode{
    .family            = FrameFamily::Zcl,
    .cluster           = nullptr,   // genBasic and manuSpecificLumi; checked per writer
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_operation_mode },
    .user_config       = nullptr,
};

namespace {
constexpr bool kWithSettings = true;
}  // namespace

extern const FzConverter kFzLumiSettings{
    .family            = FrameFamily::Zcl,
    .cluster           = nullptr,   // genBasic and manuSpecificLumi; checked per writer
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lumi_operation_mode },
    .user_config       = &kWithSettings,
};

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
    // z2m lumi_on_off: wall switches report button presses as genOnOff
    // on endpoints 4/5/6 — that is not relay state, skip it.
    if (msg.src_endpoint >= 4 && msg.src_endpoint <= 6) return false;

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
static_assert(kReportsLumiOnOffCount == std::size(kReportsLumiOnOff));

// reporting.onOff + reporting.deviceTemperature.
//   genOnOff         0x0006 / 0x0000  bool  0..3600s   rc 0
//   genDeviceTempCfg 0x0002 / 0x0000  s16   300..3600s rc 1
const ReportingSpec kReportsLumiOnOffDevTemp[] = {
    {1, 0x0006, 0x0000, 0x10,   0, 3600, 0, 0},
    {1, 0x0002, 0x0000, 0x29, 300, 3600, 1, 0},
};
static_assert(kReportsLumiOnOffDevTempCount == std::size(kReportsLumiOnOffDevTemp));

// reporting.onOff + reporting.currentSummDelivered (energy only — no V/I/P).
//   genOnOff   0x0006 / 0x0000  bool  0..3600s  rc 0
//   seMetering 0x0702 / 0x0000  u48   5..3600s  rc 257
const ReportingSpec kReportsLumiOnOffEnergy[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600,   0, 0},
    {1, 0x0702, 0x0000, 0x25, 5, 3600, 257, 0},
};
static_assert(kReportsLumiOnOffEnergyCount == std::size(kReportsLumiOnOffEnergy));

}  // namespace zhc::lumi
