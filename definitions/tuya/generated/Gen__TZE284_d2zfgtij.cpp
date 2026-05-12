// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_d2zfgtij).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_d2zfgtij[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "device_locating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_d2zfgtij{ kEntries__TZE284_d2zfgtij, 5 };
constexpr FzConverter kFzDp__TZE284_d2zfgtij{
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
    .user_config       = &kMap__TZE284_d2zfgtij,
};
const FzConverter* const kFz__TZE284_d2zfgtij[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_d2zfgtij,
};
constexpr TzConverter kTzDp__TZE284_d2zfgtij{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_d2zfgtij,
};
const TzConverter* const kTz__TZE284_d2zfgtij[] = { &kTzDp__TZE284_d2zfgtij };
constexpr const char* kM__TZE284_d2zfgtij[] = { "TS0601" };
constexpr const char* kN__TZE284_d2zfgtij[] = { "_TZE284_d2zfgtij" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_d2zfgtij{
    .zigbee_models=kM__TZE284_d2zfgtij,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_d2zfgtij,.manufacturer_names_count=1,
    .model="TS0601__TZE284_d2zfgtij",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_d2zfgtij,
    .from_zigbee_count=sizeof(kFz__TZE284_d2zfgtij)/sizeof(kFz__TZE284_d2zfgtij[0]),
    .to_zigbee=kTz__TZE284_d2zfgtij,
    .to_zigbee_count=sizeof(kTz__TZE284_d2zfgtij)/sizeof(kTz__TZE284_d2zfgtij[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
