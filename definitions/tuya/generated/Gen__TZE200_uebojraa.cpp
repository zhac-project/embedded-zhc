// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_uebojraa).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_uebojraa_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_uebojraa[] = {
    { 1, "smoke", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_uebojraa_battery_state_dp14, sizeof(kEnum__TZE200_uebojraa_battery_state_dp14)/sizeof(kEnum__TZE200_uebojraa_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_uebojraa{ kEntries__TZE200_uebojraa, 3 };
constexpr FzConverter kFzDp__TZE200_uebojraa{
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
    .user_config       = &kMap__TZE200_uebojraa,
};
const FzConverter* const kFz__TZE200_uebojraa[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_uebojraa,
};
constexpr TzConverter kTzDp__TZE200_uebojraa{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_uebojraa,
};
const TzConverter* const kTz__TZE200_uebojraa[] = { &kTzDp__TZE200_uebojraa };
constexpr const char* kM__TZE200_uebojraa[] = { "TS0601" };
constexpr const char* kN__TZE200_uebojraa[] = { "_TZE200_uebojraa" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_uebojraa{
    .zigbee_models=kM__TZE200_uebojraa,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_uebojraa,.manufacturer_names_count=1,
    .model="TS0601__TZE200_uebojraa",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_uebojraa,
    .from_zigbee_count=sizeof(kFz__TZE200_uebojraa)/sizeof(kFz__TZE200_uebojraa[0]),
    .to_zigbee=kTz__TZE200_uebojraa,
    .to_zigbee_count=sizeof(kTz__TZE200_uebojraa)/sizeof(kTz__TZE200_uebojraa[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
