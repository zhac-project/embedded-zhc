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

}  // namespace zhc::sinope
