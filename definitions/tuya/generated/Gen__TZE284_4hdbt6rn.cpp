// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_4hdbt6rn).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_4hdbt6rn_over_voltage_setting_dp102[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_4hdbt6rn_under_voltage_setting_dp103[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_4hdbt6rn_over_current_setting_dp104[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_4hdbt6rn_over_power_setting_dp105[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_4hdbt6rn_temperature_setting_dp107[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_4hdbt6rn[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 13, "test1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "over_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_4hdbt6rn_over_voltage_setting_dp102, sizeof(kEnum__TZE284_4hdbt6rn_over_voltage_setting_dp102)/sizeof(kEnum__TZE284_4hdbt6rn_over_voltage_setting_dp102[0]) },
    { 103, "under_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_4hdbt6rn_under_voltage_setting_dp103, sizeof(kEnum__TZE284_4hdbt6rn_under_voltage_setting_dp103)/sizeof(kEnum__TZE284_4hdbt6rn_under_voltage_setting_dp103[0]) },
    { 104, "over_current_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_4hdbt6rn_over_current_setting_dp104, sizeof(kEnum__TZE284_4hdbt6rn_over_current_setting_dp104)/sizeof(kEnum__TZE284_4hdbt6rn_over_current_setting_dp104[0]) },
    { 105, "over_power_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_4hdbt6rn_over_power_setting_dp105, sizeof(kEnum__TZE284_4hdbt6rn_over_power_setting_dp105)/sizeof(kEnum__TZE284_4hdbt6rn_over_power_setting_dp105[0]) },
    { 107, "temperature_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_4hdbt6rn_temperature_setting_dp107, sizeof(kEnum__TZE284_4hdbt6rn_temperature_setting_dp107)/sizeof(kEnum__TZE284_4hdbt6rn_temperature_setting_dp107[0]) },
    { 114, "over_current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "temperature_threshold", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 119, "over_power_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 125, "forward_electricity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 131, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_4hdbt6rn{ kEntries__TZE284_4hdbt6rn, 16 };
constexpr FzConverter kFzDp__TZE284_4hdbt6rn{
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
    .user_config       = &kMap__TZE284_4hdbt6rn,
};
const FzConverter* const kFz__TZE284_4hdbt6rn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_4hdbt6rn,
};
constexpr TzConverter kTzDp__TZE284_4hdbt6rn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_4hdbt6rn,
};
const TzConverter* const kTz__TZE284_4hdbt6rn[] = { &kTzDp__TZE284_4hdbt6rn };
constexpr const char* kM__TZE284_4hdbt6rn[] = { "TS0601" };
constexpr const char* kN__TZE284_4hdbt6rn[] = { "_TZE284_4hdbt6rn" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_4hdbt6rn[] = {
    {"Tongou","TOSA1-01WXJAT2A"},
};
extern const PreparedDefinition kDefGen__TZE284_4hdbt6rn{
    .zigbee_models=kM__TZE284_4hdbt6rn,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_4hdbt6rn,.manufacturer_names_count=1,
    .model="TS0601__TZE284_4hdbt6rn",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE284_4hdbt6rn, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_4hdbt6rn)/sizeof(kWhiteLabels_Gen_TZE284_4hdbt6rn[0]),
    .from_zigbee=kFz__TZE284_4hdbt6rn,
    .from_zigbee_count=sizeof(kFz__TZE284_4hdbt6rn)/sizeof(kFz__TZE284_4hdbt6rn[0]),
    .to_zigbee=kTz__TZE284_4hdbt6rn,
    .to_zigbee_count=sizeof(kTz__TZE284_4hdbt6rn)/sizeof(kTz__TZE284_4hdbt6rn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
