// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_debczeci).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_debczeci[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_debczeci{ kEntries__TZE284_debczeci, 2 };
constexpr FzConverter kFzDp__TZE284_debczeci{
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
    .user_config       = &kMap__TZE284_debczeci,
};
const FzConverter* const kFz__TZE284_debczeci[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_debczeci,
};
constexpr TzConverter kTzDp__TZE284_debczeci{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_debczeci,
};
const TzConverter* const kTz__TZE284_debczeci[] = { &kTzDp__TZE284_debczeci };
constexpr const char* kM__TZE284_debczeci[] = { "TS0601" };
constexpr const char* kN__TZE284_debczeci[] = { "_TZE284_debczeci" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_debczeci{
    .zigbee_models=kM__TZE284_debczeci,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_debczeci,.manufacturer_names_count=1,
    .model="TS0601__TZE284_debczeci",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_debczeci,
    .from_zigbee_count=sizeof(kFz__TZE284_debczeci)/sizeof(kFz__TZE284_debczeci[0]),
    .to_zigbee=kTz__TZE284_debczeci,
    .to_zigbee_count=sizeof(kTz__TZE284_debczeci)/sizeof(kTz__TZE284_debczeci[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
