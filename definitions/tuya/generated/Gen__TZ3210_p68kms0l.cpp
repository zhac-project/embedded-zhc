// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0207 / _TZ3210_p68kms0l).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ3210_p68kms0l[] = {
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "illuminance_raw", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance_average_20min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "illuminance_maximum_today", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "cleaning_reminder", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 105, "rain_intensity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ3210_p68kms0l{ kEntries__TZ3210_p68kms0l, 6 };
constexpr FzConverter kFzDp__TZ3210_p68kms0l{
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
    .user_config       = &kMap__TZ3210_p68kms0l,
};
const FzConverter* const kFz__TZ3210_p68kms0l[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ3210_p68kms0l,
};
constexpr TzConverter kTzDp__TZ3210_p68kms0l{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ3210_p68kms0l,
};
const TzConverter* const kTz__TZ3210_p68kms0l[] = { &kTzDp__TZ3210_p68kms0l };
constexpr const char* kM__TZ3210_p68kms0l[] = { "TS0207" };
constexpr const char* kN__TZ3210_p68kms0l[] = { "_TZ3210_p68kms0l" };
}  // namespace
extern const PreparedDefinition kDefGen__TZ3210_p68kms0l{
    .zigbee_models=kM__TZ3210_p68kms0l,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ3210_p68kms0l,.manufacturer_names_count=1,
    .model="TS0207__TZ3210_p68kms0l",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ3210_p68kms0l,
    .from_zigbee_count=sizeof(kFz__TZ3210_p68kms0l)/sizeof(kFz__TZ3210_p68kms0l[0]),
    .to_zigbee=kTz__TZ3210_p68kms0l,
    .to_zigbee_count=sizeof(kTz__TZ3210_p68kms0l)/sizeof(kTz__TZ3210_p68kms0l[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
