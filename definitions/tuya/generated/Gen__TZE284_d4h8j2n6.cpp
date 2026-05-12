// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint ZP-301Z / _TZE284_d4h8j2n6).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_d4h8j2n6[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "battery_value", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 100, "brightness_value", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "illuminance_trigger", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "presence_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "presence_delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "detection_cycle", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_d4h8j2n6{ kEntries__TZE284_d4h8j2n6, 8 };
constexpr FzConverter kFzDp__TZE284_d4h8j2n6{
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
    .user_config       = &kMap__TZE284_d4h8j2n6,
};
const FzConverter* const kFz__TZE284_d4h8j2n6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_d4h8j2n6,
};
constexpr TzConverter kTzDp__TZE284_d4h8j2n6{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_d4h8j2n6,
};
const TzConverter* const kTz__TZE284_d4h8j2n6[] = { &kTzDp__TZE284_d4h8j2n6 };
constexpr const char* kM__TZE284_d4h8j2n6[] = { "ZP-301Z" };
constexpr const char* kN__TZE284_d4h8j2n6[] = { "_TZE284_d4h8j2n6" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_d4h8j2n6{
    .zigbee_models=kM__TZE284_d4h8j2n6,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_d4h8j2n6,.manufacturer_names_count=1,
    .model="ZP-301Z__TZE284_d4h8j2n6",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_d4h8j2n6,
    .from_zigbee_count=sizeof(kFz__TZE284_d4h8j2n6)/sizeof(kFz__TZE284_d4h8j2n6[0]),
    .to_zigbee=kTz__TZE284_d4h8j2n6,
    .to_zigbee_count=sizeof(kTz__TZE284_d4h8j2n6)/sizeof(kTz__TZE284_d4h8j2n6[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
