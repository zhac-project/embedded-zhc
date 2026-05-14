// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_4cqhd2ha).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_4cqhd2ha[] = {
    { 1, "vibration", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "buzzer_mute", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_4cqhd2ha{ kEntries__TZE284_4cqhd2ha, 3 };
constexpr FzConverter kFzDp__TZE284_4cqhd2ha{
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
    .user_config       = &kMap__TZE284_4cqhd2ha,
};
const FzConverter* const kFz__TZE284_4cqhd2ha[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_4cqhd2ha,
};
constexpr TzConverter kTzDp__TZE284_4cqhd2ha{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_4cqhd2ha,
};
const TzConverter* const kTz__TZE284_4cqhd2ha[] = { &kTzDp__TZE284_4cqhd2ha };
constexpr const char* kM__TZE284_4cqhd2ha[] = { "TS0601" };
constexpr const char* kN__TZE284_4cqhd2ha[] = { "_TZE284_4cqhd2ha" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_4cqhd2ha[] = {
    {"Conecto","COZIGVS"},
};
extern const PreparedDefinition kDefGen__TZE284_4cqhd2ha{
    .zigbee_models=kM__TZE284_4cqhd2ha,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_4cqhd2ha,.manufacturer_names_count=1,
    .model="TS0601__TZE284_4cqhd2ha",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE284_4cqhd2ha, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_4cqhd2ha)/sizeof(kWhiteLabels_Gen_TZE284_4cqhd2ha[0]),
    .from_zigbee=kFz__TZE284_4cqhd2ha,
    .from_zigbee_count=sizeof(kFz__TZE284_4cqhd2ha)/sizeof(kFz__TZE284_4cqhd2ha[0]),
    .to_zigbee=kTz__TZE284_4cqhd2ha,
    .to_zigbee_count=sizeof(kTz__TZE284_4cqhd2ha)/sizeof(kTz__TZE284_4cqhd2ha[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
