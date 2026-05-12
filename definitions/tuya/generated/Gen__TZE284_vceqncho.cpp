// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_vceqncho).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_vceqncho[] = {
    { 1, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "presence_distance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "radar_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 104, "pir_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "delay_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "led_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_vceqncho{ kEntries__TZE284_vceqncho, 9 };
constexpr FzConverter kFzDp__TZE284_vceqncho{
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
    .user_config       = &kMap__TZE284_vceqncho,
};
const FzConverter* const kFz__TZE284_vceqncho[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_vceqncho,
};
constexpr TzConverter kTzDp__TZE284_vceqncho{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_vceqncho,
};
const TzConverter* const kTz__TZE284_vceqncho[] = { &kTzDp__TZE284_vceqncho };
constexpr const char* kM__TZE284_vceqncho[] = { "TS0601" };
constexpr const char* kN__TZE284_vceqncho[] = { "_TZE284_vceqncho" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_vceqncho{
    .zigbee_models=kM__TZE284_vceqncho,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_vceqncho,.manufacturer_names_count=1,
    .model="TS0601__TZE284_vceqncho",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_vceqncho,
    .from_zigbee_count=sizeof(kFz__TZE284_vceqncho)/sizeof(kFz__TZE284_vceqncho[0]),
    .to_zigbee=kTz__TZE284_vceqncho,
    .to_zigbee_count=sizeof(kTz__TZE284_vceqncho)/sizeof(kTz__TZE284_vceqncho[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
