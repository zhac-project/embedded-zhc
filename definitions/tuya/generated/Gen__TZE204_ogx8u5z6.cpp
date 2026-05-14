// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ogx8u5z6).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ogx8u5z6_system_mode_dp2[] = {
    { 0, "auto" },
    { 1, "heat" },
    { 2, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ogx8u5z6_running_state_dp3[] = {
    { 0, "heat" },
    { 1, "idle" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ogx8u5z6[] = {
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ogx8u5z6_system_mode_dp2, sizeof(kEnum__TZE204_ogx8u5z6_system_mode_dp2)/sizeof(kEnum__TZE204_ogx8u5z6_system_mode_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ogx8u5z6_running_state_dp3, sizeof(kEnum__TZE204_ogx8u5z6_running_state_dp3)/sizeof(kEnum__TZE204_ogx8u5z6_running_state_dp3[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "scale_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "pi_heating_demand", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ogx8u5z6{ kEntries__TZE204_ogx8u5z6, 16 };
constexpr FzConverter kFzDp__TZE204_ogx8u5z6{
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
    .user_config       = &kMap__TZE204_ogx8u5z6,
};
const FzConverter* const kFz__TZE204_ogx8u5z6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ogx8u5z6,
};
constexpr TzConverter kTzDp__TZE204_ogx8u5z6{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ogx8u5z6,
};
const TzConverter* const kTz__TZE204_ogx8u5z6[] = { &kTzDp__TZE204_ogx8u5z6 };
constexpr const char* kM__TZE204_ogx8u5z6[] = { "TS0601" };
constexpr const char* kN__TZE204_ogx8u5z6[] = { "_TZE204_ogx8u5z6" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_ogx8u5z6[] = {
    {"AVATTO","ME167"},
    {"AVATTO","ME168_1"},
    {"AVATTO","TRV06_1"},
    {"EARU","TRV06"},
    {"AVATTO","AVATTO_TRV06"},
    {"THALEOS","TRV06-AT"},
    {"Echos","Eco-4160"},
};
extern const PreparedDefinition kDefGen__TZE204_ogx8u5z6{
    .zigbee_models=kM__TZE204_ogx8u5z6,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ogx8u5z6,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ogx8u5z6",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_ogx8u5z6, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_ogx8u5z6)/sizeof(kWhiteLabels_Gen_TZE204_ogx8u5z6[0]),
    .from_zigbee=kFz__TZE204_ogx8u5z6,
    .from_zigbee_count=sizeof(kFz__TZE204_ogx8u5z6)/sizeof(kFz__TZE204_ogx8u5z6[0]),
    .to_zigbee=kTz__TZE204_ogx8u5z6,
    .to_zigbee_count=sizeof(kTz__TZE204_ogx8u5z6)/sizeof(kTz__TZE204_ogx8u5z6[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
