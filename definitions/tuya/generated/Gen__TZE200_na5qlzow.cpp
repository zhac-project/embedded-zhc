// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_na5qlzow).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_na5qlzow_sensitivity_dp9[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_na5qlzow_keep_time_dp10[] = {
    { 0, "10" },
    { 1, "30" },
    { 2, "60" },
    { 3, "120" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_na5qlzow[] = {
    { 1, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_na5qlzow_sensitivity_dp9, sizeof(kEnum__TZE200_na5qlzow_sensitivity_dp9)/sizeof(kEnum__TZE200_na5qlzow_sensitivity_dp9[0]) },
    { 10, "keep_time", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_na5qlzow_keep_time_dp10, sizeof(kEnum__TZE200_na5qlzow_keep_time_dp10)/sizeof(kEnum__TZE200_na5qlzow_keep_time_dp10[0]) },
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance_interval", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_na5qlzow{ kEntries__TZE200_na5qlzow, 7 };
constexpr FzConverter kFzDp__TZE200_na5qlzow{
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
    .user_config       = &kMap__TZE200_na5qlzow,
};
const FzConverter* const kFz__TZE200_na5qlzow[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_na5qlzow,
};
constexpr TzConverter kTzDp__TZE200_na5qlzow{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_na5qlzow,
};
const TzConverter* const kTz__TZE200_na5qlzow[] = { &kTzDp__TZE200_na5qlzow };
constexpr const char* kM__TZE200_na5qlzow[] = { "TS0601" };
constexpr const char* kN__TZE200_na5qlzow[] = { "_TZE200_na5qlzow" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_na5qlzow{
    .zigbee_models=kM__TZE200_na5qlzow,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_na5qlzow,.manufacturer_names_count=1,
    .model="TS0601__TZE200_na5qlzow",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_na5qlzow,
    .from_zigbee_count=sizeof(kFz__TZE200_na5qlzow)/sizeof(kFz__TZE200_na5qlzow[0]),
    .to_zigbee=kTz__TZE200_na5qlzow,
    .to_zigbee_count=sizeof(kTz__TZE200_na5qlzow)/sizeof(kTz__TZE200_na5qlzow[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
