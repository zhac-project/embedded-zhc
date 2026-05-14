// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_x9axofse).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_x9axofse_preset_dp2[] = {
    { 0, "schedule" },
    { 1, "manual" },
    { 2, "away" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_x9axofse_frost_protection_dp10[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_x9axofse_working_day_dp31[] = {
    { 0, "mon_sun" },
    { 2, "separate" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_x9axofse_system_mode_dp107[] = {
    { 0, "off" },
    { 1, "auto" },
    { 2, "heat" },
    { 3, "cool" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_x9axofse_heating_stop_dp107[] = {
    { 0, "off" },
    { 1, "auto" },
    { 2, "heat" },
    { 3, "cool" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_x9axofse[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_x9axofse_preset_dp2, sizeof(kEnum__TZE200_x9axofse_preset_dp2)/sizeof(kEnum__TZE200_x9axofse_preset_dp2[0]) },
    { 8, "open_window", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 10, "frost_protection", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_x9axofse_frost_protection_dp10, sizeof(kEnum__TZE200_x9axofse_frost_protection_dp10)/sizeof(kEnum__TZE200_x9axofse_frost_protection_dp10[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 31, "working_day", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_x9axofse_working_day_dp31, sizeof(kEnum__TZE200_x9axofse_working_day_dp31)/sizeof(kEnum__TZE200_x9axofse_working_day_dp31[0]) },
    { 32, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 35, "battery_low", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 45, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "boost_timeset_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "schedule", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_x9axofse_system_mode_dp107, sizeof(kEnum__TZE200_x9axofse_system_mode_dp107)/sizeof(kEnum__TZE200_x9axofse_system_mode_dp107[0]) },
    { 107, "heating_stop", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_x9axofse_heating_stop_dp107, sizeof(kEnum__TZE200_x9axofse_heating_stop_dp107)/sizeof(kEnum__TZE200_x9axofse_heating_stop_dp107[0]) },
    { 115, "online", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 112, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 109, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 113, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 114, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 111, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_x9axofse{ kEntries__TZE200_x9axofse, 26 };
constexpr FzConverter kFzDp__TZE200_x9axofse{
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
    .user_config       = &kMap__TZE200_x9axofse,
};
const FzConverter* const kFz__TZE200_x9axofse[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_x9axofse,
};
constexpr TzConverter kTzDp__TZE200_x9axofse{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_x9axofse,
};
const TzConverter* const kTz__TZE200_x9axofse[] = { &kTzDp__TZE200_x9axofse };
constexpr const char* kM__TZE200_x9axofse[] = { "TS0601" };
constexpr const char* kN__TZE200_x9axofse[] = { "_TZE200_x9axofse" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_x9axofse[] = {
    {"Moes","ZTRV-ZX-TV01-MS"},
    {"Moes","TV01-ZB"},
    {"AlecoAir","HA-08_THERMO"},
    {"GIEX","TV06"},
    {"Moes","ZTRV-ZX-TV02"},
    {"EKF","ETT-8"},
    {"AVATTO","TRV06-1"},
    {"Tesla Smart","TSL-TRV-TV01ZG"},
    {"Tesla Smart","TSL-TRV-TV05ZG"},
    {"Unknown/id3.pl","GTZ08"},
};
extern const PreparedDefinition kDefGen__TZE200_x9axofse{
    .zigbee_models=kM__TZE200_x9axofse,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_x9axofse,.manufacturer_names_count=1,
    .model="TS0601__TZE200_x9axofse",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_x9axofse, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_x9axofse)/sizeof(kWhiteLabels_Gen_TZE200_x9axofse[0]),
    .from_zigbee=kFz__TZE200_x9axofse,
    .from_zigbee_count=sizeof(kFz__TZE200_x9axofse)/sizeof(kFz__TZE200_x9axofse[0]),
    .to_zigbee=kTz__TZE200_x9axofse,
    .to_zigbee_count=sizeof(kTz__TZE200_x9axofse)/sizeof(kTz__TZE200_x9axofse[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
