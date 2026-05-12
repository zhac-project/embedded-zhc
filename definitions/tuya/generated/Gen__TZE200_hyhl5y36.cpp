// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_hyhl5y36).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_hyhl5y36_current_status_dp108[] = {
    { 0, "approaching" },
    { 1, "departing" },
    { 2, "clear" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_hyhl5y36_sensitivity_dp110[] = {
    { 3, "low" },
    { 2, "medium" },
    { 1, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_hyhl5y36_lux_mode_dp119[] = {
    { 0, "threshold" },
    { 1, "report" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_hyhl5y36_lux_report_mode_dp120[] = {
    { 0, "timed" },
    { 1, "difference" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_hyhl5y36[] = {
    { 2, "trigger_distance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "lux_difference_value", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "indicator", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "hold_delay_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "current_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_hyhl5y36_current_status_dp108, sizeof(kEnum__TZE200_hyhl5y36_current_status_dp108)/sizeof(kEnum__TZE200_hyhl5y36_current_status_dp108[0]) },
    { 109, "enable_sensor", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 110, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_hyhl5y36_sensitivity_dp110, sizeof(kEnum__TZE200_hyhl5y36_sensitivity_dp110)/sizeof(kEnum__TZE200_hyhl5y36_sensitivity_dp110[0]) },
    { 112, "status_flip", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 113, "interference_positions", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "forbidden_area", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 115, "daylight_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "single_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 118, "find_device", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 119, "lux_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_hyhl5y36_lux_mode_dp119, sizeof(kEnum__TZE200_hyhl5y36_lux_mode_dp119)/sizeof(kEnum__TZE200_hyhl5y36_lux_mode_dp119[0]) },
    { 120, "lux_report_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_hyhl5y36_lux_report_mode_dp120, sizeof(kEnum__TZE200_hyhl5y36_lux_report_mode_dp120)/sizeof(kEnum__TZE200_hyhl5y36_lux_report_mode_dp120[0]) },
    { 121, "lux_difference_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 122, "lux_timed_interval", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 123, "absence_circling_report", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 124, "absence_circling_interval", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_hyhl5y36{ kEntries__TZE200_hyhl5y36, 20 };
constexpr FzConverter kFzDp__TZE200_hyhl5y36{
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
    .user_config       = &kMap__TZE200_hyhl5y36,
};
const FzConverter* const kFz__TZE200_hyhl5y36[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_hyhl5y36,
};
constexpr TzConverter kTzDp__TZE200_hyhl5y36{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_hyhl5y36,
};
const TzConverter* const kTz__TZE200_hyhl5y36[] = { &kTzDp__TZE200_hyhl5y36 };
constexpr const char* kM__TZE200_hyhl5y36[] = { "TS0601" };
constexpr const char* kN__TZE200_hyhl5y36[] = { "_TZE200_hyhl5y36" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_hyhl5y36{
    .zigbee_models=kM__TZE200_hyhl5y36,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_hyhl5y36,.manufacturer_names_count=1,
    .model="TS0601__TZE200_hyhl5y36",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_hyhl5y36,
    .from_zigbee_count=sizeof(kFz__TZE200_hyhl5y36)/sizeof(kFz__TZE200_hyhl5y36[0]),
    .to_zigbee=kTz__TZE200_hyhl5y36,
    .to_zigbee_count=sizeof(kTz__TZE200_hyhl5y36)/sizeof(kTz__TZE200_hyhl5y36[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
