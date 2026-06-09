// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared ShinaSystem (SiHAS) converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/shinasystem.ts

#include "definitions/shinasystem/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/foundation.hpp"

namespace zhc::shinasystem {

// ── kFzSihasAction ──────────────────────────────────────────────────

extern const SihasActionConfig kSihasActionPlain{ /*prefix_endpoint=*/false };
extern const SihasActionConfig kSihasActionMulti{ /*prefix_endpoint=*/true  };

namespace {

// Map ZCL onOff command id → gesture label (z2m: commandOff→"single",
// commandOn→"double", commandToggle→"long").
const char* gesture_for_cmd(std::uint16_t cmd) {
    switch (cmd) {
        case 0x00: return "single";
        case 0x01: return "double";
        case 0x02: return "long";
        default:   return nullptr;
    }
}

// Static literals for the multi-button case. Indexed by
// (endpoint-1)*3 + gesture_id where gesture_id is 0=single, 1=double,
// 2=long. Only endpoints 1..4 supported (z2m caps the SBM/SQM/MSM
// families at 4 buttons); higher endpoints fall back to the plain
// label so callers still get a usable signal.
constexpr const char* kEpActionTable[] = {
    "1_single", "1_double", "1_long",
    "2_single", "2_double", "2_long",
    "3_single", "3_double", "3_long",
    "4_single", "4_double", "4_long",
};

const char* gesture_id(std::uint16_t cmd) {
    // Returns the canonical gesture order index (0..2) as "single"/etc.
    return gesture_for_cmd(cmd);
}

int gesture_index(std::uint16_t cmd) {
    switch (cmd) {
        case 0x00: return 0;  // single
        case 0x01: return 1;  // double
        case 0x02: return 2;  // long
        default:   return -1;
    }
}

bool fz_sihas_action(const ::zhc::DecodedMessage& msg,
                     const ::zhc::FzConverter& self,
                     const ::zhc::PreparedDefinition&,
                     ::zhc::RuntimeContext&,
                     ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const int gi = gesture_index(msg.command_id);
    if (gi < 0) return false;

    const auto* cfg = static_cast<const SihasActionConfig*>(self.user_config);
    const bool prefix = cfg && cfg->prefix_endpoint;

    const char* label = nullptr;
    if (prefix && msg.src_endpoint >= 1 && msg.src_endpoint <= 4) {
        const int idx = (msg.src_endpoint - 1) * 3 + gi;
        label = kEpActionTable[idx];
    } else {
        label = gesture_id(msg.command_id);
    }
    if (!label) return false;

    ::zhc::Value v{};
    v.type = ::zhc::ValueType::StringRef;
    v.str  = label;     // points at .rodata literal — safe across return.
    out.put("action", v);
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzSihasAction{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = &fz_sihas_action },
    .user_config       = &kSihasActionPlain,
};

extern const ::zhc::FzConverter kFzSihasActionMulti{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = &fz_sihas_action },
    .user_config       = &kSihasActionMulti,
};

// ── kFzSihasPeopleCnt ───────────────────────────────────────────────
//
// genAnalogInput presentValue is a float encoded as IEEE 754 single. We
// peel off the integer count + fractional tenth to derive status:
//   value ≤ 80 → people = floor(value); status = (round(value*10) % 10).
// Status lookup: 0 → idle, 1 → in, 2 → out.

namespace {

const char* people_status(std::uint32_t tenths) {
    switch (tenths) {
        case 0: return "idle";
        case 1: return "in";
        case 2: return "out";
        default: return nullptr;
    }
}

float u32_to_float(std::uint32_t bits) {
    float f;
    std::memcpy(&f, &bits, sizeof(f));
    return f;
}

bool fz_sihas_people_cnt(const ::zhc::DecodedMessage& msg,
                         const ::zhc::FzConverter&,
                         const ::zhc::PreparedDefinition&,
                         ::zhc::RuntimeContext&,
                         ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genAnalogInput.presentValue = attr 0x0055 (decimal "85").
    const ::zhc::Value* v = msg.payload.find("85");
    if (!v) return false;

    float raw = 0.0f;
    if (v->type == ::zhc::ValueType::Float) {
        raw = static_cast<float>(v->f);
    } else if (v->type == ::zhc::ValueType::Uint) {
        raw = u32_to_float(static_cast<std::uint32_t>(v->u));
    } else if (v->type == ::zhc::ValueType::Int) {
        raw = u32_to_float(static_cast<std::uint32_t>(v->i));
    } else {
        return false;
    }

    if (raw < 0.0f || raw > 80.0f) return false;
    const std::uint32_t whole  = static_cast<std::uint32_t>(raw);
    const std::uint32_t tenths = static_cast<std::uint32_t>((raw * 10.0f) + 0.5f) % 10;
    const char* status = people_status(tenths);
    if (!status) return false;

    ::zhc::Value people{};
    people.type = ::zhc::ValueType::Uint;
    people.u    = whole;
    out.put("people", people);

    ::zhc::Value st{};
    st.type = ::zhc::ValueType::StringRef;
    st.str  = status;
    out.put("status", st);
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzSihasPeopleCnt{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genAnalogInput",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_sihas_people_cnt },
    .user_config       = nullptr,
};

// ── kFzSihasOccupancyIn / kFzSihasOccupancyOut (DMS-300Z) ───────────

namespace {

// IN sensor: msOccupancySensing 0x0000 (bitmap8) bit 0. Mirror of the
// generic fz_occupancy but emits the suffixed key `occupancy_in`.
bool fz_sihas_occupancy_in(const ::zhc::DecodedMessage& msg,
                           const ::zhc::FzConverter&,
                           const ::zhc::PreparedDefinition&,
                           ::zhc::RuntimeContext&,
                           ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("0");   // attr 0x0000
    if (!v || v->type != ::zhc::ValueType::Uint) return false;
    ::zhc::Value o{};
    o.type = ::zhc::ValueType::Bool;
    o.b    = (v->u & 0x01u) != 0;
    out.put("occupancy_in", o);
    return true;
}

// OUT sensor: ssIasZone commandStatusChangeNotification, zoneStatus
// (u16 LE) bit 0. Mirror of the generic IAS status-change decode but
// emits the single suffixed key `occupancy_out`.
bool fz_sihas_occupancy_out(const ::zhc::DecodedMessage& msg,
                            const ::zhc::FzConverter&,
                            const ::zhc::PreparedDefinition&,
                            ::zhc::RuntimeContext&,
                            ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload after ZCL header: zoneStatus:u16 LE, extStatus:u8,
    // zoneId:u8, delay:u16. SiHAS sends no manufacturer code.
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
    ::zhc::Value o{};
    o.type = ::zhc::ValueType::Bool;
    o.b    = (status & 0x0001u) != 0;
    out.put("occupancy_out", o);
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzSihasOccupancyIn{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "msOccupancySensing",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_sihas_occupancy_in },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzSihasOccupancyOut{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    // ZoneStatusChangeNotification flows device → coord (FC bit 3 = 1).
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_sihas_occupancy_out },
    .user_config       = nullptr,
};

// ── kFzSihasGasValveState (GCM-300Z) ───────────────────────────────

namespace {

// genOnOff 0x0000 (key "0"): onOff==1 → "OPEN", 0 → "CLOSE". Emits the
// `gas_valve_state` string key (z2m fzLocal.GCM300Z_valve_status).
bool fz_sihas_gas_valve_state(const ::zhc::DecodedMessage& msg,
                              const ::zhc::FzConverter&,
                              const ::zhc::PreparedDefinition&,
                              ::zhc::RuntimeContext&,
                              ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("0");   // attr 0x0000
    if (!v) return false;
    bool open;
    if      (v->type == ::zhc::ValueType::Bool) open = v->b;
    else if (v->type == ::zhc::ValueType::Uint) open = v->u != 0;
    else if (v->type == ::zhc::ValueType::Int)  open = v->i != 0;
    else return false;
    ::zhc::Value o{};
    o.type = ::zhc::ValueType::StringRef;
    o.str  = open ? "OPEN" : "CLOSE";   // .rodata literal — safe across return.
    out.put("gas_valve_state", o);
    return true;
}

}  // namespace

extern const ::zhc::FzConverter kFzSihasGasValveState{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0, .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_sihas_gas_valve_state },
    .user_config       = nullptr,
};

// ── ZclWriteSpec set + TzConverter macros ──────────────────────────

namespace {

// Lookups for enum-style writes that come in as strings.
constexpr ::zhc::generic::ZclWriteLookup kLkOpMode[] = {
    {"auto",  0}, {"push",  1}, {"latch", 2},
};
constexpr ::zhc::generic::ZclWriteLookup kLkRfPairing[] = {
    {"none", 0}, {"l1", 1}, {"l2", 2}, {"l3", 3},
};
constexpr ::zhc::generic::ZclWriteLookup kLkOverheat[] = {
    {"normal", 0}, {"overheat", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLkVolume[] = {
    {"voice", 1}, {"high", 2}, {"low", 3},
};
constexpr ::zhc::generic::ZclWriteLookup kLkDiStatus[] = {
    {"Close", 0}, {"Open", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLkDiType[] = {
    {"Button", 0}, {"Door", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLkButtonLock[] = {
    {"unlock", 0}, {"lock", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLk3way[] = {
    {"disable", 0}, {"enable", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLkValveStatus[] = {
    {"close", 0}, {"open", 1},
};

// Specs.  attr_id, attr_type (0x20=u8/enum8, 0x21=u16, 0x10=bool), manuf=0.
constexpr ::zhc::generic::ZclWriteSpec kSpecOperationMode{
    "operation_mode", 0x9000, 0x20, 0,
    kLkOpMode, sizeof(kLkOpMode)/sizeof(kLkOpMode[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRfPairing{
    "rf_pairing", 0x9001, 0x20, 0,
    kLkRfPairing, sizeof(kLkRfPairing)/sizeof(kLkRfPairing[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecScheduleTime{
    "schedule_time", 0x9002, 0x20, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecValveStatus{
    "valve_status", 0x9003, 0x10, 0,
    kLkValveStatus, sizeof(kLkValveStatus)/sizeof(kLkValveStatus[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOverheatMode{
    "overheat_mode", 0x9005, 0x20, 0,
    kLkOverheat, sizeof(kLkOverheat)/sizeof(kLkOverheat[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCloseTimeout{
    "close_timeout", 0x9006, 0x21, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCloseRemainTimeout{
    "close_remain_timeout", 0x9007, 0x21, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecVolume{
    "volume", 0x9008, 0x20, 0,
    kLkVolume, sizeof(kLkVolume)/sizeof(kLkVolume[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDiStatus{
    "di_status", 0x9009, 0x20, 0,
    kLkDiStatus, sizeof(kLkDiStatus)/sizeof(kLkDiStatus[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDiType{
    "di_type", 0x900a, 0x20, 0,
    kLkDiType, sizeof(kLkDiType)/sizeof(kLkDiType[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDoPulseTime{
    "do_pulse_time", 0x900b, 0x21, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecButtonLockMode{
    "button_lock_mode", 0x900e, 0x20, 0,
    kLkButtonLock, sizeof(kLkButtonLock)/sizeof(kLkButtonLock[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecSwitch3Way{
    "switch_3way_mode", 0x900f, 0x20, 0,
    kLk3way, sizeof(kLk3way)/sizeof(kLk3way[0]),
};

}  // namespace

#define ZHC_SHINA_TZ(var, spec_ref, key_str, cluster_str, cluster_id_val)  \
    extern const ::zhc::TzConverter var{                                   \
        .key         = key_str,                                            \
        .cluster     = cluster_str,                                        \
        .cluster_id  = cluster_id_val,                                     \
        .command_id  = 0x02,                                               \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                 \
        .user_config = &spec_ref,                                          \
    };

ZHC_SHINA_TZ(kTzSihasOperationMode,      kSpecOperationMode,      "operation_mode",      "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasRfPairing,          kSpecRfPairing,          "rf_pairing",          "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasOverheatMode,       kSpecOverheatMode,       "overheat_mode",       "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasCloseTimeout,       kSpecCloseTimeout,       "close_timeout",       "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasCloseRemainTimeout, kSpecCloseRemainTimeout, "close_remain_timeout","genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasVolume,             kSpecVolume,             "volume",              "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasDiStatus,           kSpecDiStatus,           "di_status",           "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasDiType,             kSpecDiType,             "di_type",             "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasDoPulseTime,        kSpecDoPulseTime,        "do_pulse_time",       "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasButtonLockMode,     kSpecButtonLockMode,     "button_lock_mode",    "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasSwitch3WayMode,     kSpecSwitch3Way,         "switch_3way_mode",    "genOnOff",       0x0006)
ZHC_SHINA_TZ(kTzSihasScheduleTime,       kSpecScheduleTime,       "schedule_time",       "hvacThermostat", 0x0201)
ZHC_SHINA_TZ(kTzSihasValveStatus,        kSpecValveStatus,        "valve_status",        "hvacThermostat", 0x0201)

#undef ZHC_SHINA_TZ

// ── kTzSihasGasValveState (GCM-300Z) ───────────────────────────────
//
// Writes a genOnOff command (0x01 = on/OPEN, 0x00 = off/CLOSE) from the
// `gas_valve_state` key. Accepts the "OPEN"/"CLOSE" strings z2m sends as
// well as the bool/uint forms the SPA's attr-set path produces. Mirrors
// z2m tzLocal.GCM300Z_valve_status. FC 0x11 = client→server, default
// response disabled (same convention as the generic onOff command).
namespace {

bool tz_sihas_gas_valve_state(std::string_view key, const ::zhc::Value& input,
                              const ::zhc::TzConverter&,
                              const ::zhc::PreparedDefinition&,
                              ::zhc::RuntimeContext&,
                              std::span<std::uint8_t> out_frame,
                              std::size_t& out_size) {
    out_size = 0;
    if (key != "gas_valve_state") return false;

    std::uint8_t cmd;
    if (input.type == ::zhc::ValueType::Bool) {
        cmd = input.b ? 0x01 : 0x00;
    } else if (input.type == ::zhc::ValueType::Uint) {
        cmd = input.u ? 0x01 : 0x00;
    } else if (input.type == ::zhc::ValueType::Int) {
        cmd = input.i != 0 ? 0x01 : 0x00;
    } else if (input.type == ::zhc::ValueType::StringRef && input.str) {
        if      (std::strcmp(input.str, "OPEN")  == 0) cmd = 0x01;
        else if (std::strcmp(input.str, "CLOSE") == 0) cmd = 0x00;
        else return false;
    } else {
        return false;
    }

    if (out_frame.size() < 3) return false;
    out_frame[0] = 0x11;     // FC: client→server, disable default response.
    out_frame[1] = 0x00;     // TSN — platform adapter patches.
    out_frame[2] = cmd;      // genOnOff command id (0x00 off / 0x01 on).
    out_size = 3;
    return true;
}

}  // namespace

extern const ::zhc::TzConverter kTzSihasGasValveState{
    .key         = "gas_valve_state",
    .cluster     = "genOnOff",
    .cluster_id  = 0x0006,
    .command_id  = 0x00,    // encoded per-request (on/off).
    .fn          = &tz_sihas_gas_valve_state,
    .user_config = nullptr,
};

}  // namespace zhc::shinasystem
