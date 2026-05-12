// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_4vbj3fxh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_4vbj3fxh_fan_mode_dp49[] = {
    { 0, "auto" },
    { 1, "high" },
    { 2, "medium" },
    { 3, "low" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_4vbj3fxh[] = {
    { 16, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 34, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 49, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_4vbj3fxh_fan_mode_dp49, sizeof(kEnum__TZE284_4vbj3fxh_fan_mode_dp49)/sizeof(kEnum__TZE284_4vbj3fxh_fan_mode_dp49[0]) },
    { 50, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "screen_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 107, "eco_temperature_heating", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "eco_temperature_cooling", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_4vbj3fxh{ kEntries__TZE284_4vbj3fxh, 10 };
constexpr FzConverter kFzDp__TZE284_4vbj3fxh{
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
    .user_config       = &kMap__TZE284_4vbj3fxh,
};
const FzConverter* const kFz__TZE284_4vbj3fxh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_4vbj3fxh,
};
constexpr TzConverter kTzDp__TZE284_4vbj3fxh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_4vbj3fxh,
};
const TzConverter* const kTz__TZE284_4vbj3fxh[] = { &kTzDp__TZE284_4vbj3fxh };
constexpr const char* kM__TZE284_4vbj3fxh[] = { "TS0601" };
constexpr const char* kN__TZE284_4vbj3fxh[] = { "_TZE284_4vbj3fxh" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_4vbj3fxh{
    .zigbee_models=kM__TZE284_4vbj3fxh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_4vbj3fxh,.manufacturer_names_count=1,
    .model="TS0601__TZE284_4vbj3fxh",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_4vbj3fxh,
    .from_zigbee_count=sizeof(kFz__TZE284_4vbj3fxh)/sizeof(kFz__TZE284_4vbj3fxh[0]),
    .to_zigbee=kTz__TZE284_4vbj3fxh,
    .to_zigbee_count=sizeof(kTz__TZE284_4vbj3fxh)/sizeof(kTz__TZE284_4vbj3fxh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
