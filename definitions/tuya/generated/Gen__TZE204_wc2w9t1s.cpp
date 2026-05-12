// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_wc2w9t1s).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_wc2w9t1s_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
    { 2, "eco" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_wc2w9t1s[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_wc2w9t1s_preset_dp2, sizeof(kEnum__TZE204_wc2w9t1s_preset_dp2)/sizeof(kEnum__TZE204_wc2w9t1s_preset_dp2[0]) },
    { 10, "frost", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 65, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 66, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 67, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 68, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 69, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 70, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 71, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 112, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 116, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_wc2w9t1s{ kEntries__TZE204_wc2w9t1s, 18 };
constexpr FzConverter kFzDp__TZE204_wc2w9t1s{
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
    .user_config       = &kMap__TZE204_wc2w9t1s,
};
const FzConverter* const kFz__TZE204_wc2w9t1s[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_wc2w9t1s,
};
constexpr TzConverter kTzDp__TZE204_wc2w9t1s{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_wc2w9t1s,
};
const TzConverter* const kTz__TZE204_wc2w9t1s[] = { &kTzDp__TZE204_wc2w9t1s };
constexpr const char* kM__TZE204_wc2w9t1s[] = { "TS0601" };
constexpr const char* kN__TZE204_wc2w9t1s[] = { "_TZE204_wc2w9t1s" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_wc2w9t1s{
    .zigbee_models=kM__TZE204_wc2w9t1s,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_wc2w9t1s,.manufacturer_names_count=1,
    .model="TS0601__TZE204_wc2w9t1s",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_wc2w9t1s,
    .from_zigbee_count=sizeof(kFz__TZE204_wc2w9t1s)/sizeof(kFz__TZE204_wc2w9t1s[0]),
    .to_zigbee=kTz__TZE204_wc2w9t1s,
    .to_zigbee_count=sizeof(kTz__TZE204_wc2w9t1s)/sizeof(kTz__TZE204_wc2w9t1s[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
