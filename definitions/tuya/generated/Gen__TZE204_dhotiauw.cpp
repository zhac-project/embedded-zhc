// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_dhotiauw).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_dhotiauw[] = {
    { 105, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 103, "power_a", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "power_b", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "power", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 20, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "current_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 102, "current_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 120, "power_factor_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 121, "power_factor_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "energy_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 117, "energy_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 122, "update_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 123, "calibration_voltage", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 125, "calibration_current_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 126, "calibration_power_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 130, "calibration_current_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 131, "calibration_power_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 124, "calibration_ac_frequency", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_dhotiauw{ kEntries__TZE204_dhotiauw, 18 };
constexpr FzConverter kFzDp__TZE204_dhotiauw{
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
    .user_config       = &kMap__TZE204_dhotiauw,
};
const FzConverter* const kFz__TZE204_dhotiauw[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_dhotiauw,
};
constexpr TzConverter kTzDp__TZE204_dhotiauw{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_dhotiauw,
};
const TzConverter* const kTz__TZE204_dhotiauw[] = { &kTzDp__TZE204_dhotiauw };
constexpr const char* kM__TZE204_dhotiauw[] = { "TS0601" };
constexpr const char* kN__TZE204_dhotiauw[] = { "_TZE204_dhotiauw" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_dhotiauw{
    .zigbee_models=kM__TZE204_dhotiauw,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_dhotiauw,.manufacturer_names_count=1,
    .model="TS0601__TZE204_dhotiauw",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_dhotiauw,
    .from_zigbee_count=sizeof(kFz__TZE204_dhotiauw)/sizeof(kFz__TZE204_dhotiauw[0]),
    .to_zigbee=kTz__TZE204_dhotiauw,
    .to_zigbee_count=sizeof(kTz__TZE204_dhotiauw)/sizeof(kTz__TZE204_dhotiauw[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
