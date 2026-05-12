// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_toeldckg).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_toeldckg_power_outage_memory_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "restore" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_toeldckg_auto_on_dp104[] = {
    { 0, "off" },
    { 1, "all" },
    { 1, "ch1" },
    { 2, "ch2" },
    { 3, "ch3" },
    { 4, "ch1_and_ch2" },
    { 5, "ch2_and_ch3" },
    { 6, "ch1_and_ch3" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_toeldckg_auto_off_dp105[] = {
    { 0, "off" },
    { 1, "all" },
    { 1, "ch1" },
    { 2, "ch2" },
    { 3, "ch3" },
    { 4, "ch1_and_ch2" },
    { 5, "ch2_and_ch3" },
    { 6, "ch1_and_ch3" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_toeldckg[] = {
    { 101, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 1, "switch_1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "switch_2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "switch_3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "trigger_hold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_outage_memory", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_toeldckg_power_outage_memory_dp14, sizeof(kEnum__TZE200_toeldckg_power_outage_memory_dp14)/sizeof(kEnum__TZE200_toeldckg_power_outage_memory_dp14[0]) },
    { 104, "auto_on", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_toeldckg_auto_on_dp104, sizeof(kEnum__TZE200_toeldckg_auto_on_dp104)/sizeof(kEnum__TZE200_toeldckg_auto_on_dp104[0]) },
    { 105, "auto_off", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_toeldckg_auto_off_dp105, sizeof(kEnum__TZE200_toeldckg_auto_off_dp105)/sizeof(kEnum__TZE200_toeldckg_auto_off_dp105[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_toeldckg{ kEntries__TZE200_toeldckg, 10 };
constexpr FzConverter kFzDp__TZE200_toeldckg{
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
    .user_config       = &kMap__TZE200_toeldckg,
};
const FzConverter* const kFz__TZE200_toeldckg[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_toeldckg,
};
constexpr TzConverter kTzDp__TZE200_toeldckg{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_toeldckg,
};
const TzConverter* const kTz__TZE200_toeldckg[] = { &kTzDp__TZE200_toeldckg };
constexpr const char* kM__TZE200_toeldckg[] = { "TS0601" };
constexpr const char* kN__TZE200_toeldckg[] = { "_TZE200_toeldckg" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_toeldckg{
    .zigbee_models=kM__TZE200_toeldckg,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_toeldckg,.manufacturer_names_count=1,
    .model="TS0601__TZE200_toeldckg",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_toeldckg,
    .from_zigbee_count=sizeof(kFz__TZE200_toeldckg)/sizeof(kFz__TZE200_toeldckg[0]),
    .to_zigbee=kTz__TZE200_toeldckg,
    .to_zigbee_count=sizeof(kTz__TZE200_toeldckg)/sizeof(kTz__TZE200_toeldckg[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
