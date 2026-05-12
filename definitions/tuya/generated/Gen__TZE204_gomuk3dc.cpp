// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_gomuk3dc).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_gomuk3dc[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 29, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "voltage_a", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "current_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 105, "power_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "power_factor_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "energy_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 110, "energy_produced_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 112, "voltage_b", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 113, "current_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 114, "power_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "power_factor_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "energy_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 119, "energy_produced_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 121, "voltage_c", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 122, "current_c", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 123, "power_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 126, "power_factor_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 127, "energy_c", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 128, "energy_produced_c", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_gomuk3dc{ kEntries__TZE204_gomuk3dc, 23 };
constexpr FzConverter kFzDp__TZE204_gomuk3dc{
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
    .user_config       = &kMap__TZE204_gomuk3dc,
};
const FzConverter* const kFz__TZE204_gomuk3dc[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_gomuk3dc,
};
constexpr TzConverter kTzDp__TZE204_gomuk3dc{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_gomuk3dc,
};
const TzConverter* const kTz__TZE204_gomuk3dc[] = { &kTzDp__TZE204_gomuk3dc };
constexpr const char* kM__TZE204_gomuk3dc[] = { "TS0601" };
constexpr const char* kN__TZE204_gomuk3dc[] = { "_TZE204_gomuk3dc" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_gomuk3dc{
    .zigbee_models=kM__TZE204_gomuk3dc,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_gomuk3dc,.manufacturer_names_count=1,
    .model="TS0601__TZE204_gomuk3dc",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_gomuk3dc,
    .from_zigbee_count=sizeof(kFz__TZE204_gomuk3dc)/sizeof(kFz__TZE204_gomuk3dc[0]),
    .to_zigbee=kTz__TZE204_gomuk3dc,
    .to_zigbee_count=sizeof(kTz__TZE204_gomuk3dc)/sizeof(kTz__TZE204_gomuk3dc[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
