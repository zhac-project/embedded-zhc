// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_oyti2ums).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_oyti2ums[] = {
    { 1, "state_right", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_light", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_left", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "state_master", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "current", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 22, "energy_wh", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 23, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_oyti2ums{ kEntries__TZE200_oyti2ums, 7 };
constexpr FzConverter kFzDp__TZE200_oyti2ums{
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
    .user_config       = &kMap__TZE200_oyti2ums,
};
const FzConverter* const kFz__TZE200_oyti2ums[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_oyti2ums,
};
constexpr TzConverter kTzDp__TZE200_oyti2ums{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_oyti2ums,
};
const TzConverter* const kTz__TZE200_oyti2ums[] = { &kTzDp__TZE200_oyti2ums };
constexpr const char* kM__TZE200_oyti2ums[] = { "TS0601" };
constexpr const char* kN__TZE200_oyti2ums[] = { "_TZE200_oyti2ums" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_oyti2ums{
    .zigbee_models=kM__TZE200_oyti2ums,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_oyti2ums,.manufacturer_names_count=1,
    .model="TS0601__TZE200_oyti2ums",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_oyti2ums,
    .from_zigbee_count=sizeof(kFz__TZE200_oyti2ums)/sizeof(kFz__TZE200_oyti2ums[0]),
    .to_zigbee=kTz__TZE200_oyti2ums,
    .to_zigbee_count=sizeof(kTz__TZE200_oyti2ums)/sizeof(kTz__TZE200_oyti2ums[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
