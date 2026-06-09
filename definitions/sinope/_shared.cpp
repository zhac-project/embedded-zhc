// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Shared Sinope (mfgcode 0x119C, "Sinope") manuSpecific runtime.
// z2m-source: zigbee-herdsman-converters/src/devices/sinope.ts
//             (no separate lib/sinope.ts — sinopeExtend lives inline).

#include "definitions/sinope/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/foundation.hpp"

namespace zhc::sinope {

namespace {

constexpr std::uint16_t kSinopeMfgCode = 0x119C;

// ── lookup tables (raw → label) ───────────────────────────────────

constexpr ::zhc::generic::ZclWriteLookup kLookupKeypadLockout[] = {
    {"unlock", 0}, {"lock",   1}, {"lock1", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupTimeFormat[] = {
    {"24h", 0}, {"12h", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupSecondDisplayMode[] = {
    {"auto", 0}, {"setpoint", 1}, {"outdoor temp", 2},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupOccupancy[] = {
    {"unoccupied", 0}, {"occupied", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupBacklight[] = {
    {"on_demand", 0}, {"sensing", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupFloorControlMode[] = {
    {"ambiant", 1}, {"floor", 2},
};

// Reverse-lookup helper — returns label for a uint value, or nullptr.
const char* lookup_label(const ::zhc::generic::ZclWriteLookup* tbl,
                          std::size_t n, std::uint64_t v) {
    for (std::size_t i = 0; i < n; ++i) {
        if (tbl[i].value == v) return tbl[i].label;
    }
    return nullptr;
}

// Emit helpers (pattern-matches lumi).
void emit_str(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
               const char* key, const char* label) {
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put(key, o);
}
void emit_uint(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
                const char* key, std::uint64_t u) {
    Value o{}; o.type = ValueType::Uint; o.u = u;
    out.put(key, o);
}
void emit_int(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
               const char* key, std::int64_t i) {
    Value o{}; o.type = ValueType::Int; o.i = i;
    out.put(key, o);
}

// Look up an attribute by id-as-string (lumi convention).
const Value* attr(const DecodedMessage& msg, std::uint16_t id) {
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%u", static_cast<unsigned>(id));
    return msg.payload.find(buf);
}

}  // namespace

// ── fz_sinope_manu_specific (cluster 0xFF01) ──────────────────────

bool fz_sinope_manu_specific(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    if (const Value* v = attr(msg, 0x0002);  // keypadLockout
        v && v->type == ValueType::Uint) {
        if (const char* l = lookup_label(kLookupKeypadLockout,
                                          sizeof(kLookupKeypadLockout)/sizeof(kLookupKeypadLockout[0]),
                                          v->u)) {
            emit_str(out, "keypad_lockout", l); emitted = true;
        }
    }
    if (const Value* v = attr(msg, 0x000A);  // timeFormatToDisplay
        v && v->type == ValueType::Uint) {
        if (const char* l = lookup_label(kLookupTimeFormat,
                                          sizeof(kLookupTimeFormat)/sizeof(kLookupTimeFormat[0]),
                                          v->u)) {
            emit_str(out, "time_format", l); emitted = true;
        }
    }
    if (const Value* v = attr(msg, 0x0010);  // outdoorTempToDisplay (int16, ×100 °C)
        v) {
        std::int64_t raw = (v->type == ValueType::Int)  ? v->i
                          : (v->type == ValueType::Uint) ? static_cast<std::int64_t>(v->u)
                          : 0;
        // emit raw centi-°C; consumer divides by 100. Keeps shadow integer.
        emit_int(out, "outdoor_temp_raw", raw); emitted = true;
    }
    if (const Value* v = attr(msg, 0x0011);  // outdoorTempToDisplayTimeout
        v && v->type == ValueType::Uint) {
        emit_uint(out, "outdoor_temp_to", v->u); emitted = true;
    }
    if (const Value* v = attr(msg, 0x0012);  // secondScreenBehavior
        v && v->type == ValueType::Uint) {
        if (const char* l = lookup_label(kLookupSecondDisplayMode,
                                          sizeof(kLookupSecondDisplayMode)/sizeof(kLookupSecondDisplayMode[0]),
                                          v->u)) {
            emit_str(out, "second_display", l); emitted = true;
        }
    }
    if (const Value* v = attr(msg, 0x0050);  // connectedLoad
        v && v->type == ValueType::Uint) {
        emit_uint(out, "connected_load", v->u); emitted = true;
    }
    if (const Value* v = attr(msg, 0x0070);  // auxConnectedLoad
        v && v->type == ValueType::Uint) {
        if (v->u == 0xFFFF) emit_str(out, "aux_load", "disabled");
        else                emit_uint(out, "aux_load", v->u);
        emitted = true;
    }
    if (const Value* v = attr(msg, 0x0102);  // GFCiStatus
        v && v->type == ValueType::Uint) {
        emit_str(out, "gfci_status", v->u ? "on" : "off"); emitted = true;
    }
    if (const Value* v = attr(msg, 0x0105);  // floorControlMode (1=ambiant,2=floor)
        v && v->type == ValueType::Uint) {
        if (const char* l = lookup_label(kLookupFloorControlMode,
                                          sizeof(kLookupFloorControlMode)/sizeof(kLookupFloorControlMode[0]),
                                          v->u)) {
            emit_str(out, "floor_mode", l); emitted = true;
        }
    }
    if (const Value* v = attr(msg, 0x0107);  // floorTemperature (int16 ×100 °C)
        v) {
        std::int64_t raw = (v->type == ValueType::Int)  ? v->i
                          : (v->type == ValueType::Uint) ? static_cast<std::int64_t>(v->u)
                          : 0;
        emit_int(out, "floor_temp_raw", raw); emitted = true;
    }
    if (const Value* v = attr(msg, 0x010C);  // floorLimitStatus
        v && v->type == ValueType::Uint) {
        emit_str(out, "floor_limit", v->u ? "on" : "off"); emitted = true;
    }
    if (const Value* v = attr(msg, 0x010D);  // roomTemperature (int16 ×100 °C)
        v) {
        std::int64_t raw = (v->type == ValueType::Int)  ? v->i
                          : (v->type == ValueType::Uint) ? static_cast<std::int64_t>(v->u)
                          : 0;
        emit_int(out, "room_temp_raw", raw); emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzSinopeManu{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificSinope",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_sinope_manu_specific },
    .user_config       = nullptr,
};

// ── fz_sinope_hvac_manu (cluster 0x0201, manuSpec attr 0x040x) ─────

bool fz_sinope_hvac_manu(const DecodedMessage& msg,
                          const FzConverter&,
                          const PreparedDefinition&,
                          RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    if (const Value* v = attr(msg, 0x0400);  // sinopeOccupancy enum8
        v && v->type == ValueType::Uint) {
        if (const char* l = lookup_label(kLookupOccupancy,
                                          sizeof(kLookupOccupancy)/sizeof(kLookupOccupancy[0]),
                                          v->u)) {
            emit_str(out, "occupancy", l); emitted = true;
        }
    }
    if (const Value* v = attr(msg, 0x0401);  // sinopeMainCycleOutput uint16
        v && v->type == ValueType::Uint) {
        emit_uint(out, "main_cycle_out", v->u); emitted = true;
    }
    if (const Value* v = attr(msg, 0x0402);  // sinopeBacklight enum8
        v && v->type == ValueType::Uint) {
        if (const char* l = lookup_label(kLookupBacklight,
                                          sizeof(kLookupBacklight)/sizeof(kLookupBacklight[0]),
                                          v->u)) {
            emit_str(out, "backlight_dim", l); emitted = true;
        }
    }
    if (const Value* v = attr(msg, 0x0404);  // sinopeAuxCycleOutput uint16
        v && v->type == ValueType::Uint) {
        emit_uint(out, "aux_cycle_out", v->u); emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzSinopeHvacManu{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_sinope_hvac_manu },
    .user_config       = nullptr,
};

// ── ZclWriteSpec table — fields: key, attr_id, attr_type, mfg, lookup, lookup_count ───

namespace {

// manuSpecificSinope (cluster 0xFF01) write specs
constexpr ::zhc::generic::ZclWriteSpec kSpecKeypadLockout{
    "keypad_lockout", 0x0002, 0x30, kSinopeMfgCode,
    kLookupKeypadLockout, sizeof(kLookupKeypadLockout)/sizeof(kLookupKeypadLockout[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecTimeFormat{
    "time_format", 0x000A, 0x30, kSinopeMfgCode,
    kLookupTimeFormat, sizeof(kLookupTimeFormat)/sizeof(kLookupTimeFormat[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecSecondDisplayMode{
    "second_display_mode", 0x0012, 0x30, kSinopeMfgCode,
    kLookupSecondDisplayMode, sizeof(kLookupSecondDisplayMode)/sizeof(kLookupSecondDisplayMode[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOutdoorTemperature{
    "thermostat_outdoor_temperature", 0x0010, 0x29, kSinopeMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOutdoorTempTimeout{
    "outdoor_temperature_timeout", 0x0011, 0x21, kSinopeMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecConnectedLoad{
    "connected_load", 0x0050, 0x21, kSinopeMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecAuxConnectedLoad{
    "aux_connected_load", 0x0070, 0x21, kSinopeMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecEcoMode{
    "eco_mode", 0x0080, 0x28, kSinopeMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecFloorControlMode{
    "floor_control_mode", 0x0105, 0x30, kSinopeMfgCode,
    kLookupFloorControlMode, sizeof(kLookupFloorControlMode)/sizeof(kLookupFloorControlMode[0]),
};

// hvacThermostat (cluster 0x0201) manuSpec write specs
constexpr ::zhc::generic::ZclWriteSpec kSpecOccupancy{
    "thermostat_occupancy", 0x0400, 0x30, kSinopeMfgCode,
    kLookupOccupancy, sizeof(kLookupOccupancy)/sizeof(kLookupOccupancy[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMainCycleOutput{
    "main_cycle_output", 0x0401, 0x21, kSinopeMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecBacklightAutoDim{
    "backlight_auto_dim", 0x0402, 0x30, kSinopeMfgCode,
    kLookupBacklight, sizeof(kLookupBacklight)/sizeof(kLookupBacklight[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecAuxCycleOutput{
    "aux_cycle_output", 0x0404, 0x21, kSinopeMfgCode, nullptr, 0,
};

}  // namespace

// ── ZHC_SINOPE_TZ macro — stamps out canonical TzConverters ──────

#define ZHC_SINOPE_TZ(var, spec_ref, key_str, cluster_str, cluster_hex) \
    extern const TzConverter var{                                       \
        .key         = key_str,                                         \
        .cluster     = cluster_str,                                     \
        .cluster_id  = cluster_hex,                                     \
        .command_id  = 0x02,                                            \
        .fn          = &::zhc::generic::tz_zcl_write_attr,              \
        .user_config = &spec_ref,                                       \
    };

// manuSpecificSinope (0xFF01)
ZHC_SINOPE_TZ(kTzSinopeKeypadLockout,        kSpecKeypadLockout,        "keypad_lockout",                "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeTimeFormat,           kSpecTimeFormat,           "time_format",                   "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeSecondDisplayMode,    kSpecSecondDisplayMode,    "second_display_mode",           "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeOutdoorTemperature,   kSpecOutdoorTemperature,   "thermostat_outdoor_temperature","manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeOutdoorTempTimeout,   kSpecOutdoorTempTimeout,   "outdoor_temperature_timeout",   "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeConnectedLoad,        kSpecConnectedLoad,        "connected_load",                "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeAuxConnectedLoad,     kSpecAuxConnectedLoad,     "aux_connected_load",            "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeEcoMode,              kSpecEcoMode,              "eco_mode",                      "manuSpecificSinope", 0xFF01)
ZHC_SINOPE_TZ(kTzSinopeFloorControlMode,     kSpecFloorControlMode,     "floor_control_mode",            "manuSpecificSinope", 0xFF01)

// hvacThermostat (0x0201)
ZHC_SINOPE_TZ(kTzSinopeOccupancy,            kSpecOccupancy,            "thermostat_occupancy",          "hvacThermostat",     0x0201)
ZHC_SINOPE_TZ(kTzSinopeMainCycleOutput,      kSpecMainCycleOutput,      "main_cycle_output",             "hvacThermostat",     0x0201)
ZHC_SINOPE_TZ(kTzSinopeBacklightAutoDim,     kSpecBacklightAutoDim,     "backlight_auto_dim",            "hvacThermostat",     0x0201)
ZHC_SINOPE_TZ(kTzSinopeAuxCycleOutput,       kSpecAuxCycleOutput,       "aux_cycle_output",              "hvacThermostat",     0x0201)

#undef ZHC_SINOPE_TZ

// ── Sinopé water-valve (VA42xx) position / state ──────────────────────
//
// z2m-source: converters/fromZigbee.ts cover_position_via_brightness /
//             cover_state_via_onoff, converters/toZigbee.ts
//             cover_via_brightness.

namespace {

// genLevelCtrl currentLevel (0-255) → position percent (0-100), rounded.
int level_to_position(std::uint64_t level) {
    if (level > 255) level = 255;
    // mapNumberRange(level, 0, 255, 0, 100) with round-to-nearest.
    return static_cast<int>((level * 100 + 127) / 255);
}

bool fz_sinope_valve_position(const DecodedMessage& msg,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");  // attr 0x0000 currentLevel
    if (!v || v->type != ValueType::Uint) return false;
    const int position = level_to_position(v->u);
    emit_uint(out, "position", static_cast<std::uint64_t>(position));
    emit_str(out, "state", position > 0 ? "OPEN" : "CLOSE");
    return true;
}

bool fz_sinope_valve_state(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");  // attr 0x0000 onOff
    if (!v) return false;
    bool on;
    if (v->type == ValueType::Bool)      on = v->b;
    else if (v->type == ValueType::Uint) on = v->u != 0;
    else return false;
    emit_str(out, "state", on ? "OPEN" : "CLOSE");
    return true;
}

}  // namespace

extern const FzConverter kFzSinopeValvePosition{
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
    .fn                = { .zcl_fn = fz_sinope_valve_position },
    .user_config       = nullptr,
};

extern const FzConverter kFzSinopeValveState{
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
    .fn                = { .zcl_fn = fz_sinope_valve_state },
    .user_config       = nullptr,
};

namespace {

// Case-insensitive equality for short ASCII command words.
bool ieq(std::string_view a, const char* b) {
    std::size_t i = 0;
    for (; i < a.size() && b[i]; ++i) {
        char ca = a[i], cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca = static_cast<char>(ca - 'A' + 'a');
        if (cb >= 'A' && cb <= 'Z') cb = static_cast<char>(cb - 'A' + 'a');
        if (ca != cb) return false;
    }
    return i == a.size() && b[i] == '\0';
}

// Write valve `position` (0-100) or `state` (open|close|stop) through
// genLevelCtrl. Mirrors z2m `tz.cover_via_brightness` (key ["position",
// "state"]): numeric / open / close → moveToLevelWithOnOff (cmd 0x04)
// with level mapped 0-100 → 0-255 (open=255, close=0); stop → genLevelCtrl
// stop (cmd 0x03). Wildcard key — the fn decides which keys it honours.
bool tz_sinope_valve_position(std::string_view key, const Value& input,
                               const TzConverter&,
                               const PreparedDefinition&, RuntimeContext&,
                               std::span<std::uint8_t> out_frame,
                               std::size_t& out_size) {
    out_size = 0;

    bool is_stop = false;
    std::uint64_t pct = 0;

    if (key == "position") {
        if (input.type == ValueType::Uint)      pct = input.u;
        else if (input.type == ValueType::Int)  pct = input.i < 0 ? 0
                                                      : static_cast<std::uint64_t>(input.i);
        else return false;
    } else if (key == "state") {
        if (input.type != ValueType::StringRef || !input.str) return false;
        const std::string_view s{input.str};
        if      (ieq(s, "open"))  pct = 100;
        else if (ieq(s, "close")) pct = 0;
        else if (ieq(s, "stop"))  is_stop = true;
        else return false;
    } else {
        return false;
    }

    if (is_stop) {
        if (out_frame.size() < 3) return false;
        out_frame[0] = 0x11;  // fc: client→server, disable default rsp
        out_frame[1] = 0x00;  // TSN — adapter patches
        out_frame[2] = 0x03;  // genLevelCtrl stop
        out_size = 3;
        return true;
    }

    if (pct > 100) pct = 100;
    if (out_frame.size() < 6) return false;
    const std::uint8_t level = static_cast<std::uint8_t>((pct * 255 + 50) / 100);
    out_frame[0] = 0x11;     // fc: client→server, disable default rsp
    out_frame[1] = 0x00;     // TSN — adapter patches
    out_frame[2] = 0x04;     // moveToLevelWithOnOff
    out_frame[3] = level;
    out_frame[4] = 0x00;     // transition time LSB
    out_frame[5] = 0x00;     // transition time MSB
    out_size = 6;
    return true;
}

}  // namespace

extern const TzConverter kTzSinopeValvePosition{
    .key         = nullptr,  // claims "position" + "state" (open|close|stop)
    .cluster     = "genLevelCtrl",
    .cluster_id  = 0x0008,
    .command_id  = 0x04,     // moveToLevelWithOnOff (stop path writes 0x03 in-frame)
    .fn          = tz_sinope_valve_position,
    .user_config = nullptr,
};

// ── Sinopé tank-level monitor (LM4110ZB) ──────────────────────────────
//
// z2m-source: sinope.ts fzLocal.tank_level. genAnalogInput presentValue
// (attr 0x0055) is a dial angle; map to 0-100 % via the fixed gauge
// calibration. presentValue == -1 → empty (0 %).

namespace {

bool fz_sinope_tank_level(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");  // attr 0x0055 presentValue
    if (!v) return false;
    double x;
    if (v->type == ValueType::Float)     x = static_cast<double>(v->f);
    else if (v->type == ValueType::Int)  x = static_cast<double>(v->i);
    else if (v->type == ValueType::Uint) x = static_cast<double>(v->u);
    else return false;

    double pct;
    if (x == -1.0) {
        pct = 0.0;
    } else {
        constexpr double xMin = 110.0, xMax = 406.0, delta = 46.0;
        if (delta <= x && x <= 70.0) x = delta;
        if (0.0 <= x && x <= delta)  x = x + 360.0;
        const double y = (x - xMin) / (xMax - xMin);
        constexpr double lowerLimit = 10.0, upperLimit = 80.0;
        pct = y * (upperLimit - lowerLimit) + lowerLimit;
    }
    Value o{}; o.type = ValueType::Float;
    o.f = static_cast<float>(pct);
    out.put("tank_level", o);
    return true;
}

}  // namespace

extern const FzConverter kFzSinopeTankLevel{
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
    .fn                = { .zcl_fn = fz_sinope_tank_level },
    .user_config       = nullptr,
};

}  // namespace zhc::sinope
