// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_0zaf1cr8).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_0zaf1cr8[] = {
    { 1, "smoke", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 4, "tamper", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "battery_low", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_0zaf1cr8{ kEntries__TZE284_0zaf1cr8, 3 };
constexpr FzConverter kFzDp__TZE284_0zaf1cr8{
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
    .user_config       = &kMap__TZE284_0zaf1cr8,
};
const FzConverter* const kFz__TZE284_0zaf1cr8[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_0zaf1cr8,
};
constexpr TzConverter kTzDp__TZE284_0zaf1cr8{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_0zaf1cr8,
};
const TzConverter* const kTz__TZE284_0zaf1cr8[] = { &kTzDp__TZE284_0zaf1cr8 };
constexpr const char* kM__TZE284_0zaf1cr8[] = { "TS0601" };
constexpr const char* kN__TZE284_0zaf1cr8[] = { "_TZE284_0zaf1cr8" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_0zaf1cr8{
    .zigbee_models=kM__TZE284_0zaf1cr8,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_0zaf1cr8,.manufacturer_names_count=1,
    .model="TS0601__TZE284_0zaf1cr8",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_0zaf1cr8,
    .from_zigbee_count=sizeof(kFz__TZE284_0zaf1cr8)/sizeof(kFz__TZE284_0zaf1cr8[0]),
    .to_zigbee=kTz__TZE284_0zaf1cr8,
    .to_zigbee_count=sizeof(kTz__TZE284_0zaf1cr8)/sizeof(kTz__TZE284_0zaf1cr8[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
