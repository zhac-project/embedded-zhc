// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_bql5khqx).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_bql5khqx_power_on_behavior_dp11[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_bql5khqx_indicator_dp12[] = {
    { 0, "off" },
    { 2, "on" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_bql5khqx[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "fan_speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "light_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 11, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_bql5khqx_power_on_behavior_dp11, sizeof(kEnum__TZE204_bql5khqx_power_on_behavior_dp11)/sizeof(kEnum__TZE204_bql5khqx_power_on_behavior_dp11[0]) },
    { 12, "indicator", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_bql5khqx_indicator_dp12, sizeof(kEnum__TZE204_bql5khqx_indicator_dp12)/sizeof(kEnum__TZE204_bql5khqx_indicator_dp12[0]) },
    { 13, "backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 104, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "minimum_speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_bql5khqx{ kEntries__TZE204_bql5khqx, 8 };
constexpr FzConverter kFzDp__TZE204_bql5khqx{
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
    .user_config       = &kMap__TZE204_bql5khqx,
};
const FzConverter* const kFz__TZE204_bql5khqx[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_bql5khqx,
};
constexpr TzConverter kTzDp__TZE204_bql5khqx{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_bql5khqx,
};
const TzConverter* const kTz__TZE204_bql5khqx[] = { &kTzDp__TZE204_bql5khqx };
constexpr const char* kM__TZE204_bql5khqx[] = { "TS0601" };
constexpr const char* kN__TZE204_bql5khqx[] = { "_TZE204_bql5khqx" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_bql5khqx[] = {
    {"Coswall","X99-G-kbFan-1g-ZG-LN-11"},
    {"Zemismart","ZN2S-RS1E-FL / ZN2S-US1U-FL"},
};
extern const PreparedDefinition kDefGen__TZE204_bql5khqx{
    .zigbee_models=kM__TZE204_bql5khqx,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_bql5khqx,.manufacturer_names_count=1,
    .model="TS0601__TZE204_bql5khqx",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_bql5khqx, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_bql5khqx)/sizeof(kWhiteLabels_Gen_TZE204_bql5khqx[0]),
    .from_zigbee=kFz__TZE204_bql5khqx,
    .from_zigbee_count=sizeof(kFz__TZE204_bql5khqx)/sizeof(kFz__TZE204_bql5khqx[0]),
    .to_zigbee=kTz__TZE204_bql5khqx,
    .to_zigbee_count=sizeof(kTz__TZE204_bql5khqx)/sizeof(kTz__TZE204_bql5khqx[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
