// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Perenio shared converter implementations.
// z2m-source: zigbee-herdsman-converters/src/devices/perenio.ts.
#include "definitions/perenio/_shared.hpp"

#include <cstdio>

namespace zhc::devices::perenio {

namespace {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::TzConverter;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::WILDCARD_ATTR_ID;
using ::zhc::WILDCARD_CMD_ID;
using ::zhc::WILDCARD_ENDPOINT;
using ::zhc::type_bit;

constexpr std::uint16_t kPerenioManuf = 0x007B;        // CUSTOM_PERENIO
constexpr std::uint16_t kPerenioCluster = 0xFC7B;      // perenioSpecific (64635)

// ── kFzPerenioSmartPlug ────────────────────────────────────────────
//
// Pass-through decoder for the perenioSpecific cluster. Mirrors
// `fzPerenio.smart_plug` in perenio.ts:
//   attr 0  default_on_off_state (enum8) — emit "default_on_off_state" str
//           via lookup {0:"off", 1:"on", 2:"previous"}
//   attr 2  rms_current  uint16 → "rms_current"  (raw)
//   attr 3  rms_voltage  uint16 → "rms_voltage"
//   attr 4  voltage_min  uint16 → "voltage_min"
//   attr 5  voltage_max  uint16 → "voltage_max"
//   attr 10 active_power uint16 → "active_power"
//   attr 11 power_max    uint16 → "power_max"
//   attr 14 consumed_energy       uint16 → "consumed_energy"
//   attr 15 consumed_energy_limit uint16 → "consumed_energy_limit"
//   attr 24 rssi         int8/uint16 → "rssi"
//
// Each attribute reports independently; this decoder iterates the
// payload and emits whichever ones are present.
bool fz_perenio_smart_plug(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    auto emit_uint = [&](const char* attr_key, const char* out_key) {
        const Value* v = msg.payload.find(attr_key);
        if (!v) return;
        if (v->type != ValueType::Uint && v->type != ValueType::Int) return;
        Value o{}; o.type = ValueType::Uint;
        o.u = v->type == ValueType::Uint
                  ? v->u
                  : static_cast<std::uint64_t>(v->i);
        out.put(out_key, o);
        emitted = true;
    };

    // attr 0 — default_on_off_state (enum8)
    if (const Value* v = msg.payload.find("0")) {
        if (v->type == ValueType::Uint) {
            const char* label = nullptr;
            switch (v->u) {
                case 0: label = "off";      break;
                case 1: label = "on";       break;
                case 2: label = "previous"; break;
                default: break;
            }
            if (label) {
                Value o{}; o.type = ValueType::StringRef; o.str = label;
                out.put("default_on_off_state", o);
                emitted = true;
            }
        }
    }
    emit_uint("2",  "rms_current");
    emit_uint("3",  "rms_voltage");
    emit_uint("4",  "voltage_min");
    emit_uint("5",  "voltage_max");
    emit_uint("10", "active_power");
    emit_uint("11", "power_max");
    emit_uint("14", "consumed_energy");
    emit_uint("15", "consumed_energy_limit");
    // attr 24 may be int8 (rssi). Pass through as Int.
    if (const Value* v = msg.payload.find("24")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int
                      ? v->i
                      : static_cast<std::int64_t>(v->u);
            out.put("rssi", o);
            emitted = true;
        }
    }
    return emitted;
}

// ── kFzPerenioSwitchType ───────────────────────────────────────────
//
// genMultistateValue.presentValue (attr 0x0055) → switch_type string.
// Maps the perenio firmware's quirky enum values:
//   1   → "momentary_state"
//   16  → "maintained_state"
//   204 → "maintained_toggle"
//   205 → "momentary_release"
//   220 → "momentary_press"
bool fz_perenio_switch_type(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // 0x0055 = 85.
    const Value* v = msg.payload.find("85");
    if (!v || v->type != ValueType::Uint) return false;
    const char* label = nullptr;
    switch (v->u) {
        case 1:   label = "momentary_state";   break;
        case 16:  label = "maintained_state";  break;
        case 204: label = "maintained_toggle"; break;
        case 205: label = "momentary_release"; break;
        case 220: label = "momentary_press";   break;
        default: return false;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("switch_type", o);
    return true;
}

// ── kFzPerenioDiagnostic ───────────────────────────────────────────
//
// haDiagnostic (cluster 0x0B05):
//   attr 0x011C (284) lastMessageLqi  uint8  → "last_message_lqi"
//   attr 0x011D (285) lastMessageRssi int8   → "last_message_rssi"
bool fz_perenio_diagnostic(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    if (const Value* v = msg.payload.find("284")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("last_message_lqi", o);
            emitted = true;
        }
    }
    if (const Value* v = msg.payload.find("285")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int
                      ? v->i
                      : static_cast<std::int64_t>(v->u);
            out.put("last_message_rssi", o);
            emitted = true;
        }
    }
    return emitted;
}

// ── tz specs ───────────────────────────────────────────────────────

// switch_type string → presentValue uint16 (genMultistateValue 0x0055).
constexpr ::zhc::generic::ZclWriteLookup kSwitchTypeLookup[] = {
    {"momentary_state",   0x0001},
    {"maintained_state",  0x0010},
    {"maintained_toggle", 0x00CC},
    {"momentary_release", 0x00CD},
    {"momentary_press",   0x00DC},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecSwitchType{
    "switch_type", 0x0055, 0x21 /* uint16 */, 0 /* non-manu */,
    kSwitchTypeLookup, sizeof(kSwitchTypeLookup) / sizeof(kSwitchTypeLookup[0]),
};

// default_on_off_state string → enum8 attr 0 (perenioSpecific manu).
constexpr ::zhc::generic::ZclWriteLookup kDefaultStateLookup[] = {
    {"off",      0},
    {"on",       1},
    {"previous", 2},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDefaultState{
    "default_on_off_state", 0x0000, 0x30 /* enum8 */, kPerenioManuf,
    kDefaultStateLookup, sizeof(kDefaultStateLookup) / sizeof(kDefaultStateLookup[0]),
};
// Numeric uint16 limits (perenioSpecific manu).
constexpr ::zhc::generic::ZclWriteSpec kSpecVoltageMin{
    "voltage_min",           0x0004, 0x21, kPerenioManuf, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecVoltageMax{
    "voltage_max",           0x0005, 0x21, kPerenioManuf, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecPowerMax{
    "power_max",             0x000B, 0x21, kPerenioManuf, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecEnergyLimit{
    "consumed_energy_limit", 0x000F, 0x21, kPerenioManuf, nullptr, 0,
};
// alarms_reset: z2m always writes attr 1 := 0 regardless of input.
// Custom encoder — ignores `input` entirely, gates on `key`.
constexpr const char* kAlarmResetKeys[] = {
    "alarm_voltage_min",
    "alarm_voltage_max",
    "alarm_power_max",
    "alarm_consumed_energy",
};

bool tz_perenio_alarms_reset(std::string_view key,
                              const Value& /*input*/,
                              const TzConverter& self,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              std::span<std::uint8_t> out_frame,
                              std::size_t& out_size) {
    out_size = 0;
    const auto* gate = static_cast<const char*>(self.user_config);
    if (!gate || key != gate) return false;

    // Manuf-specific writeAttributes: attr 1 (uint8) := 0.
    //   fc(0x14) | manu_lo | manu_hi | tsn(0) | cmd(0x02) |
    //   attr_lo | attr_hi | type(0x20) | value(0x00)
    const std::size_t total = 1 + 2 + 1 + 1 + 2 + 1 + 1;
    if (out_frame.size() < total) return false;
    std::size_t p = 0;
    out_frame[p++] = 0x14;
    out_frame[p++] = static_cast<std::uint8_t>(kPerenioManuf & 0xFF);
    out_frame[p++] = static_cast<std::uint8_t>((kPerenioManuf >> 8) & 0xFF);
    out_frame[p++] = 0x00;     // tsn placeholder
    out_frame[p++] = 0x02;     // writeAttributes
    out_frame[p++] = 0x01;     // attr 0x0001 lo
    out_frame[p++] = 0x00;     // attr hi
    out_frame[p++] = 0x20;     // uint8
    out_frame[p++] = 0x00;     // value = 0
    out_size = total;
    return true;
}

}  // namespace

// ── Public converters ──────────────────────────────────────────────

extern const FzConverter kFzPerenioSmartPlug{
    .family            = FrameFamily::Zcl,
    .cluster           = "perenioSpecific",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_perenio_smart_plug },
    .user_config       = nullptr,
};

extern const FzConverter kFzPerenioSwitchType{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateValue",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_perenio_switch_type },
    .user_config       = nullptr,
};

extern const FzConverter kFzPerenioDiagnostic{
    .family            = FrameFamily::Zcl,
    .cluster           = "haDiagnostic",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_perenio_diagnostic },
    .user_config       = nullptr,
};

// switch_type write — genMultistateValue cluster, no manuf code.
extern const TzConverter kTzPerenioSwitchType{
    .key         = "switch_type",
    .cluster     = "genMultistateValue",
    .cluster_id  = 0x0012,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecSwitchType,
};

// perenioSpecific writes — share cluster_id / command_id; specs differ.
#define ZHC_PERENIO_TZ(var, spec_ref)                                 \
    extern const TzConverter var{                                     \
        .key         = (spec_ref).key,                                \
        .cluster     = "perenioSpecific",                             \
        .cluster_id  = kPerenioCluster,                               \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &(spec_ref),                                   \
    };
ZHC_PERENIO_TZ(kTzPerenioDefaultState, kSpecDefaultState)
ZHC_PERENIO_TZ(kTzPerenioVoltageMin,   kSpecVoltageMin)
ZHC_PERENIO_TZ(kTzPerenioVoltageMax,   kSpecVoltageMax)
ZHC_PERENIO_TZ(kTzPerenioPowerMax,     kSpecPowerMax)
ZHC_PERENIO_TZ(kTzPerenioEnergyLimit,  kSpecEnergyLimit)
#undef ZHC_PERENIO_TZ

// alarms_reset wired via custom encoder — `user_config` is the gating
// key string (one TzConverter per alarm binary so the dispatcher can
// select on `key` cleanly).
#define ZHC_PERENIO_ALARM_RESET(var, key_idx)                         \
    extern const TzConverter var{                                     \
        .key         = kAlarmResetKeys[key_idx],                      \
        .cluster     = "perenioSpecific",                             \
        .cluster_id  = kPerenioCluster,                               \
        .command_id  = 0x02,                                          \
        .fn          = &tz_perenio_alarms_reset,                      \
        .user_config = kAlarmResetKeys[key_idx],                      \
    };
ZHC_PERENIO_ALARM_RESET(kTzPerenioAlarmsResetVMin,   0)
ZHC_PERENIO_ALARM_RESET(kTzPerenioAlarmsResetVMax,   1)
ZHC_PERENIO_ALARM_RESET(kTzPerenioAlarmsResetPMax,   2)
ZHC_PERENIO_ALARM_RESET(kTzPerenioAlarmsResetEnergy, 3)
#undef ZHC_PERENIO_ALARM_RESET

}  // namespace zhc::devices::perenio
