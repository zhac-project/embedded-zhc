// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_9qhuzgo0).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_9qhuzgo0_light_type_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_9qhuzgo0_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_9qhuzgo0_backlight_mode_dp21[] = {
    { 0, "off" },
    { 1, "normal" },
    { 2, "inverted" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_9qhuzgo0[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_9qhuzgo0_light_type_dp4, sizeof(kEnum__TZE204_9qhuzgo0_light_type_dp4)/sizeof(kEnum__TZE204_9qhuzgo0_light_type_dp4[0]) },
    { 5, "max_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_9qhuzgo0_power_on_behavior_dp14, sizeof(kEnum__TZE204_9qhuzgo0_power_on_behavior_dp14)/sizeof(kEnum__TZE204_9qhuzgo0_power_on_behavior_dp14[0]) },
    { 21, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_9qhuzgo0_backlight_mode_dp21, sizeof(kEnum__TZE204_9qhuzgo0_backlight_mode_dp21)/sizeof(kEnum__TZE204_9qhuzgo0_backlight_mode_dp21[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_9qhuzgo0{ kEntries__TZE204_9qhuzgo0, 8 };
constexpr FzConverter kFzDp__TZE204_9qhuzgo0{
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
    .user_config       = &kMap__TZE204_9qhuzgo0,
};
const FzConverter* const kFz__TZE204_9qhuzgo0[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_9qhuzgo0,
};
constexpr TzConverter kTzDp__TZE204_9qhuzgo0{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_9qhuzgo0,
};
const TzConverter* const kTz__TZE204_9qhuzgo0[] = { &kTzDp__TZE204_9qhuzgo0 };
constexpr const char* kM__TZE204_9qhuzgo0[] = { "TS0601" };
constexpr const char* kN__TZE204_9qhuzgo0[] = { "_TZE204_9qhuzgo0" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_9qhuzgo0{
    .zigbee_models=kM__TZE204_9qhuzgo0,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_9qhuzgo0,.manufacturer_names_count=1,
    .model="TS0601__TZE204_9qhuzgo0",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_9qhuzgo0,
    .from_zigbee_count=sizeof(kFz__TZE204_9qhuzgo0)/sizeof(kFz__TZE204_9qhuzgo0[0]),
    .to_zigbee=kTz__TZE204_9qhuzgo0,
    .to_zigbee_count=sizeof(kTz__TZE204_9qhuzgo0)/sizeof(kTz__TZE204_9qhuzgo0[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
