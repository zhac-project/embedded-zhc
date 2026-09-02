// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: PushOk Hardware POK001 — battery powered retrofit valve.
// Graduated 2026-09-02 from generated/ for z2m v26.103.0 (#13062): the valve
// status lookup grew MOVING / STUCK / OFFLINE_CLOSE / OFFLINE_OPEN / BLOCKED
// and the device gained `offline_action` and `inverted`. The generated copy
// carried only state + battery, so the vendor channels are wired here. They
// all ride `presentValue` (0x0055) of a ZCL "general I/O" cluster:
//
//   status        genMultistateInput  0x0012  u16 lookup        read
//   inverted      genBinaryOutput     0x0010  bool              read + write
//   kamikaze      genBinaryValue      0x0011  bool  (OFF/ON)    read + write
//   battery_type  genMultistateOutput 0x0013  u16 lookup        read + write
//   stall_time    genMultistateValue  0x0014  u16, seconds      read + write
//
// NOT wired — both are SINGLE-precision floats on the wire (analog clusters)
// and neither the attribute decoder nor the write encoder has a float32 path:
//   offline_action genAnalogOutput 0x000D  (NONE 1 / CLOSE_RESTORE 2 /
//                  OPEN_RESTORE 3 / CLOSE 4 / OPEN 5)
//   end_lag        genAnalogValue  0x000E  (0..15 degrees)
// Neither is exposed, so nothing looks alive that is not.
//
// z2m-source: pushok.ts #POK001.
#include "definitions/_generic/_shared.hpp"
#include "definitions/pushok/_shared.hpp"

namespace zhc::devices::pushok {
namespace {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;

// presentValue is attr 0x0055 → decimal key "85".
bool present_value(const DecodedMessage& msg, std::uint64_t& out) {
    const Value* v = msg.payload.find("85");
    if (!v) return false;
    switch (v->type) {
        case ValueType::Uint: out = v->u; return true;
        case ValueType::Int:  out = static_cast<std::uint64_t>(v->i); return true;
        case ValueType::Bool: out = v->b ? 1 : 0; return true;
        default: return false;
    }
}

struct LabelTable { const char* key; const char* const* labels; std::uint8_t count; };

bool emit_lookup(const DecodedMessage& msg, const LabelTable& t,
                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint64_t raw;
    if (!present_value(msg, raw)) return false;
    if (raw >= t.count || !t.labels[raw]) return false;
    Value o{}; o.type = ValueType::StringRef; o.str = t.labels[raw];
    out.put(t.key, o);
    return true;
}

constexpr const char* kStatusLabels[] = {
    "OFF", "ON", "MOVING", "STUCK", "OFFLINE_CLOSE", "OFFLINE_OPEN", "BLOCKED" };
constexpr const char* kOnOffLabels[]  = { "OFF", "ON" };
constexpr const char* kBatteryTypeLabels[] = { "LIION", "ALKALINE", "NIMH" };

constexpr LabelTable kStatusTable{ "status", kStatusLabels, 7 };
constexpr LabelTable kKamikazeTable{ "kamikaze", kOnOffLabels, 2 };
constexpr LabelTable kBatteryTypeTable{ "battery_type", kBatteryTypeLabels, 3 };

bool fz_status(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
               RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_lookup(msg, kStatusTable, out);
}
bool fz_kamikaze(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                 RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_lookup(msg, kKamikazeTable, out);
}
bool fz_battery_type(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                     RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_lookup(msg, kBatteryTypeTable, out);
}
bool fz_inverted(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                 RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint64_t raw;
    if (!present_value(msg, raw)) return false;
    Value o{}; o.type = ValueType::Bool; o.b = raw != 0;
    out.put("inverted", o);
    return true;
}
bool fz_stall_time(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                   RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint64_t raw;
    if (!present_value(msg, raw)) return false;
    Value o{}; o.type = ValueType::Uint; o.u = raw;
    out.put("stall_time", o);
    return true;
}

#define POK_FZ(NAME, CLUSTER, FN)                                              \
    constexpr FzConverter NAME{                                                \
        .family            = FrameFamily::Zcl,                                 \
        .cluster           = CLUSTER,                                          \
        .type_mask         = type_bit(MessageType::AttributeReport) |          \
                             type_bit(MessageType::ReadResponse),              \
        .command_id        = WILDCARD_CMD_ID,                                  \
        .attr_id           = WILDCARD_ATTR_ID,                                 \
        .endpoint          = WILDCARD_ENDPOINT,                                \
        .frame_flags_mask  = 0,                                                \
        .frame_flags_value = 0,                                                \
        .direction         = Direction::ServerToClient,                        \
        .fn                = { .zcl_fn = FN },                                 \
        .user_config       = nullptr,                                          \
    };
POK_FZ(kFzStatus,      "genMultistateInput",  fz_status)
POK_FZ(kFzInverted,    "genBinaryOutput",     fz_inverted)
POK_FZ(kFzKamikaze,    "genBinaryValue",      fz_kamikaze)
POK_FZ(kFzBatteryType, "genMultistateOutput", fz_battery_type)
POK_FZ(kFzStallTime,   "genMultistateValue",  fz_stall_time)
#undef POK_FZ

const FzConverter* const kFz_POK001[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &kFzStatus,
    &kFzInverted,
    &kFzKamikaze,
    &kFzBatteryType,
    &kFzStallTime,
};

// Writes: plain ZCL Write Attributes on presentValue. Bool inputs coerce to
// 0/1; the OFF/ON labels go through the lookup.
constexpr ::zhc::generic::ZclWriteLookup kOnOffLut[] = { {"OFF", 0}, {"ON", 1} };
constexpr ::zhc::generic::ZclWriteLookup kBatteryTypeLut[] = {
    {"LIION", 0}, {"ALKALINE", 1}, {"NIMH", 2} };
constexpr ::zhc::generic::ZclWriteSpec kSpecInverted{
    "inverted", 0x0055, 0x10, 0, kOnOffLut, 2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecKamikaze{
    "kamikaze", 0x0055, 0x10, 0, kOnOffLut, 2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecBatteryType{
    "battery_type", 0x0055, 0x21, 0, kBatteryTypeLut, 3 };
constexpr ::zhc::generic::ZclWriteSpec kSpecStallTime{
    "stall_time", 0x0055, 0x21, 0, nullptr, 0 };

constexpr TzConverter kTzInverted{
    "inverted", "genBinaryOutput", 0x0010, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecInverted };
constexpr TzConverter kTzKamikaze{
    "kamikaze", "genBinaryValue", 0x0011, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecKamikaze };
constexpr TzConverter kTzBatteryType{
    "battery_type", "genMultistateOutput", 0x0013, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecBatteryType };
constexpr TzConverter kTzStallTime{
    "stall_time", "genMultistateValue", 0x0014, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecStallTime };

const TzConverter* const kTz_POK001[] = {
    &::zhc::generic::kTzOnOff,
    &kTzInverted,
    &kTzKamikaze,
    &kTzBatteryType,
    &kTzStallTime,
};

constexpr const char* kModels_POK001[] = { "POK001" };

constexpr Expose kExposes_POK001[] = {
    {"state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"status",  ExposeType::Enum,    Access::State, nullptr, "Actual valve status", kStatusLabels, 7},
    {"inverted", ExposeType::Binary, Access::StateSet, nullptr,
     "Swap the two valve positions, for actuators fitted the other way round. "
     "Changing this does not move the valve, it only changes which position is "
     "reported as on and which command drives where", nullptr, 0, ExposeCategory::Config},
    {"kamikaze", ExposeType::Enum, Access::StateSet, nullptr,
     "Allow operation on low battery (can destroy battery)", kOnOffLabels, 2, ExposeCategory::Config},
    {"battery_type", ExposeType::Enum, Access::StateSet, nullptr, "Battery type",
     kBatteryTypeLabels, 3, ExposeCategory::Config},
    {"stall_time", ExposeType::Numeric, Access::StateSet, "s", "Timeout for state transition",
     nullptr, 0, ExposeCategory::Config, 0, 60, 1},
};

constexpr BindingSpec kBindings_POK001[] = {
    {1, 0x0006},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_POK001{
    .zigbee_models=kModels_POK001, .zigbee_models_count=sizeof(kModels_POK001)/sizeof(kModels_POK001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK001", .vendor="PushOk Hardware",
    .meta=nullptr, .exposes=kExposes_POK001, .exposes_count=sizeof(kExposes_POK001)/sizeof(kExposes_POK001[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK001, .from_zigbee_count=sizeof(kFz_POK001)/sizeof(kFz_POK001[0]),
    .to_zigbee=kTz_POK001, .to_zigbee_count=sizeof(kTz_POK001)/sizeof(kTz_POK001[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_POK001, .bindings_count=sizeof(kBindings_POK001)/sizeof(kBindings_POK001[0]),
};

}  // namespace zhc::devices::pushok
