// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_pl31aqf5).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_pl31aqf5_air_quality_dp1[] = {
    { 0, "excellent" },
    { 1, "moderate" },
    { 2, "poor" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_pl31aqf5_alarm_ringtone_dp5[] = {
    { 0, "melody_1" },
    { 1, "melody_2" },
    { 2, "OFF" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_pl31aqf5_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_pl31aqf5[] = {
    { 1, "air_quality", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_pl31aqf5_air_quality_dp1, sizeof(kEnum__TZE200_pl31aqf5_air_quality_dp1)/sizeof(kEnum__TZE200_pl31aqf5_air_quality_dp1[0]) },
    { 2, "co2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "alarm_ringtone", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_pl31aqf5_alarm_ringtone_dp5, sizeof(kEnum__TZE200_pl31aqf5_alarm_ringtone_dp5)/sizeof(kEnum__TZE200_pl31aqf5_alarm_ringtone_dp5[0]) },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_pl31aqf5_battery_state_dp14, sizeof(kEnum__TZE200_pl31aqf5_battery_state_dp14)/sizeof(kEnum__TZE200_pl31aqf5_battery_state_dp14[0]) },
    { 17, "backlight_mode", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_pl31aqf5{ kEntries__TZE200_pl31aqf5, 7 };
constexpr FzConverter kFzDp__TZE200_pl31aqf5{
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
    .user_config       = &kMap__TZE200_pl31aqf5,
};
const FzConverter* const kFz__TZE200_pl31aqf5[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_pl31aqf5,
};
constexpr TzConverter kTzDp__TZE200_pl31aqf5{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_pl31aqf5,
};
const TzConverter* const kTz__TZE200_pl31aqf5[] = { &kTzDp__TZE200_pl31aqf5 };
constexpr const char* kM__TZE200_pl31aqf5[] = { "TS0601" };
constexpr const char* kN__TZE200_pl31aqf5[] = { "_TZE200_pl31aqf5" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_pl31aqf5[] = {
    {"Nous","E10"},
};
extern const PreparedDefinition kDefGen__TZE200_pl31aqf5{
    .zigbee_models=kM__TZE200_pl31aqf5,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_pl31aqf5,.manufacturer_names_count=1,
    .model="TS0601__TZE200_pl31aqf5",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_pl31aqf5, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_pl31aqf5)/sizeof(kWhiteLabels_Gen_TZE200_pl31aqf5[0]),
    .from_zigbee=kFz__TZE200_pl31aqf5,
    .from_zigbee_count=sizeof(kFz__TZE200_pl31aqf5)/sizeof(kFz__TZE200_pl31aqf5[0]),
    .to_zigbee=kTz__TZE200_pl31aqf5,
    .to_zigbee_count=sizeof(kTz__TZE200_pl31aqf5)/sizeof(kTz__TZE200_pl31aqf5[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
