// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ejh6owwz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ejh6owwz_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ejh6owwz_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ejh6owwz_border_dp16[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "up_delete" },
    { 3, "down_delete" },
    { 4, "remove_top_bottom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ejh6owwz_click_control_dp20[] = {
    { 0, "up" },
    { 1, "down" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ejh6owwz[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ejh6owwz_state_dp1, sizeof(kEnum__TZE204_ejh6owwz_state_dp1)/sizeof(kEnum__TZE204_ejh6owwz_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ejh6owwz_reverse_direction_dp5, sizeof(kEnum__TZE204_ejh6owwz_reverse_direction_dp5)/sizeof(kEnum__TZE204_ejh6owwz_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ejh6owwz_border_dp16, sizeof(kEnum__TZE204_ejh6owwz_border_dp16)/sizeof(kEnum__TZE204_ejh6owwz_border_dp16[0]) },
    { 20, "click_control", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ejh6owwz_click_control_dp20, sizeof(kEnum__TZE204_ejh6owwz_click_control_dp20)/sizeof(kEnum__TZE204_ejh6owwz_click_control_dp20[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ejh6owwz{ kEntries__TZE204_ejh6owwz, 8 };
constexpr FzConverter kFzDp__TZE204_ejh6owwz{
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
    .user_config       = &kMap__TZE204_ejh6owwz,
};
const FzConverter* const kFz__TZE204_ejh6owwz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ejh6owwz,
};
constexpr TzConverter kTzDp__TZE204_ejh6owwz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ejh6owwz,
};
const TzConverter* const kTz__TZE204_ejh6owwz[] = { &kTzDp__TZE204_ejh6owwz };
constexpr const char* kM__TZE204_ejh6owwz[] = { "TS0601" };
constexpr const char* kN__TZE204_ejh6owwz[] = { "_TZE204_ejh6owwz" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_ejh6owwz[] = {
    {"Zemismart","ZM16EL-03/33"},
    {"Zemismart","ZM25EL"},
    {"Zemismart","ZM85EL-2Z"},
    {"Hiladuo","B09M3R35GC"},
    {"Tuya","MYQ-RM25-1.3/25-BZ"},
    {"Shaman","25EB-1/30-TYZ"},
};
extern const PreparedDefinition kDefGen__TZE204_ejh6owwz{
    .zigbee_models=kM__TZE204_ejh6owwz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ejh6owwz,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ejh6owwz",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_ejh6owwz, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_ejh6owwz)/sizeof(kWhiteLabels_Gen_TZE204_ejh6owwz[0]),
    .from_zigbee=kFz__TZE204_ejh6owwz,
    .from_zigbee_count=sizeof(kFz__TZE204_ejh6owwz)/sizeof(kFz__TZE204_ejh6owwz[0]),
    .to_zigbee=kTz__TZE204_ejh6owwz,
    .to_zigbee_count=sizeof(kTz__TZE204_ejh6owwz)/sizeof(kTz__TZE204_ejh6owwz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
