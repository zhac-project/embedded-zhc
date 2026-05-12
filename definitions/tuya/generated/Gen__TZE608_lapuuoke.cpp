// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0603 / _TZE608_lapuuoke).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE608_lapuuoke[] = {
    { 1, "state", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "garage_door_contact", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE608_lapuuoke{ kEntries__TZE608_lapuuoke, 2 };
constexpr FzConverter kFzDp__TZE608_lapuuoke{
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
    .user_config       = &kMap__TZE608_lapuuoke,
};
const FzConverter* const kFz__TZE608_lapuuoke[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE608_lapuuoke,
};
constexpr TzConverter kTzDp__TZE608_lapuuoke{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE608_lapuuoke,
};
const TzConverter* const kTz__TZE608_lapuuoke[] = { &kTzDp__TZE608_lapuuoke };
constexpr const char* kM__TZE608_lapuuoke[] = { "TS0603" };
constexpr const char* kN__TZE608_lapuuoke[] = { "_TZE608_lapuuoke" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE608_lapuuoke{
    .zigbee_models=kM__TZE608_lapuuoke,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE608_lapuuoke,.manufacturer_names_count=1,
    .model="TS0603__TZE608_lapuuoke",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE608_lapuuoke,
    .from_zigbee_count=sizeof(kFz__TZE608_lapuuoke)/sizeof(kFz__TZE608_lapuuoke[0]),
    .to_zigbee=kTz__TZE608_lapuuoke,
    .to_zigbee_count=sizeof(kTz__TZE608_lapuuoke)/sizeof(kTz__TZE608_lapuuoke[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
