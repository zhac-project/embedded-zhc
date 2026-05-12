// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_agcxaw3f).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_agcxaw3f_preset_dp4[] = {
    { 0, "manual" },
    { 1, "auto" },
    { 2, "mixed" },
    { 3, "away" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_agcxaw3f_running_state_dp101[] = {
    { 1, "heat" },
    { 0, "idle" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_agcxaw3f[] = {
    { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 3, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_agcxaw3f_preset_dp4, sizeof(kEnum__TZE284_agcxaw3f_preset_dp4)/sizeof(kEnum__TZE284_agcxaw3f_preset_dp4[0]) },
    { 9, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_agcxaw3f_running_state_dp101, sizeof(kEnum__TZE284_agcxaw3f_running_state_dp101)/sizeof(kEnum__TZE284_agcxaw3f_running_state_dp101[0]) },
    { 102, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "temperature_delta", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 113, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_agcxaw3f{ kEntries__TZE284_agcxaw3f, 11 };
constexpr FzConverter kFzDp__TZE284_agcxaw3f{
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
    .user_config       = &kMap__TZE284_agcxaw3f,
};
const FzConverter* const kFz__TZE284_agcxaw3f[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_agcxaw3f,
};
constexpr TzConverter kTzDp__TZE284_agcxaw3f{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_agcxaw3f,
};
const TzConverter* const kTz__TZE284_agcxaw3f[] = { &kTzDp__TZE284_agcxaw3f };
constexpr const char* kM__TZE284_agcxaw3f[] = { "TS0601" };
constexpr const char* kN__TZE284_agcxaw3f[] = { "_TZE284_agcxaw3f" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_agcxaw3f{
    .zigbee_models=kM__TZE284_agcxaw3f,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_agcxaw3f,.manufacturer_names_count=1,
    .model="TS0601__TZE284_agcxaw3f",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_agcxaw3f,
    .from_zigbee_count=sizeof(kFz__TZE284_agcxaw3f)/sizeof(kFz__TZE284_agcxaw3f[0]),
    .to_zigbee=kTz__TZE284_agcxaw3f,
    .to_zigbee_count=sizeof(kTz__TZE284_agcxaw3f)/sizeof(kTz__TZE284_agcxaw3f[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
