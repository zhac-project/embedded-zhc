// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0301 / _TZE210_inpjmc0h).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE210_inpjmc0h_state_bottom_dp109[] = {
    { 0, "OPEN" },
    { 1, "CLOSE" },
    { 2, "STOP" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE210_inpjmc0h_state_top_dp1[] = {
    { 1, "OPEN" },
    { 0, "CLOSE" },
    { 2, "STOP" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE210_inpjmc0h[] = {
    { 109, "state_bottom", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE210_inpjmc0h_state_bottom_dp109, sizeof(kEnum__TZE210_inpjmc0h_state_bottom_dp109)/sizeof(kEnum__TZE210_inpjmc0h_state_bottom_dp109[0]) },
    { 101, "position_bottom", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 102, "position_bottom", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 1, "state_top", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE210_inpjmc0h_state_top_dp1, sizeof(kEnum__TZE210_inpjmc0h_state_top_dp1)/sizeof(kEnum__TZE210_inpjmc0h_state_top_dp1[0]) },
    { 2, "position_top", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position_top", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE210_inpjmc0h{ kEntries__TZE210_inpjmc0h, 7 };
constexpr FzConverter kFzDp__TZE210_inpjmc0h{
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
    .user_config       = &kMap__TZE210_inpjmc0h,
};
const FzConverter* const kFz__TZE210_inpjmc0h[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE210_inpjmc0h,
};
constexpr TzConverter kTzDp__TZE210_inpjmc0h{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE210_inpjmc0h,
};
const TzConverter* const kTz__TZE210_inpjmc0h[] = { &kTzDp__TZE210_inpjmc0h };
constexpr const char* kM__TZE210_inpjmc0h[] = { "TS0301" };
constexpr const char* kN__TZE210_inpjmc0h[] = { "_TZE210_inpjmc0h" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE210_inpjmc0h{
    .zigbee_models=kM__TZE210_inpjmc0h,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE210_inpjmc0h,.manufacturer_names_count=1,
    .model="TS0301__TZE210_inpjmc0h",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE210_inpjmc0h,
    .from_zigbee_count=sizeof(kFz__TZE210_inpjmc0h)/sizeof(kFz__TZE210_inpjmc0h[0]),
    .to_zigbee=kTz__TZE210_inpjmc0h,
    .to_zigbee_count=sizeof(kTz__TZE210_inpjmc0h)/sizeof(kTz__TZE210_inpjmc0h[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
