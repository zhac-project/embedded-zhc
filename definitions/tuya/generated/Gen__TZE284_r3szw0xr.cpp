// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_r3szw0xr).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_r3szw0xr_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_r3szw0xr_work_state_dp3[] = {
    { 0, "opening" },
    { 1, "closing" },
    { 2, "stopped" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_r3szw0xr_dot_mode_dp104[] = {
    { 0, "single" },
    { 1, "multi" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_r3szw0xr_direction_dp11[] = {
    { 0, "normal" },
    { 1, "reversed" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_r3szw0xr_border_dp16[] = {
    { 0, "Up" },
    { 1, "Down" },
    { 2, "Delete" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_r3szw0xr[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_r3szw0xr_state_dp1, sizeof(kEnum__TZE284_r3szw0xr_state_dp1)/sizeof(kEnum__TZE284_r3szw0xr_state_dp1[0]) },
    { 3, "work_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_r3szw0xr_work_state_dp3, sizeof(kEnum__TZE284_r3szw0xr_work_state_dp3)/sizeof(kEnum__TZE284_r3szw0xr_work_state_dp3[0]) },
    { 9, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 8, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 103, "speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "dot_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_r3szw0xr_dot_mode_dp104, sizeof(kEnum__TZE284_r3szw0xr_dot_mode_dp104)/sizeof(kEnum__TZE284_r3szw0xr_dot_mode_dp104[0]) },
    { 11, "direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_r3szw0xr_direction_dp11, sizeof(kEnum__TZE284_r3szw0xr_direction_dp11)/sizeof(kEnum__TZE284_r3szw0xr_direction_dp11[0]) },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_r3szw0xr_border_dp16, sizeof(kEnum__TZE284_r3szw0xr_border_dp16)/sizeof(kEnum__TZE284_r3szw0xr_border_dp16[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_r3szw0xr{ kEntries__TZE284_r3szw0xr, 8 };
constexpr FzConverter kFzDp__TZE284_r3szw0xr{
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
    .user_config       = &kMap__TZE284_r3szw0xr,
};
const FzConverter* const kFz__TZE284_r3szw0xr[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_r3szw0xr,
};
constexpr TzConverter kTzDp__TZE284_r3szw0xr{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_r3szw0xr,
};
const TzConverter* const kTz__TZE284_r3szw0xr[] = { &kTzDp__TZE284_r3szw0xr };
constexpr const char* kM__TZE284_r3szw0xr[] = { "TS0601" };
constexpr const char* kN__TZE284_r3szw0xr[] = { "_TZE284_r3szw0xr" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_r3szw0xr{
    .zigbee_models=kM__TZE284_r3szw0xr,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_r3szw0xr,.manufacturer_names_count=1,
    .model="TS0601__TZE284_r3szw0xr",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_r3szw0xr,
    .from_zigbee_count=sizeof(kFz__TZE284_r3szw0xr)/sizeof(kFz__TZE284_r3szw0xr[0]),
    .to_zigbee=kTz__TZE284_r3szw0xr,
    .to_zigbee_count=sizeof(kTz__TZE284_r3szw0xr)/sizeof(kTz__TZE284_r3szw0xr[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
