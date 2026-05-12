// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_roujjevx).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_roujjevx_work_mode_dp8[] = {
    { 0, "heating" },
    { 2, "cooling" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_roujjevx_temperature_unit_dp20[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_roujjevx[] = {
    { 2, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "work_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_roujjevx_work_mode_dp8, sizeof(kEnum__TZE284_roujjevx_work_mode_dp8)/sizeof(kEnum__TZE284_roujjevx_work_mode_dp8[0]) },
    { 9, "autowork", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 20, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_roujjevx_temperature_unit_dp20, sizeof(kEnum__TZE284_roujjevx_temperature_unit_dp20)/sizeof(kEnum__TZE284_roujjevx_temperature_unit_dp20[0]) },
    { 21, "temperature_f_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 22, "temperature_c_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "temperature_c", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "temperature_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 29, "temperature_range", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 30, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 55, "cooling_delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 56, "cooling_delay_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_roujjevx{ kEntries__TZE284_roujjevx, 13 };
constexpr FzConverter kFzDp__TZE284_roujjevx{
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
    .user_config       = &kMap__TZE284_roujjevx,
};
const FzConverter* const kFz__TZE284_roujjevx[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_roujjevx,
};
constexpr TzConverter kTzDp__TZE284_roujjevx{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_roujjevx,
};
const TzConverter* const kTz__TZE284_roujjevx[] = { &kTzDp__TZE284_roujjevx };
constexpr const char* kM__TZE284_roujjevx[] = { "TS0601" };
constexpr const char* kN__TZE284_roujjevx[] = { "_TZE284_roujjevx" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_roujjevx{
    .zigbee_models=kM__TZE284_roujjevx,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_roujjevx,.manufacturer_names_count=1,
    .model="TS0601__TZE284_roujjevx",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_roujjevx,
    .from_zigbee_count=sizeof(kFz__TZE284_roujjevx)/sizeof(kFz__TZE284_roujjevx[0]),
    .to_zigbee=kTz__TZE284_roujjevx,
    .to_zigbee_count=sizeof(kTz__TZE284_roujjevx)/sizeof(kTz__TZE284_roujjevx[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
