// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_xpvamyfz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_xpvamyfz_air_quality_dp1[] = {
    { 0, "excellent" },
    { 1, "moderate" },
    { 2, "poor" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_xpvamyfz_alarm_ringtone_dp5[] = {
    { 0, "melody_1" },
    { 1, "melody_2" },
    { 2, "OFF" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_xpvamyfz_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_xpvamyfz[] = {
    { 1, "air_quality", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_xpvamyfz_air_quality_dp1, sizeof(kEnum__TZE200_xpvamyfz_air_quality_dp1)/sizeof(kEnum__TZE200_xpvamyfz_air_quality_dp1[0]) },
    { 2, "co2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "alarm_ringtone", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_xpvamyfz_alarm_ringtone_dp5, sizeof(kEnum__TZE200_xpvamyfz_alarm_ringtone_dp5)/sizeof(kEnum__TZE200_xpvamyfz_alarm_ringtone_dp5[0]) },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_xpvamyfz_battery_state_dp14, sizeof(kEnum__TZE200_xpvamyfz_battery_state_dp14)/sizeof(kEnum__TZE200_xpvamyfz_battery_state_dp14[0]) },
    { 17, "backlight_mode", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_xpvamyfz{ kEntries__TZE200_xpvamyfz, 7 };
constexpr FzConverter kFzDp__TZE200_xpvamyfz{
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
    .user_config       = &kMap__TZE200_xpvamyfz,
};
const FzConverter* const kFz__TZE200_xpvamyfz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_xpvamyfz,
};
constexpr TzConverter kTzDp__TZE200_xpvamyfz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_xpvamyfz,
};
const TzConverter* const kTz__TZE200_xpvamyfz[] = { &kTzDp__TZE200_xpvamyfz };
constexpr const char* kM__TZE200_xpvamyfz[] = { "TS0601" };
constexpr const char* kN__TZE200_xpvamyfz[] = { "_TZE200_xpvamyfz" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_xpvamyfz_3[] = {"melody_1", "melody_2", "OFF"};
constexpr const char* kAutoOpts__TZE200_xpvamyfz_5[] = {"low", "medium", "high"};
constexpr const char* kAutoOpts__TZE200_xpvamyfz_6[] = {"excellent", "moderate", "poor"};
constexpr Expose kAutoExposes__TZE200_xpvamyfz[] = {
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"alarm_ringtone", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_xpvamyfz_3, 3},
    {"backlight_mode", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 3, 1},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_xpvamyfz_5, 3, ExposeCategory::Diagnostic},
    {"air_quality", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_xpvamyfz_6, 3},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_xpvamyfz[] = {
    {"Nous","E10"},
};
extern const PreparedDefinition kDefGen__TZE200_xpvamyfz{
    .zigbee_models=kM__TZE200_xpvamyfz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_xpvamyfz,.manufacturer_names_count=1,
    .model="TS0601__TZE200_xpvamyfz",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_xpvamyfz,.exposes_count=sizeof(kAutoExposes__TZE200_xpvamyfz)/sizeof(kAutoExposes__TZE200_xpvamyfz[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_xpvamyfz, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_xpvamyfz)/sizeof(kWhiteLabels_Gen_TZE200_xpvamyfz[0]),
    .from_zigbee=kFz__TZE200_xpvamyfz,
    .from_zigbee_count=sizeof(kFz__TZE200_xpvamyfz)/sizeof(kFz__TZE200_xpvamyfz[0]),
    .to_zigbee=kTz__TZE200_xpvamyfz,
    .to_zigbee_count=sizeof(kTz__TZE200_xpvamyfz)/sizeof(kTz__TZE200_xpvamyfz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
