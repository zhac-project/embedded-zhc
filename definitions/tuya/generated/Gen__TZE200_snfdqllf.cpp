// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_snfdqllf).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_snfdqllf_system_mode_dp4[] = {
    { 0, "cool" },
    { 1, "dry" },
    { 2, "fan_only" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_snfdqllf_fan_mode_dp5[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
    { 3, "auto" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_snfdqllf[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 30, "swing_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "local_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_snfdqllf_system_mode_dp4, sizeof(kEnum__TZE200_snfdqllf_system_mode_dp4)/sizeof(kEnum__TZE200_snfdqllf_system_mode_dp4[0]) },
    { 5, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_snfdqllf_fan_mode_dp5, sizeof(kEnum__TZE200_snfdqllf_fan_mode_dp5)/sizeof(kEnum__TZE200_snfdqllf_fan_mode_dp5[0]) },
    { 25, "sleep", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "turbo", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "quiet", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 116, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "energy", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_snfdqllf{ kEntries__TZE200_snfdqllf, 11 };
constexpr FzConverter kFzDp__TZE200_snfdqllf{
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
    .user_config       = &kMap__TZE200_snfdqllf,
};
const FzConverter* const kFz__TZE200_snfdqllf[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_snfdqllf,
};
constexpr TzConverter kTzDp__TZE200_snfdqllf{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_snfdqllf,
};
const TzConverter* const kTz__TZE200_snfdqllf[] = { &kTzDp__TZE200_snfdqllf };
constexpr const char* kM__TZE200_snfdqllf[] = { "TS0601" };
constexpr const char* kN__TZE200_snfdqllf[] = { "_TZE200_snfdqllf" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_snfdqllf{
    .zigbee_models=kM__TZE200_snfdqllf,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_snfdqllf,.manufacturer_names_count=1,
    .model="TS0601__TZE200_snfdqllf",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_snfdqllf,
    .from_zigbee_count=sizeof(kFz__TZE200_snfdqllf)/sizeof(kFz__TZE200_snfdqllf[0]),
    .to_zigbee=kTz__TZE200_snfdqllf,
    .to_zigbee_count=sizeof(kTz__TZE200_snfdqllf)/sizeof(kTz__TZE200_snfdqllf[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
