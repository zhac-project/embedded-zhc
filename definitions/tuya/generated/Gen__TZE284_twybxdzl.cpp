// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_twybxdzl).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_twybxdzl[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "move_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "detection_distance_max", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 9, "distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "presence_timeout", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "living_room", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "bedroom", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "bathroom", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "sleep", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 109, "radar_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_twybxdzl{ kEntries__TZE284_twybxdzl, 13 };
constexpr FzConverter kFzDp__TZE284_twybxdzl{
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
    .user_config       = &kMap__TZE284_twybxdzl,
};
const FzConverter* const kFz__TZE284_twybxdzl[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_twybxdzl,
};
constexpr TzConverter kTzDp__TZE284_twybxdzl{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_twybxdzl,
};
const TzConverter* const kTz__TZE284_twybxdzl[] = { &kTzDp__TZE284_twybxdzl };
constexpr const char* kM__TZE284_twybxdzl[] = { "TS0601" };
constexpr const char* kN__TZE284_twybxdzl[] = { "_TZE284_twybxdzl" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_twybxdzl[] = {
    {"Tuya","ZT24"},
    {"Tuya","ZX24"},
};
extern const PreparedDefinition kDefGen__TZE284_twybxdzl{
    .zigbee_models=kM__TZE284_twybxdzl,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_twybxdzl,.manufacturer_names_count=1,
    .model="TS0601__TZE284_twybxdzl",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE284_twybxdzl, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_twybxdzl)/sizeof(kWhiteLabels_Gen_TZE284_twybxdzl[0]),
    .from_zigbee=kFz__TZE284_twybxdzl,
    .from_zigbee_count=sizeof(kFz__TZE284_twybxdzl)/sizeof(kFz__TZE284_twybxdzl[0]),
    .to_zigbee=kTz__TZE284_twybxdzl,
    .to_zigbee_count=sizeof(kTz__TZE284_twybxdzl)/sizeof(kTz__TZE284_twybxdzl[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
