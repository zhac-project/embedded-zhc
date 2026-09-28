// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Moes BHT-002 / BHT-006 thermostat — all nine TS0601 IDs of z2m's
// BHT-002, one definition per scaling group (a definition carries one DP map).
// z2m-source: moes.ts #BHT-002 (v26.105.0): legacy.fz.moes_thermostat,
// legacy.tz.moes_thermostat_*, tuyaBase({forceTimeUpdates, timeStart: "1970"}).
//
//   DP   key                            z2m
//    1   state                          system_mode heat/off (kept as the on/off bool)
//    2   preset                         moesHold: truthy = program, else hold
//    3   preset                         moesScheduleEnable: truthy = hold (inverted)
//   16   current_heating_setpoint       ┐
//   18   max_temperature_limit          │ ÷10 on the 5toc8efa pair, raw elsewhere
//   19   max_temperature                │
//   20   deadzone_temperature           │
//   26   min_temperature_limit          ┘
//   24   local_temperature              5toc8efa ÷10; elsewhere a 16-bit sign wrap
//                                       (`v - 65536 + 1`, z2m's +1 kept), then ÷10
//                                       except ztvwu4nk / ye5jkfsb (raw); ≥ 100 dropped
//   27   local_temperature_calibration  > 4000 means v − 4096 (write: < 0 → 4096 + v)
//   36   running_state                  moesValve: truthy = idle, else heat
//   40   child_lock
//   43   sensor                         IN / AL / OU
//  101   program                        weekly schedule string (kTuyaDpFlagMoesSchedule,
//                                       3 day-groups × 4 periods "HH:MM/T.t")
//
// preset writes DP2 and DP3 in one setData frame (z2m sends two frames).
// Not ported: forceTimeUpdates (z2m pushes the time hourly), z2m's per-ID
// expose nits — setpoint step 0.5 on _TZE204_5toc8efa only (left unspecified
// on the pair, whose DP carries tenths), calibration step 0.1 (the DP is an
// integer, so 1 here), running_state `cool` (only BAC-002 reports it).
#include <array>
#include <cmath>
#include <cstring>

#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::moes {
namespace {

using ::zhc::tuya::TuyaDatapointMap;
using ::zhc::tuya::TuyaDpExpandFn;
using ::zhc::tuya::TuyaDpMapEntry;
using ::zhc::tuya::TuyaEnumEntry;
using Payload = FixedPayload<ZHC_FIXED_PAYLOAD_CAP>;

constexpr TuyaEnumEntry kSensor[]  = { {0, "IN"}, {1, "AL"}, {2, "OU"} };
constexpr TuyaEnumEntry kHold[]    = { {0, "hold"}, {1, "program"} };   // DP2
constexpr TuyaEnumEntry kSchedEn[] = { {0, "program"}, {1, "hold"} };   // DP3
constexpr TuyaEnumEntry kValve[]   = { {0, "heat"}, {1, "idle"} };      // DP36

bool local_temp(const TuyaDpMapEntry& e, const Value& raw, bool wrap, Payload& out) {
    if (raw.type != ValueType::Int) return false;
    std::int64_t v = raw.i;
    if (wrap && (v & 0x8000)) v = v - 0x10000 + 1;
    Value t{}; t.type = ValueType::Float;
    t.f = static_cast<float>(v) / static_cast<float>(e.divisor);
    if (t.f >= 100.0f) return false;
    out.put(e.out_key, t);
    return true;
}
bool local_temp_plain(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Payload& out) {
    return local_temp(e, raw, false, out);
}
bool local_temp_wrapped(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Payload& out) {
    return local_temp(e, raw, true, out);
}

bool calibration(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Payload& out) {
    if (raw.type != ValueType::Int) return false;
    Value v{}; v.type = ValueType::Int; v.i = raw.i > 4000 ? raw.i - 4096 : raw.i;
    out.put(e.out_key, v);
    return true;
}

// `div` scales DP16/18/19/20/26; DP24 gets its own divisor and decoder.
constexpr std::array<TuyaDpMapEntry, 14> entries(std::int32_t div, std::int32_t temp_div,
                                                 TuyaDpExpandFn temp) {
    using ::zhc::TuyaDpType;
    using ::zhc::tuya::kTuyaDpFlagBoolEnum;
    return {{
        { 1,   "state",                         TuyaDpType::Bool,    1,        nullptr,  0 },
        { 2,   "preset",                        TuyaDpType::Bool,    1,        kHold,    2, kTuyaDpFlagBoolEnum },
        { 3,   "preset",                        TuyaDpType::Bool,    1,        kSchedEn, 2, kTuyaDpFlagBoolEnum },
        { 16,  "current_heating_setpoint",      TuyaDpType::Numeric, div,      nullptr,  0 },
        { 18,  "max_temperature_limit",         TuyaDpType::Numeric, div,      nullptr,  0 },
        { 19,  "max_temperature",               TuyaDpType::Numeric, div,      nullptr,  0 },
        { 20,  "deadzone_temperature",          TuyaDpType::Numeric, div,      nullptr,  0 },
        { 24,  "local_temperature",             TuyaDpType::Numeric, temp_div, nullptr,  0, 0, 0.0f, temp },
        { 26,  "min_temperature_limit",         TuyaDpType::Numeric, div,      nullptr,  0 },
        { 27,  "local_temperature_calibration", TuyaDpType::Numeric, 1,        nullptr,  0, 0, 0.0f, &calibration },
        { 36,  "running_state",                 TuyaDpType::Bool,    1,        kValve,   2, kTuyaDpFlagBoolEnum },
        { 40,  "child_lock",                    TuyaDpType::Bool,    1,        nullptr,  0 },
        { 43,  "sensor",                        TuyaDpType::Enum,    1,        kSensor,  3 },
        { 101, "program",                       TuyaDpType::Raw,     1,        nullptr,  0,
               ::zhc::tuya::kTuyaDpFlagMoesSchedule },
    }};
}

constexpr auto kE_tenths_temp = entries(1,  10, &local_temp_wrapped);   // aoclfnxz, u9bfwha0
constexpr auto kE_5toc8efa    = entries(10, 10, &local_temp_plain);
constexpr auto kE_raw_temp    = entries(1,  1,  &local_temp_wrapped);   // ztvwu4nk, ye5jkfsb
constexpr TuyaDatapointMap kMap_tenths_temp{ kE_tenths_temp.data(), kE_tenths_temp.size() };
constexpr TuyaDatapointMap kMap_5toc8efa{ kE_5toc8efa.data(), kE_5toc8efa.size() };
constexpr TuyaDatapointMap kMap_raw_temp{ kE_raw_temp.data(), kE_raw_temp.size() };

constexpr FzConverter fz_dp(const TuyaDatapointMap* map) {
    return { .family = FrameFamily::TuyaDp, .cluster = "manuSpecificTuya",
             .type_mask = type_bit(MessageType::Command), .command_id = WILDCARD_CMD_ID,
             .attr_id = WILDCARD_ATTR_ID, .endpoint = WILDCARD_ENDPOINT,
             .frame_flags_mask = 0, .frame_flags_value = 0, .direction = Direction::ServerToClient,
             .fn = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints }, .user_config = map };
}
constexpr TzConverter tz_dp(const TuyaDatapointMap* map) {
    return { .key = nullptr, .cluster = "manuSpecificTuya", .cluster_id = 0xEF00,
             .command_id = 0x00, .fn = &::zhc::tuya::tz_tuya_datapoints, .user_config = map };
}

// preset → DP2 (hold 0 / program 1) and DP3 (program 0 / hold 1), both enums.
bool tz_preset(std::string_view, const Value& in, const TzConverter&, const PreparedDefinition&,
               RuntimeContext&, std::span<std::uint8_t> out, std::size_t& out_size) {
    out_size = 0;
    if (in.type != ValueType::StringRef || !in.str) return false;
    const bool program = std::strcmp(in.str, "program") == 0;
    if (!program && std::strcmp(in.str, "hold") != 0) return false;
    constexpr auto kEnum = static_cast<std::uint8_t>(::zhc::TuyaDpType::Enum);
    const std::uint8_t frame[] = {
        0x01, 0x00, 0x00, 0x00, 0x01,              // fc, tsn (platform patches), setData, seq
        2, kEnum, 0x00, 0x01, static_cast<std::uint8_t>(program ? 1 : 0),
        3, kEnum, 0x00, 0x01, static_cast<std::uint8_t>(program ? 0 : 1),
    };
    if (out.size() < sizeof(frame)) return false;
    std::memcpy(out.data(), frame, sizeof(frame));
    out_size = sizeof(frame);
    return true;
}

// Negative offsets go out as 4096 + value; the DP27 row does the framing.
constexpr TuyaDpMapEntry kCalibEntry[] = {
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
};
constexpr TuyaDatapointMap kCalibMap{ kCalibEntry, sizeof(kCalibEntry)/sizeof(kCalibEntry[0]) };
bool tz_calibration(std::string_view key, const Value& in, const TzConverter& self,
                    const PreparedDefinition& def, RuntimeContext& ctx,
                    std::span<std::uint8_t> out, std::size_t& out_size) {
    double v = 0;
    if      (in.type == ValueType::Int)   v = static_cast<double>(in.i);
    else if (in.type == ValueType::Uint)  v = static_cast<double>(in.u);
    else if (in.type == ValueType::Float) v = in.f;
    else return false;
    Value w{}; w.type = ValueType::Int; w.i = std::llround(v);
    if (w.i < 0) w.i += 4096;
    return ::zhc::tuya::tz_tuya_datapoints(key, w, self, def, ctx, out, out_size);
}

constexpr TzConverter kTzPreset{
    .key = "preset", .cluster = "manuSpecificTuya", .cluster_id = 0xEF00,
    .command_id = 0x00, .fn = &tz_preset, .user_config = nullptr };
constexpr TzConverter kTzCalibration{
    .key = "local_temperature_calibration", .cluster = "manuSpecificTuya", .cluster_id = 0xEF00,
    .command_id = 0x00, .fn = &tz_calibration, .user_config = &kCalibMap };

constexpr FzConverter kFzDp_tenths_temp = fz_dp(&kMap_tenths_temp);
constexpr FzConverter kFzDp_5toc8efa    = fz_dp(&kMap_5toc8efa);
constexpr FzConverter kFzDp_raw_temp    = fz_dp(&kMap_raw_temp);
constexpr TzConverter kTzDp_tenths_temp = tz_dp(&kMap_tenths_temp);
constexpr TzConverter kTzDp_5toc8efa    = tz_dp(&kMap_5toc8efa);
constexpr TzConverter kTzDp_raw_temp    = tz_dp(&kMap_raw_temp);

const FzConverter* const kFz_tenths_temp[] = { &::zhc::tuya::kFzTuyaMcuSyncTime, &kFzDp_tenths_temp };
const FzConverter* const kFz_5toc8efa[]    = { &::zhc::tuya::kFzTuyaMcuSyncTime, &kFzDp_5toc8efa };
const FzConverter* const kFz_raw_temp[]    = { &::zhc::tuya::kFzTuyaMcuSyncTime, &kFzDp_raw_temp };
// Keyed converters first: the DP writer would send a negative offset as s32.
const TzConverter* const kTz_tenths_temp[] = { &kTzPreset, &kTzCalibration, &kTzDp_tenths_temp };
const TzConverter* const kTz_5toc8efa[]    = { &kTzPreset, &kTzCalibration, &kTzDp_5toc8efa };
const TzConverter* const kTz_raw_temp[]    = { &kTzPreset, &kTzCalibration, &kTzDp_raw_temp };

constexpr const char* kPresetValues[]  = { "hold", "program" };
constexpr const char* kRunningValues[] = { "idle", "heat" };
constexpr const char* kSensorValues[]  = { "IN", "AL", "OU" };

// z2m ranges: setpoint 5-90, deadzone 0-5 step 1, max limit 0-80, min limit 1-5,
// calibration -30..30.
constexpr std::array<Expose, 12> exposes(std::int32_t setpoint_step) {
    using ::zhc::Access;
    using ::zhc::ExposeCategory;
    return {{
        { "current_heating_setpoint",      ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
          ExposeCategory::State, 5, 90, setpoint_step },
        { "local_temperature",             ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0 },
        { "local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
          ExposeCategory::State, -30, 30, 1 },
        { "max_temperature_limit",         ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
          ExposeCategory::State, 0, 80, 0 },
        { "min_temperature_limit",         ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
          ExposeCategory::State, 1, 5, 0 },
        { "deadzone_temperature",          ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
          ExposeCategory::State, 0, 5, 1 },
        { "child_lock",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
        { "preset",                        ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kPresetValues, 2 },
        { "sensor",                        ExposeType::Enum,    Access::StateSet, nullptr, "Select temperature sensor to use",
          kSensorValues, 3, ExposeCategory::Config },
        { "state",                         ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
        { "running_state",                 ExposeType::Enum,    Access::State,    nullptr, nullptr, kRunningValues, 2 },
        { "program",                       ExposeType::String,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    }};
}
constexpr auto kExposes        = exposes(1);
constexpr auto kExposes_tenths = exposes(0);   // 5toc8efa: the DP carries tenths

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus_tenths_temp[] = {
    "_TZE200_aoclfnxz", "_TZE204_aoclfnxz", "_TZE200_u9bfwha0", "_TZE204_u9bfwha0" };
constexpr const char* kManus_5toc8efa[] = { "_TZE200_5toc8efa", "_TZE204_5toc8efa" };
constexpr const char* kManus_raw_temp[] = { "_TZE200_ztvwu4nk", "_TZE200_ye5jkfsb", "_TZE284_ye5jkfsb" };

constexpr BindingSpec kBindings[] = { { 1, 0xEF00 } };
constexpr WhiteLabel kWhiteLabels[] = { {"Moes", "BHT-002/BHT-006"} };   // z2m: _TZE204_aoclfnxz

}  // namespace

#define ZHC_BHT_002_DEF(var, manus, fz, tz, exp, wl, wl_count)                               \
    extern const PreparedDefinition var{                                                     \
        .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),     \
        .manufacturer_name_prefix=nullptr,                                                   \
        .manufacturer_names=manus, .manufacturer_names_count=sizeof(manus)/sizeof(manus[0]), \
        .model="BHT-002", .vendor="Moes",                                                    \
        .meta=nullptr, .exposes=exp.data(), .exposes_count=exp.size(),                       \
        .white_labels=wl, .white_labels_count=wl_count,                                      \
        .from_zigbee=fz, .from_zigbee_count=sizeof(fz)/sizeof(fz[0]),                        \
        .to_zigbee=tz, .to_zigbee_count=sizeof(tz)/sizeof(tz[0]),                            \
        .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,            \
        .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),         \
        .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,                              \
        .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,                   \
        .tuya_time_start=1,                                                                  \
    };
ZHC_BHT_002_DEF(kDef_BHT_002, kManus_tenths_temp, kFz_tenths_temp, kTz_tenths_temp, kExposes,
                kWhiteLabels, sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]))
ZHC_BHT_002_DEF(kDef_BHT_002_5toc8efa, kManus_5toc8efa, kFz_5toc8efa, kTz_5toc8efa, kExposes_tenths,
                nullptr, 0)
ZHC_BHT_002_DEF(kDef_BHT_002_rawtemp, kManus_raw_temp, kFz_raw_temp, kTz_raw_temp, kExposes,
                nullptr, 0)
#undef ZHC_BHT_002_DEF

}  // namespace zhc::devices::moes
