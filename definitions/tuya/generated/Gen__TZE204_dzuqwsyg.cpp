// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_dzuqwsyg).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_dzuqwsyg_fan_mode_dp28[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
    { 3, "auto" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_dzuqwsyg[] = {
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_dzuqwsyg_fan_mode_dp28, sizeof(kEnum__TZE204_dzuqwsyg_fan_mode_dp28)/sizeof(kEnum__TZE204_dzuqwsyg_fan_mode_dp28[0]) },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_dzuqwsyg{ kEntries__TZE204_dzuqwsyg, 7 };
constexpr FzConverter kFzDp__TZE204_dzuqwsyg{
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
    .user_config       = &kMap__TZE204_dzuqwsyg,
};
const FzConverter* const kFz__TZE204_dzuqwsyg[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_dzuqwsyg,
};
constexpr TzConverter kTzDp__TZE204_dzuqwsyg{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_dzuqwsyg,
};
const TzConverter* const kTz__TZE204_dzuqwsyg[] = { &kTzDp__TZE204_dzuqwsyg };
constexpr const char* kM__TZE204_dzuqwsyg[] = { "TS0601" };
constexpr const char* kN__TZE204_dzuqwsyg[] = { "_TZE204_dzuqwsyg" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_dzuqwsyg[] = {
    {"Tuya","BAC-003"},
};
extern const PreparedDefinition kDefGen__TZE204_dzuqwsyg{
    .zigbee_models=kM__TZE204_dzuqwsyg,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_dzuqwsyg,.manufacturer_names_count=1,
    .model="TS0601__TZE204_dzuqwsyg",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_dzuqwsyg, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_dzuqwsyg)/sizeof(kWhiteLabels_Gen_TZE204_dzuqwsyg[0]),
    .from_zigbee=kFz__TZE204_dzuqwsyg,
    .from_zigbee_count=sizeof(kFz__TZE204_dzuqwsyg)/sizeof(kFz__TZE204_dzuqwsyg[0]),
    .to_zigbee=kTz__TZE204_dzuqwsyg,
    .to_zigbee_count=sizeof(kTz__TZE204_dzuqwsyg)/sizeof(kTz__TZE204_dzuqwsyg[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
