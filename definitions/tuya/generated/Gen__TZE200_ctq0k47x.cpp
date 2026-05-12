// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_ctq0k47x).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ctq0k47x_light_type_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ctq0k47x_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ctq0k47x_backlight_mode_dp21[] = {
    { 0, "off" },
    { 1, "normal" },
    { 2, "inverted" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_ctq0k47x[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ctq0k47x_light_type_dp4, sizeof(kEnum__TZE200_ctq0k47x_light_type_dp4)/sizeof(kEnum__TZE200_ctq0k47x_light_type_dp4[0]) },
    { 5, "max_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ctq0k47x_power_on_behavior_dp14, sizeof(kEnum__TZE200_ctq0k47x_power_on_behavior_dp14)/sizeof(kEnum__TZE200_ctq0k47x_power_on_behavior_dp14[0]) },
    { 21, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ctq0k47x_backlight_mode_dp21, sizeof(kEnum__TZE200_ctq0k47x_backlight_mode_dp21)/sizeof(kEnum__TZE200_ctq0k47x_backlight_mode_dp21[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_ctq0k47x{ kEntries__TZE200_ctq0k47x, 8 };
constexpr FzConverter kFzDp__TZE200_ctq0k47x{
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
    .user_config       = &kMap__TZE200_ctq0k47x,
};
const FzConverter* const kFz__TZE200_ctq0k47x[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_ctq0k47x,
};
constexpr TzConverter kTzDp__TZE200_ctq0k47x{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_ctq0k47x,
};
const TzConverter* const kTz__TZE200_ctq0k47x[] = { &kTzDp__TZE200_ctq0k47x };
constexpr const char* kM__TZE200_ctq0k47x[] = { "TS0601" };
constexpr const char* kN__TZE200_ctq0k47x[] = { "_TZE200_ctq0k47x" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_ctq0k47x{
    .zigbee_models=kM__TZE200_ctq0k47x,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_ctq0k47x,.manufacturer_names_count=1,
    .model="TS0601__TZE200_ctq0k47x",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_ctq0k47x,
    .from_zigbee_count=sizeof(kFz__TZE200_ctq0k47x)/sizeof(kFz__TZE200_ctq0k47x[0]),
    .to_zigbee=kTz__TZE200_ctq0k47x,
    .to_zigbee_count=sizeof(kTz__TZE200_ctq0k47x)/sizeof(kTz__TZE200_ctq0k47x[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
