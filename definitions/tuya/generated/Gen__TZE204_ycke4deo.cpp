// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ycke4deo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ycke4deo_state_dp1[] = {
    { 2, "OPEN" },
    { 1, "STOP" },
    { 0, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ycke4deo[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ycke4deo_state_dp1, sizeof(kEnum__TZE204_ycke4deo_state_dp1)/sizeof(kEnum__TZE204_ycke4deo_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ycke4deo{ kEntries__TZE204_ycke4deo, 3 };
constexpr FzConverter kFzDp__TZE204_ycke4deo{
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
    .user_config       = &kMap__TZE204_ycke4deo,
};
const FzConverter* const kFz__TZE204_ycke4deo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ycke4deo,
};
constexpr TzConverter kTzDp__TZE204_ycke4deo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ycke4deo,
};
const TzConverter* const kTz__TZE204_ycke4deo[] = { &kTzDp__TZE204_ycke4deo };
constexpr const char* kM__TZE204_ycke4deo[] = { "TS0601" };
constexpr const char* kN__TZE204_ycke4deo[] = { "_TZE204_ycke4deo" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_ycke4deo[] = {
    {"Nova Digital","ZC-GM42"},
    {"Moes","ZC-LS02"},
};
extern const PreparedDefinition kDefGen__TZE204_ycke4deo{
    .zigbee_models=kM__TZE204_ycke4deo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ycke4deo,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ycke4deo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_ycke4deo, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_ycke4deo)/sizeof(kWhiteLabels_Gen_TZE204_ycke4deo[0]),
    .from_zigbee=kFz__TZE204_ycke4deo,
    .from_zigbee_count=sizeof(kFz__TZE204_ycke4deo)/sizeof(kFz__TZE204_ycke4deo[0]),
    .to_zigbee=kTz__TZE204_ycke4deo,
    .to_zigbee_count=sizeof(kTz__TZE204_ycke4deo)/sizeof(kTz__TZE204_ycke4deo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
