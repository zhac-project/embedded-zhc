// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_k7mfgaen).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_k7mfgaen_type_dp1[] = {
    { 0, "sound" },
    { 1, "light" },
    { 3, "normal" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_k7mfgaen_volume_dp5[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
    { 3, "mute" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_k7mfgaen_battery_level_dp14[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_k7mfgaen_ringtone_dp21[] = {
    { 0, "melody1" },
    { 1, "melody2" },
    { 2, "melody3" },
    { 3, "melody4" },
    { 4, "melody5" },
    { 5, "melody6" },
    { 6, "melody7" },
    { 7, "melody8" },
    { 8, "door" },
    { 9, "water" },
    { 10, "temperature" },
    { 11, "entered" },
    { 12, "left" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_k7mfgaen[] = {
    { 1, "type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_k7mfgaen_type_dp1, sizeof(kEnum__TZE204_k7mfgaen_type_dp1)/sizeof(kEnum__TZE204_k7mfgaen_type_dp1[0]) },
    { 5, "volume", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_k7mfgaen_volume_dp5, sizeof(kEnum__TZE204_k7mfgaen_volume_dp5)/sizeof(kEnum__TZE204_k7mfgaen_volume_dp5[0]) },
    { 7, "duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "alarm", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "battery_level", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_k7mfgaen_battery_level_dp14, sizeof(kEnum__TZE204_k7mfgaen_battery_level_dp14)/sizeof(kEnum__TZE204_k7mfgaen_battery_level_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "ringtone", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_k7mfgaen_ringtone_dp21, sizeof(kEnum__TZE204_k7mfgaen_ringtone_dp21)/sizeof(kEnum__TZE204_k7mfgaen_ringtone_dp21[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_k7mfgaen{ kEntries__TZE204_k7mfgaen, 7 };
constexpr FzConverter kFzDp__TZE204_k7mfgaen{
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
    .user_config       = &kMap__TZE204_k7mfgaen,
};
const FzConverter* const kFz__TZE204_k7mfgaen[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_k7mfgaen,
};
constexpr TzConverter kTzDp__TZE204_k7mfgaen{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_k7mfgaen,
};
const TzConverter* const kTz__TZE204_k7mfgaen[] = { &kTzDp__TZE204_k7mfgaen };
constexpr const char* kM__TZE204_k7mfgaen[] = { "TS0601" };
constexpr const char* kN__TZE204_k7mfgaen[] = { "_TZE204_k7mfgaen" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_k7mfgaen{
    .zigbee_models=kM__TZE204_k7mfgaen,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_k7mfgaen,.manufacturer_names_count=1,
    .model="TS0601__TZE204_k7mfgaen",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_k7mfgaen,
    .from_zigbee_count=sizeof(kFz__TZE204_k7mfgaen)/sizeof(kFz__TZE204_k7mfgaen[0]),
    .to_zigbee=kTz__TZE204_k7mfgaen,
    .to_zigbee_count=sizeof(kTz__TZE204_k7mfgaen)/sizeof(kTz__TZE204_k7mfgaen[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
