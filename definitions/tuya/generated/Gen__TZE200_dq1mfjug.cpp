// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_dq1mfjug).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq1mfjug_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_dq1mfjug[] = {
    { 1, "smoke", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq1mfjug_battery_state_dp14, sizeof(kEnum__TZE200_dq1mfjug_battery_state_dp14)/sizeof(kEnum__TZE200_dq1mfjug_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_dq1mfjug{ kEntries__TZE200_dq1mfjug, 3 };
constexpr FzConverter kFzDp__TZE200_dq1mfjug{
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
    .user_config       = &kMap__TZE200_dq1mfjug,
};
const FzConverter* const kFz__TZE200_dq1mfjug[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_dq1mfjug,
};
constexpr TzConverter kTzDp__TZE200_dq1mfjug{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_dq1mfjug,
};
const TzConverter* const kTz__TZE200_dq1mfjug[] = { &kTzDp__TZE200_dq1mfjug };
constexpr const char* kM__TZE200_dq1mfjug[] = { "TS0601" };
constexpr const char* kN__TZE200_dq1mfjug[] = { "_TZE200_dq1mfjug" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_dq1mfjug{
    .zigbee_models=kM__TZE200_dq1mfjug,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_dq1mfjug,.manufacturer_names_count=1,
    .model="TS0601__TZE200_dq1mfjug",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_dq1mfjug,
    .from_zigbee_count=sizeof(kFz__TZE200_dq1mfjug)/sizeof(kFz__TZE200_dq1mfjug[0]),
    .to_zigbee=kTz__TZE200_dq1mfjug,
    .to_zigbee_count=sizeof(kTz__TZE200_dq1mfjug)/sizeof(kTz__TZE200_dq1mfjug[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
