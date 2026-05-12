// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_68utemio).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_68utemio_light_type_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_68utemio_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_68utemio_backlight_mode_dp21[] = {
    { 0, "off" },
    { 1, "normal" },
    { 2, "inverted" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_68utemio[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_68utemio_light_type_dp4, sizeof(kEnum__TZE284_68utemio_light_type_dp4)/sizeof(kEnum__TZE284_68utemio_light_type_dp4[0]) },
    { 5, "max_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_68utemio_power_on_behavior_dp14, sizeof(kEnum__TZE284_68utemio_power_on_behavior_dp14)/sizeof(kEnum__TZE284_68utemio_power_on_behavior_dp14[0]) },
    { 21, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_68utemio_backlight_mode_dp21, sizeof(kEnum__TZE284_68utemio_backlight_mode_dp21)/sizeof(kEnum__TZE284_68utemio_backlight_mode_dp21[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_68utemio{ kEntries__TZE284_68utemio, 8 };
constexpr FzConverter kFzDp__TZE284_68utemio{
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
    .user_config       = &kMap__TZE284_68utemio,
};
const FzConverter* const kFz__TZE284_68utemio[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_68utemio,
};
constexpr TzConverter kTzDp__TZE284_68utemio{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_68utemio,
};
const TzConverter* const kTz__TZE284_68utemio[] = { &kTzDp__TZE284_68utemio };
constexpr const char* kM__TZE284_68utemio[] = { "TS0601" };
constexpr const char* kN__TZE284_68utemio[] = { "_TZE284_68utemio" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_68utemio{
    .zigbee_models=kM__TZE284_68utemio,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_68utemio,.manufacturer_names_count=1,
    .model="TS0601__TZE284_68utemio",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_68utemio,
    .from_zigbee_count=sizeof(kFz__TZE284_68utemio)/sizeof(kFz__TZE284_68utemio[0]),
    .to_zigbee=kTz__TZE284_68utemio,
    .to_zigbee_count=sizeof(kTz__TZE284_68utemio)/sizeof(kTz__TZE284_68utemio[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
