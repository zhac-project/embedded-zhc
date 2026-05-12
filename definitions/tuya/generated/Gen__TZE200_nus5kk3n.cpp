// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_nus5kk3n).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nus5kk3n_self_test_result_dp9[] = {
    { 0, "checking" },
    { 1, "success" },
    { 2, "failure" },
    { 3, "others" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_nus5kk3n[] = {
    { 1, "gas", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 9, "self_test_result", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nus5kk3n_self_test_result_dp9, sizeof(kEnum__TZE200_nus5kk3n_self_test_result_dp9)/sizeof(kEnum__TZE200_nus5kk3n_self_test_result_dp9[0]) },
    { 11, "fault_alarm", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_nus5kk3n{ kEntries__TZE200_nus5kk3n, 3 };
constexpr FzConverter kFzDp__TZE200_nus5kk3n{
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
    .user_config       = &kMap__TZE200_nus5kk3n,
};
const FzConverter* const kFz__TZE200_nus5kk3n[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_nus5kk3n,
};
constexpr TzConverter kTzDp__TZE200_nus5kk3n{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_nus5kk3n,
};
const TzConverter* const kTz__TZE200_nus5kk3n[] = { &kTzDp__TZE200_nus5kk3n };
constexpr const char* kM__TZE200_nus5kk3n[] = { "TS0601" };
constexpr const char* kN__TZE200_nus5kk3n[] = { "_TZE200_nus5kk3n" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_nus5kk3n{
    .zigbee_models=kM__TZE200_nus5kk3n,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_nus5kk3n,.manufacturer_names_count=1,
    .model="TS0601__TZE200_nus5kk3n",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_nus5kk3n,
    .from_zigbee_count=sizeof(kFz__TZE200_nus5kk3n)/sizeof(kFz__TZE200_nus5kk3n[0]),
    .to_zigbee=kTz__TZE200_nus5kk3n,
    .to_zigbee_count=sizeof(kTz__TZE200_nus5kk3n)/sizeof(kTz__TZE200_nus5kk3n[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
