// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_seq9cm6u).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_seq9cm6u_sensitivity_dp9[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_seq9cm6u_work_state_dp104[] = {
    { 0, "presence" },
    { 1, "none" },
    { 2, "presence_5min" },
    { 3, "presence_30min" },
    { 4, "none_5min" },
    { 5, "none_30min" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_seq9cm6u[] = {
    { 1, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_seq9cm6u_sensitivity_dp9, sizeof(kEnum__TZE200_seq9cm6u_sensitivity_dp9)/sizeof(kEnum__TZE200_seq9cm6u_sensitivity_dp9[0]) },
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "interval_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "presence_delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "presence_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "work_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_seq9cm6u_work_state_dp104, sizeof(kEnum__TZE200_seq9cm6u_work_state_dp104)/sizeof(kEnum__TZE200_seq9cm6u_work_state_dp104[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_seq9cm6u{ kEntries__TZE200_seq9cm6u, 8 };
constexpr FzConverter kFzDp__TZE200_seq9cm6u{
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
    .user_config       = &kMap__TZE200_seq9cm6u,
};
const FzConverter* const kFz__TZE200_seq9cm6u[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_seq9cm6u,
};
constexpr TzConverter kTzDp__TZE200_seq9cm6u{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_seq9cm6u,
};
const TzConverter* const kTz__TZE200_seq9cm6u[] = { &kTzDp__TZE200_seq9cm6u };
constexpr const char* kM__TZE200_seq9cm6u[] = { "TS0601" };
constexpr const char* kN__TZE200_seq9cm6u[] = { "_TZE200_seq9cm6u" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_seq9cm6u{
    .zigbee_models=kM__TZE200_seq9cm6u,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_seq9cm6u,.manufacturer_names_count=1,
    .model="TS0601__TZE200_seq9cm6u",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_seq9cm6u,
    .from_zigbee_count=sizeof(kFz__TZE200_seq9cm6u)/sizeof(kFz__TZE200_seq9cm6u[0]),
    .to_zigbee=kTz__TZE200_seq9cm6u,
    .to_zigbee_count=sizeof(kTz__TZE200_seq9cm6u)/sizeof(kTz__TZE200_seq9cm6u[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
