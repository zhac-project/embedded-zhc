// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_6ycgarab).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_6ycgarab_alarm_volume_dp5[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
    { 3, "mute" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_6ycgarab[] = {
    { 5, "alarm_volume", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_6ycgarab_alarm_volume_dp5, sizeof(kEnum__TZE284_6ycgarab_alarm_volume_dp5)/sizeof(kEnum__TZE284_6ycgarab_alarm_volume_dp5[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "alarm_switch", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "carbon_monoxide", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_6ycgarab{ kEntries__TZE284_6ycgarab, 5 };
constexpr FzConverter kFzDp__TZE284_6ycgarab{
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
    .user_config       = &kMap__TZE284_6ycgarab,
};
const FzConverter* const kFz__TZE284_6ycgarab[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_6ycgarab,
};
constexpr TzConverter kTzDp__TZE284_6ycgarab{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_6ycgarab,
};
const TzConverter* const kTz__TZE284_6ycgarab[] = { &kTzDp__TZE284_6ycgarab };
constexpr const char* kM__TZE284_6ycgarab[] = { "TS0601" };
constexpr const char* kN__TZE284_6ycgarab[] = { "_TZE284_6ycgarab" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_6ycgarab{
    .zigbee_models=kM__TZE284_6ycgarab,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_6ycgarab,.manufacturer_names_count=1,
    .model="TS0601__TZE284_6ycgarab",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_6ycgarab,
    .from_zigbee_count=sizeof(kFz__TZE284_6ycgarab)/sizeof(kFz__TZE284_6ycgarab[0]),
    .to_zigbee=kTz__TZE284_6ycgarab,
    .to_zigbee_count=sizeof(kTz__TZE284_6ycgarab)/sizeof(kTz__TZE284_6ycgarab[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
