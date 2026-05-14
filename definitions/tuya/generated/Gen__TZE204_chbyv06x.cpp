// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_chbyv06x).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_chbyv06x_alarm_ringtone_dp6[] = {
    { 0, "melody_1" },
    { 1, "melody_2" },
    { 2, "melody_3" },
    { 3, "melody_4" },
    { 4, "melody_5" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_chbyv06x_self_test_result_dp9[] = {
    { 0, "checking" },
    { 1, "success" },
    { 2, "failure" },
    { 3, "others" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_chbyv06x[] = {
    { 1, "gas", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 2, "gas_value", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "alarm_ringtone", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_chbyv06x_alarm_ringtone_dp6, sizeof(kEnum__TZE204_chbyv06x_alarm_ringtone_dp6)/sizeof(kEnum__TZE204_chbyv06x_alarm_ringtone_dp6[0]) },
    { 7, "alarm_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "self_test", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "self_test_result", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_chbyv06x_self_test_result_dp9, sizeof(kEnum__TZE204_chbyv06x_self_test_result_dp9)/sizeof(kEnum__TZE204_chbyv06x_self_test_result_dp9[0]) },
    { 10, "preheat", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_chbyv06x{ kEntries__TZE204_chbyv06x, 8 };
constexpr FzConverter kFzDp__TZE204_chbyv06x{
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
    .user_config       = &kMap__TZE204_chbyv06x,
};
const FzConverter* const kFz__TZE204_chbyv06x[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_chbyv06x,
};
constexpr TzConverter kTzDp__TZE204_chbyv06x{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_chbyv06x,
};
const TzConverter* const kTz__TZE204_chbyv06x[] = { &kTzDp__TZE204_chbyv06x };
constexpr const char* kM__TZE204_chbyv06x[] = { "TS0601" };
constexpr const char* kN__TZE204_chbyv06x[] = { "_TZE204_chbyv06x" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_chbyv06x[] = {
    {"DYGSM","DY-RQ500A"},
};
extern const PreparedDefinition kDefGen__TZE204_chbyv06x{
    .zigbee_models=kM__TZE204_chbyv06x,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_chbyv06x,.manufacturer_names_count=1,
    .model="TS0601__TZE204_chbyv06x",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_chbyv06x, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_chbyv06x)/sizeof(kWhiteLabels_Gen_TZE204_chbyv06x[0]),
    .from_zigbee=kFz__TZE204_chbyv06x,
    .from_zigbee_count=sizeof(kFz__TZE204_chbyv06x)/sizeof(kFz__TZE204_chbyv06x[0]),
    .to_zigbee=kTz__TZE204_chbyv06x,
    .to_zigbee_count=sizeof(kTz__TZE204_chbyv06x)/sizeof(kTz__TZE204_chbyv06x[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
