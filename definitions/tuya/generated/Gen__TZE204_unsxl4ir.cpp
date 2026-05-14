// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_unsxl4ir).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_unsxl4ir_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_unsxl4ir[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "countdown_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "countdown_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_unsxl4ir_power_on_behavior_dp14, sizeof(kEnum__TZE204_unsxl4ir_power_on_behavior_dp14)/sizeof(kEnum__TZE204_unsxl4ir_power_on_behavior_dp14[0]) },
    { 16, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_unsxl4ir{ kEntries__TZE204_unsxl4ir, 11 };
constexpr FzConverter kFzDp__TZE204_unsxl4ir{
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
    .user_config       = &kMap__TZE204_unsxl4ir,
};
const FzConverter* const kFz__TZE204_unsxl4ir[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_unsxl4ir,
};
constexpr TzConverter kTzDp__TZE204_unsxl4ir{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_unsxl4ir,
};
const TzConverter* const kTz__TZE204_unsxl4ir[] = { &kTzDp__TZE204_unsxl4ir };
constexpr const char* kM__TZE204_unsxl4ir[] = { "TS0601" };
constexpr const char* kN__TZE204_unsxl4ir[] = { "_TZE204_unsxl4ir" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_unsxl4ir[] = {
    {"Nova Digital","FZB-4"},
};
extern const PreparedDefinition kDefGen__TZE204_unsxl4ir{
    .zigbee_models=kM__TZE204_unsxl4ir,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_unsxl4ir,.manufacturer_names_count=1,
    .model="TS0601__TZE204_unsxl4ir",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_unsxl4ir, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_unsxl4ir)/sizeof(kWhiteLabels_Gen_TZE204_unsxl4ir[0]),
    .from_zigbee=kFz__TZE204_unsxl4ir,
    .from_zigbee_count=sizeof(kFz__TZE204_unsxl4ir)/sizeof(kFz__TZE204_unsxl4ir[0]),
    .to_zigbee=kTz__TZE204_unsxl4ir,
    .to_zigbee_count=sizeof(kTz__TZE204_unsxl4ir)/sizeof(kTz__TZE204_unsxl4ir[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
