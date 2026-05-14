// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_icka1clh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_icka1clh_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_icka1clh_motor_direction_dp5[] = {
    { 0, "forward" },
    { 1, "reverse" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_icka1clh_opening_mode_dp101[] = {
    { 0, "tilt" },
    { 1, "lift" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_icka1clh[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_icka1clh_state_dp1, sizeof(kEnum__TZE200_icka1clh_state_dp1)/sizeof(kEnum__TZE200_icka1clh_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_icka1clh_motor_direction_dp5, sizeof(kEnum__TZE200_icka1clh_motor_direction_dp5)/sizeof(kEnum__TZE200_icka1clh_motor_direction_dp5[0]) },
    { 101, "opening_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_icka1clh_opening_mode_dp101, sizeof(kEnum__TZE200_icka1clh_opening_mode_dp101)/sizeof(kEnum__TZE200_icka1clh_opening_mode_dp101[0]) },
    { 102, "factory_reset", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "set_upper_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "set_bottom_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "motor_speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_icka1clh{ kEntries__TZE200_icka1clh, 9 };
constexpr FzConverter kFzDp__TZE200_icka1clh{
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
    .user_config       = &kMap__TZE200_icka1clh,
};
const FzConverter* const kFz__TZE200_icka1clh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_icka1clh,
};
constexpr TzConverter kTzDp__TZE200_icka1clh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_icka1clh,
};
const TzConverter* const kTz__TZE200_icka1clh[] = { &kTzDp__TZE200_icka1clh };
constexpr const char* kM__TZE200_icka1clh[] = { "TS0601" };
constexpr const char* kN__TZE200_icka1clh[] = { "_TZE200_icka1clh" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_icka1clh[] = {
    {"Moes","AM43-0.45/40-ES-EB"},
};
extern const PreparedDefinition kDefGen__TZE200_icka1clh{
    .zigbee_models=kM__TZE200_icka1clh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_icka1clh,.manufacturer_names_count=1,
    .model="TS0601__TZE200_icka1clh",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_icka1clh, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_icka1clh)/sizeof(kWhiteLabels_Gen_TZE200_icka1clh[0]),
    .from_zigbee=kFz__TZE200_icka1clh,
    .from_zigbee_count=sizeof(kFz__TZE200_icka1clh)/sizeof(kFz__TZE200_icka1clh[0]),
    .to_zigbee=kTz__TZE200_icka1clh,
    .to_zigbee_count=sizeof(kTz__TZE200_icka1clh)/sizeof(kTz__TZE200_icka1clh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
