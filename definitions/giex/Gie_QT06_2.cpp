// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Giex QT06_2 — GiEX water irrigation valve (TS0601, 5 mfr variants).
//
// The auto extract ported this Tuya-MCU (0xEF00) water valve as a bare
// kFzBattery + kFzOnOff stub, wiring ZCL clusters 0x0001/0x0006 the device
// never speaks and dropping 8 of z2m's irrigation datapoints. z2m decodes
// everything through legacy.fromZigbee.giexWaterValve (a 0xEF00 DP stream).
// Identical DP map to QT06_1; QT06_2 uses SECONDS (not minutes) for the
// irrigation target / cycle interval (cosmetic unit difference only — the
// wire values are still raw pass-through, z2m applies NO scale):
//   DP 1   mode                        bool -> "duration"/"capacity"
//   DP 2   state                       bool -> "ON"/"OFF"
//   DP 101 irrigation_start_time       String (raw "HH:MM:SS")
//   DP 102 irrigation_end_time         String (raw)
//   DP 103 cycle_irrigation_num_times  raw   (divisor 1)
//   DP 104 irrigation_target           raw   (divisor 1; seconds/litres)
//   DP 105 cycle_irrigation_interval   raw   (divisor 1; seconds)
//   DP 108 battery                     raw % (divisor 1)
//   DP 111 water_consumed              raw L (divisor 1 — z2m applies NO scale)
//   DP 114 last_irrigation_duration    String (raw)
// SUSPECT (water_consumed float divisor / per-DP scale) confirmed FALSE.
// DP 106 (currentTemperature) intentionally NOT mapped — z2m ignores it.
// QT06_2 has no per-model time converter (local timezone), so the raw
// String surfaces directly.
//
// z2m-source: devices/giex.ts #QT06_2 + lib/legacy.ts giexWaterValve.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::giex {
namespace {

// z2m: mode = value ? "capacity" : "duration" (bool DP fanned to a string
// label via kTuyaDpFlagBoolEnum, keyed 0/1).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_QT06_2_mode_dp1[] = {
    { 0, "duration" },
    { 1, "capacity" },
};
// z2m: state = value ? "ON" : "OFF" (binary expose publishes the string).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_QT06_2_state_dp2[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_QT06_2[] = {
    { 1,   "mode",                       ::zhc::TuyaDpType::Bool,    1, kEnum_QT06_2_mode_dp1,  sizeof(kEnum_QT06_2_mode_dp1)/sizeof(kEnum_QT06_2_mode_dp1[0]),  ::zhc::tuya::kTuyaDpFlagBoolEnum },
    { 2,   "state",                      ::zhc::TuyaDpType::Bool,    1, kEnum_QT06_2_state_dp2, sizeof(kEnum_QT06_2_state_dp2)/sizeof(kEnum_QT06_2_state_dp2[0]), ::zhc::tuya::kTuyaDpFlagBoolEnum },
    { 101, "irrigation_start_time",      ::zhc::TuyaDpType::String,  1, nullptr, 0, 0 },
    { 102, "irrigation_end_time",        ::zhc::TuyaDpType::String,  1, nullptr, 0, 0 },
    { 103, "cycle_irrigation_num_times", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "irrigation_target",          ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "cycle_irrigation_interval",  ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "battery",                    ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "water_consumed",             ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "last_irrigation_duration",   ::zhc::TuyaDpType::String,  1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_QT06_2{
    kEntries_QT06_2, sizeof(kEntries_QT06_2)/sizeof(kEntries_QT06_2[0]) };

constexpr FzConverter kFzDp_QT06_2{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap_QT06_2,
};
const FzConverter* const kFz_QT06_2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_QT06_2,
};
constexpr TzConverter kTzDp_QT06_2{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_QT06_2,
};
const TzConverter* const kTz_QT06_2[] = { &kTzDp_QT06_2 };

constexpr const char* kModels_QT06_2[] = { "TS0601" };
constexpr const char* kManus_QT06_2[]  = {
    "_TZE200_a7sghmms", "_TZE204_a7sghmms", "_TZE204_7ytb3h8u",
    "_TZE200_7ytb3h8u", "_TZE284_7ytb3h8u" };

// z2m exposes (QT06_2 uses seconds for target/interval).
constexpr Expose kExp_QT06_2[] = {
    { "state",                      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "mode",                       ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "cycle_irrigation_num_times", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "irrigation_start_time",      ExposeType::String,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "irrigation_end_time",        ExposeType::String,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "last_irrigation_duration",   ExposeType::String,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "water_consumed",             ExposeType::Numeric, ::zhc::Access::State,    "L", nullptr, nullptr, 0 },
    { "battery",                    ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "irrigation_target",          ExposeType::Numeric, ::zhc::Access::StateSet, "seconds or litres", nullptr, nullptr, 0 },
    { "cycle_irrigation_interval",  ExposeType::Numeric, ::zhc::Access::StateSet, "sec", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_QT06_2[] = {
    { 1, 0xEF00 },
};

constexpr WhiteLabel kWhiteLabels_QT06_2[] = {
    {"GIEX","GX02"},
};
}  // namespace

extern const PreparedDefinition kDef_QT06_2{
    .zigbee_models=kModels_QT06_2, .zigbee_models_count=sizeof(kModels_QT06_2)/sizeof(kModels_QT06_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QT06_2, .manufacturer_names_count=sizeof(kManus_QT06_2)/sizeof(kManus_QT06_2[0]),
    .model="QT06_2", .vendor="Giex",
    .meta=nullptr, .exposes=kExp_QT06_2, .exposes_count=sizeof(kExp_QT06_2)/sizeof(kExp_QT06_2[0]),
    .white_labels=kWhiteLabels_QT06_2, .white_labels_count=sizeof(kWhiteLabels_QT06_2)/sizeof(kWhiteLabels_QT06_2[0]),
    .from_zigbee=kFz_QT06_2, .from_zigbee_count=sizeof(kFz_QT06_2)/sizeof(kFz_QT06_2[0]),
    .to_zigbee=kTz_QT06_2, .to_zigbee_count=sizeof(kTz_QT06_2)/sizeof(kTz_QT06_2[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings                 = kBind_QT06_2,
    .bindings_count           = sizeof(kBind_QT06_2)/sizeof(kBind_QT06_2[0]),
};

}  // namespace zhc::devices::giex
