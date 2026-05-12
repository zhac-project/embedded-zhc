// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_o9ofysmo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_o9ofysmo_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_o9ofysmo[] = {
    { 3, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_o9ofysmo_battery_state_dp14, sizeof(kEnum__TZE284_o9ofysmo_battery_state_dp14)/sizeof(kEnum__TZE284_o9ofysmo_battery_state_dp14[0]) },
    { 101, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "report_interval", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_o9ofysmo{ kEntries__TZE284_o9ofysmo, 7 };
constexpr FzConverter kFzDp__TZE284_o9ofysmo{
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
    .user_config       = &kMap__TZE284_o9ofysmo,
};
const FzConverter* const kFz__TZE284_o9ofysmo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_o9ofysmo,
};
constexpr TzConverter kTzDp__TZE284_o9ofysmo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_o9ofysmo,
};
const TzConverter* const kTz__TZE284_o9ofysmo[] = { &kTzDp__TZE284_o9ofysmo };
constexpr const char* kM__TZE284_o9ofysmo[] = { "TS0601" };
constexpr const char* kN__TZE284_o9ofysmo[] = { "_TZE284_o9ofysmo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_o9ofysmo{
    .zigbee_models=kM__TZE284_o9ofysmo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_o9ofysmo,.manufacturer_names_count=1,
    .model="TS0601__TZE284_o9ofysmo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_o9ofysmo,
    .from_zigbee_count=sizeof(kFz__TZE284_o9ofysmo)/sizeof(kFz__TZE284_o9ofysmo[0]),
    .to_zigbee=kTz__TZE284_o9ofysmo,
    .to_zigbee_count=sizeof(kTz__TZE284_o9ofysmo)/sizeof(kTz__TZE284_o9ofysmo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
