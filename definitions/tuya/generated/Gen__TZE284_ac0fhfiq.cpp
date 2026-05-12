// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_ac0fhfiq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_ac0fhfiq[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 2, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_ac0fhfiq{ kEntries__TZE284_ac0fhfiq, 2 };
constexpr FzConverter kFzDp__TZE284_ac0fhfiq{
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
    .user_config       = &kMap__TZE284_ac0fhfiq,
};
const FzConverter* const kFz__TZE284_ac0fhfiq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_ac0fhfiq,
};
constexpr TzConverter kTzDp__TZE284_ac0fhfiq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_ac0fhfiq,
};
const TzConverter* const kTz__TZE284_ac0fhfiq[] = { &kTzDp__TZE284_ac0fhfiq };
constexpr const char* kM__TZE284_ac0fhfiq[] = { "TS0601" };
constexpr const char* kN__TZE284_ac0fhfiq[] = { "_TZE284_ac0fhfiq" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_ac0fhfiq{
    .zigbee_models=kM__TZE284_ac0fhfiq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_ac0fhfiq,.manufacturer_names_count=1,
    .model="TS0601__TZE284_ac0fhfiq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_ac0fhfiq,
    .from_zigbee_count=sizeof(kFz__TZE284_ac0fhfiq)/sizeof(kFz__TZE284_ac0fhfiq[0]),
    .to_zigbee=kTz__TZE284_ac0fhfiq,
    .to_zigbee_count=sizeof(kTz__TZE284_ac0fhfiq)/sizeof(kTz__TZE284_ac0fhfiq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
