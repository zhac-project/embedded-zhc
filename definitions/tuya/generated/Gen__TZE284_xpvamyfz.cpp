// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_xpvamyfz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_xpvamyfz_air_quality_dp1[] = {
    { 0, "excellent" },
    { 1, "moderate" },
    { 2, "poor" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_xpvamyfz_alarm_ringtone_dp5[] = {
    { 0, "melody_1" },
    { 1, "melody_2" },
    { 2, "OFF" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_xpvamyfz_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_xpvamyfz[] = {
    { 1, "air_quality", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_xpvamyfz_air_quality_dp1, sizeof(kEnum__TZE284_xpvamyfz_air_quality_dp1)/sizeof(kEnum__TZE284_xpvamyfz_air_quality_dp1[0]) },
    { 2, "co2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "alarm_ringtone", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_xpvamyfz_alarm_ringtone_dp5, sizeof(kEnum__TZE284_xpvamyfz_alarm_ringtone_dp5)/sizeof(kEnum__TZE284_xpvamyfz_alarm_ringtone_dp5[0]) },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_xpvamyfz_battery_state_dp14, sizeof(kEnum__TZE284_xpvamyfz_battery_state_dp14)/sizeof(kEnum__TZE284_xpvamyfz_battery_state_dp14[0]) },
    { 17, "backlight_mode", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_xpvamyfz{ kEntries__TZE284_xpvamyfz, 7 };
constexpr FzConverter kFzDp__TZE284_xpvamyfz{
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
    .user_config       = &kMap__TZE284_xpvamyfz,
};
const FzConverter* const kFz__TZE284_xpvamyfz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_xpvamyfz,
};
constexpr TzConverter kTzDp__TZE284_xpvamyfz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_xpvamyfz,
};
const TzConverter* const kTz__TZE284_xpvamyfz[] = { &kTzDp__TZE284_xpvamyfz };
constexpr const char* kM__TZE284_xpvamyfz[] = { "TS0601" };
constexpr const char* kN__TZE284_xpvamyfz[] = { "_TZE284_xpvamyfz" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_xpvamyfz{
    .zigbee_models=kM__TZE284_xpvamyfz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_xpvamyfz,.manufacturer_names_count=1,
    .model="TS0601__TZE284_xpvamyfz",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_xpvamyfz,
    .from_zigbee_count=sizeof(kFz__TZE284_xpvamyfz)/sizeof(kFz__TZE284_xpvamyfz[0]),
    .to_zigbee=kTz__TZE284_xpvamyfz,
    .to_zigbee_count=sizeof(kTz__TZE284_xpvamyfz)/sizeof(kTz__TZE284_xpvamyfz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
