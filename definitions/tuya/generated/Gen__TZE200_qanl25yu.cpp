// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_qanl25yu).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_qanl25yu_fan_speed_dp101[] = {
    { 0, "minimum" },
    { 1, "medium" },
    { 2, "maximum" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_qanl25yu_power_on_behavior_dp11[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_qanl25yu[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "fan_speed", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_qanl25yu_fan_speed_dp101, sizeof(kEnum__TZE200_qanl25yu_fan_speed_dp101)/sizeof(kEnum__TZE200_qanl25yu_fan_speed_dp101[0]) },
    { 11, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_qanl25yu_power_on_behavior_dp11, sizeof(kEnum__TZE200_qanl25yu_power_on_behavior_dp11)/sizeof(kEnum__TZE200_qanl25yu_power_on_behavior_dp11[0]) },
    { 5, "status_indication", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_qanl25yu{ kEntries__TZE200_qanl25yu, 4 };
constexpr FzConverter kFzDp__TZE200_qanl25yu{
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
    .user_config       = &kMap__TZE200_qanl25yu,
};
const FzConverter* const kFz__TZE200_qanl25yu[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_qanl25yu,
};
constexpr TzConverter kTzDp__TZE200_qanl25yu{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_qanl25yu,
};
const TzConverter* const kTz__TZE200_qanl25yu[] = { &kTzDp__TZE200_qanl25yu };
constexpr const char* kM__TZE200_qanl25yu[] = { "TS0601" };
constexpr const char* kN__TZE200_qanl25yu[] = { "_TZE200_qanl25yu" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_qanl25yu[] = {
    {"Liwokit","Fan+Light-01"},
};
extern const PreparedDefinition kDefGen__TZE200_qanl25yu{
    .zigbee_models=kM__TZE200_qanl25yu,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_qanl25yu,.manufacturer_names_count=1,
    .model="TS0601__TZE200_qanl25yu",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_qanl25yu, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_qanl25yu)/sizeof(kWhiteLabels_Gen_TZE200_qanl25yu[0]),
    .from_zigbee=kFz__TZE200_qanl25yu,
    .from_zigbee_count=sizeof(kFz__TZE200_qanl25yu)/sizeof(kFz__TZE200_qanl25yu[0]),
    .to_zigbee=kTz__TZE200_qanl25yu,
    .to_zigbee_count=sizeof(kTz__TZE200_qanl25yu)/sizeof(kTz__TZE200_qanl25yu[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
