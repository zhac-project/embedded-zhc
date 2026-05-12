// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_mxujdmxo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_mxujdmxo_liquid_state_dp1[] = {
    { 1, "low" },
    { 0, "normal" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_mxujdmxo[] = {
    { 1, "liquid_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_mxujdmxo_liquid_state_dp1, sizeof(kEnum__TZE284_mxujdmxo_liquid_state_dp1)/sizeof(kEnum__TZE284_mxujdmxo_liquid_state_dp1[0]) },
    { 2, "liquid_depth", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 5, "power_level", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 7, "max_set", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "mini_set", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "liquid_depth_max", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 22, "liquid_level_percent", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "version", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_mxujdmxo{ kEntries__TZE284_mxujdmxo, 8 };
constexpr FzConverter kFzDp__TZE284_mxujdmxo{
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
    .user_config       = &kMap__TZE284_mxujdmxo,
};
const FzConverter* const kFz__TZE284_mxujdmxo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_mxujdmxo,
};
constexpr TzConverter kTzDp__TZE284_mxujdmxo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_mxujdmxo,
};
const TzConverter* const kTz__TZE284_mxujdmxo[] = { &kTzDp__TZE284_mxujdmxo };
constexpr const char* kM__TZE284_mxujdmxo[] = { "TS0601" };
constexpr const char* kN__TZE284_mxujdmxo[] = { "_TZE284_mxujdmxo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_mxujdmxo{
    .zigbee_models=kM__TZE284_mxujdmxo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_mxujdmxo,.manufacturer_names_count=1,
    .model="TS0601__TZE284_mxujdmxo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_mxujdmxo,
    .from_zigbee_count=sizeof(kFz__TZE284_mxujdmxo)/sizeof(kFz__TZE284_mxujdmxo[0]),
    .to_zigbee=kTz__TZE284_mxujdmxo,
    .to_zigbee_count=sizeof(kTz__TZE284_mxujdmxo)/sizeof(kTz__TZE284_mxujdmxo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
