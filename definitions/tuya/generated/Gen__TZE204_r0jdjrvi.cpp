// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_r0jdjrvi).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_r0jdjrvi_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_r0jdjrvi_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_r0jdjrvi_motor_working_mode_dp106[] = {
    { 0, "continuous" },
    { 1, "intermittently" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_r0jdjrvi[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_r0jdjrvi_state_dp1, sizeof(kEnum__TZE204_r0jdjrvi_state_dp1)/sizeof(kEnum__TZE204_r0jdjrvi_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_r0jdjrvi_reverse_direction_dp5, sizeof(kEnum__TZE204_r0jdjrvi_reverse_direction_dp5)/sizeof(kEnum__TZE204_r0jdjrvi_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "motor_working_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_r0jdjrvi_motor_working_mode_dp106, sizeof(kEnum__TZE204_r0jdjrvi_motor_working_mode_dp106)/sizeof(kEnum__TZE204_r0jdjrvi_motor_working_mode_dp106[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_r0jdjrvi{ kEntries__TZE204_r0jdjrvi, 6 };
constexpr FzConverter kFzDp__TZE204_r0jdjrvi{
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
    .user_config       = &kMap__TZE204_r0jdjrvi,
};
const FzConverter* const kFz__TZE204_r0jdjrvi[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_r0jdjrvi,
};
constexpr TzConverter kTzDp__TZE204_r0jdjrvi{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_r0jdjrvi,
};
const TzConverter* const kTz__TZE204_r0jdjrvi[] = { &kTzDp__TZE204_r0jdjrvi };
constexpr const char* kM__TZE204_r0jdjrvi[] = { "TS0601" };
constexpr const char* kN__TZE204_r0jdjrvi[] = { "_TZE204_r0jdjrvi" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_r0jdjrvi[] = {
    {"Lilistore","TS0601_lilistore"},
    {"Zemismart","ZM90E-DT250N/A400"},
    {"Nova Digital","CMR-1"},
};
extern const PreparedDefinition kDefGen__TZE204_r0jdjrvi{
    .zigbee_models=kM__TZE204_r0jdjrvi,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_r0jdjrvi,.manufacturer_names_count=1,
    .model="TS0601__TZE204_r0jdjrvi",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_r0jdjrvi, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_r0jdjrvi)/sizeof(kWhiteLabels_Gen_TZE204_r0jdjrvi[0]),
    .from_zigbee=kFz__TZE204_r0jdjrvi,
    .from_zigbee_count=sizeof(kFz__TZE204_r0jdjrvi)/sizeof(kFz__TZE204_r0jdjrvi[0]),
    .to_zigbee=kTz__TZE204_r0jdjrvi,
    .to_zigbee_count=sizeof(kTz__TZE204_r0jdjrvi)/sizeof(kTz__TZE204_r0jdjrvi[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
