// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_eaet5qt5).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_eaet5qt5[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 25, "valve_duration_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 26, "valve_duration_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 59, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_eaet5qt5{ kEntries__TZE284_eaet5qt5, 7 };
constexpr FzConverter kFzDp__TZE284_eaet5qt5{
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
    .user_config       = &kMap__TZE284_eaet5qt5,
};
const FzConverter* const kFz__TZE284_eaet5qt5[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_eaet5qt5,
};
constexpr TzConverter kTzDp__TZE284_eaet5qt5{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_eaet5qt5,
};
const TzConverter* const kTz__TZE284_eaet5qt5[] = { &kTzDp__TZE284_eaet5qt5 };
constexpr const char* kM__TZE284_eaet5qt5[] = { "TS0601" };
constexpr const char* kN__TZE284_eaet5qt5[] = { "_TZE284_eaet5qt5" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_eaet5qt5{
    .zigbee_models=kM__TZE284_eaet5qt5,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_eaet5qt5,.manufacturer_names_count=1,
    .model="TS0601__TZE284_eaet5qt5",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_eaet5qt5,
    .from_zigbee_count=sizeof(kFz__TZE284_eaet5qt5)/sizeof(kFz__TZE284_eaet5qt5[0]),
    .to_zigbee=kTz__TZE284_eaet5qt5,
    .to_zigbee_count=sizeof(kTz__TZE284_eaet5qt5)/sizeof(kTz__TZE284_eaet5qt5[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
