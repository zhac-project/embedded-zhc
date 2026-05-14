// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_86nbew0j).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_86nbew0j_power_on_behavior_dp2[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_86nbew0j[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_86nbew0j_power_on_behavior_dp2, sizeof(kEnum__TZE200_86nbew0j_power_on_behavior_dp2)/sizeof(kEnum__TZE200_86nbew0j_power_on_behavior_dp2[0]) },
    { 3, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_86nbew0j{ kEntries__TZE200_86nbew0j, 3 };
constexpr FzConverter kFzDp__TZE200_86nbew0j{
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
    .user_config       = &kMap__TZE200_86nbew0j,
};
const FzConverter* const kFz__TZE200_86nbew0j[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_86nbew0j,
};
constexpr TzConverter kTzDp__TZE200_86nbew0j{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_86nbew0j,
};
const TzConverter* const kTz__TZE200_86nbew0j[] = { &kTzDp__TZE200_86nbew0j };
constexpr const char* kM__TZE200_86nbew0j[] = { "TS0601" };
constexpr const char* kN__TZE200_86nbew0j[] = { "_TZE200_86nbew0j" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_86nbew0j[] = {
    {"Ltech","TY-12-100-400-W1Z"},
    {"Ltech","TY-75-24-G2Z2"},
    {"Lifud","LF-AAZ012-0400-42"},
    {"Lifud","LF-GAZ150A6250-24"},
    {"Lifud","LF-GAZ150B6250-24"},
};
extern const PreparedDefinition kDefGen__TZE200_86nbew0j{
    .zigbee_models=kM__TZE200_86nbew0j,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_86nbew0j,.manufacturer_names_count=1,
    .model="TS0601__TZE200_86nbew0j",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_86nbew0j, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_86nbew0j)/sizeof(kWhiteLabels_Gen_TZE200_86nbew0j[0]),
    .from_zigbee=kFz__TZE200_86nbew0j,
    .from_zigbee_count=sizeof(kFz__TZE200_86nbew0j)/sizeof(kFz__TZE200_86nbew0j[0]),
    .to_zigbee=kTz__TZE200_86nbew0j,
    .to_zigbee_count=sizeof(kTz__TZE200_86nbew0j)/sizeof(kTz__TZE200_86nbew0j[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
