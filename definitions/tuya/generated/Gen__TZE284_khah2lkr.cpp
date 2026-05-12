// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_khah2lkr).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_khah2lkr[] = {
    { 16, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 50, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "temperature_sensor", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "high_temperature_protection_state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "low_temperature_protection_state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 109, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "temperature_return_difference", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "high_temperature_protection_setting", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "low_temperature_protection_setting", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "max_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 125, "system_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 125, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 129, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 130, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_khah2lkr{ kEntries__TZE284_khah2lkr, 15 };
constexpr FzConverter kFzDp__TZE284_khah2lkr{
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
    .user_config       = &kMap__TZE284_khah2lkr,
};
const FzConverter* const kFz__TZE284_khah2lkr[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_khah2lkr,
};
constexpr TzConverter kTzDp__TZE284_khah2lkr{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_khah2lkr,
};
const TzConverter* const kTz__TZE284_khah2lkr[] = { &kTzDp__TZE284_khah2lkr };
constexpr const char* kM__TZE284_khah2lkr[] = { "TS0601" };
constexpr const char* kN__TZE284_khah2lkr[] = { "_TZE284_khah2lkr" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_khah2lkr{
    .zigbee_models=kM__TZE284_khah2lkr,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_khah2lkr,.manufacturer_names_count=1,
    .model="TS0601__TZE284_khah2lkr",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_khah2lkr,
    .from_zigbee_count=sizeof(kFz__TZE284_khah2lkr)/sizeof(kFz__TZE284_khah2lkr[0]),
    .to_zigbee=kTz__TZE284_khah2lkr,
    .to_zigbee_count=sizeof(kTz__TZE284_khah2lkr)/sizeof(kTz__TZE284_khah2lkr[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
