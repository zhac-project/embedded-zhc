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

}  // namespace zhc::shinasystem
