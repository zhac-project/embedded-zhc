// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_ajlu4cud).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_ajlu4cud[] = {
    { 1, "water_consumed", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 16, "meter_id", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 22, "temperature", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 26, "battery_voltage", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_ajlu4cud{ kEntries__TZE284_ajlu4cud, 4 };
constexpr FzConverter kFzDp__TZE284_ajlu4cud{
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
    .user_config       = &kMap__TZE284_ajlu4cud,
};
const FzConverter* const kFz__TZE284_ajlu4cud[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_ajlu4cud,
};
constexpr TzConverter kTzDp__TZE284_ajlu4cud{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_ajlu4cud,
};
const TzConverter* const kTz__TZE284_ajlu4cud[] = { &kTzDp__TZE284_ajlu4cud };
constexpr const char* kM__TZE284_ajlu4cud[] = { "TS0601" };
constexpr const char* kN__TZE284_ajlu4cud[] = { "_TZE284_ajlu4cud" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_ajlu4cud{
    .zigbee_models=kM__TZE284_ajlu4cud,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_ajlu4cud,.manufacturer_names_count=1,
    .model="TS0601__TZE284_ajlu4cud",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_ajlu4cud,
    .from_zigbee_count=sizeof(kFz__TZE284_ajlu4cud)/sizeof(kFz__TZE284_ajlu4cud[0]),
    .to_zigbee=kTz__TZE284_ajlu4cud,
    .to_zigbee_count=sizeof(kTz__TZE284_ajlu4cud)/sizeof(kTz__TZE284_ajlu4cud[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
