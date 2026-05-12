// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_loejka0i).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_loejka0i[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 2, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 15, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 102, "voltage_a", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "current_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 104, "power_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "voltage_b", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "current_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 107, "power_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "voltage_c", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 109, "current_c", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 110, "power_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "energy_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 114, "energy_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 116, "energy_c", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 113, "energy_produced_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 115, "energy_produced_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 117, "energy_produced_c", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 118, "power_factor_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 119, "power_factor_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 120, "power_factor_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_loejka0i{ kEntries__TZE204_loejka0i, 23 };
constexpr FzConverter kFzDp__TZE204_loejka0i{
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
    .user_config       = &kMap__TZE204_loejka0i,
};
const FzConverter* const kFz__TZE204_loejka0i[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_loejka0i,
};
constexpr TzConverter kTzDp__TZE204_loejka0i{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_loejka0i,
};
const TzConverter* const kTz__TZE204_loejka0i[] = { &kTzDp__TZE204_loejka0i };
constexpr const char* kM__TZE204_loejka0i[] = { "TS0601" };
constexpr const char* kN__TZE204_loejka0i[] = { "_TZE204_loejka0i" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_loejka0i{
    .zigbee_models=kM__TZE204_loejka0i,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_loejka0i,.manufacturer_names_count=1,
    .model="TS0601__TZE204_loejka0i",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_loejka0i,
    .from_zigbee_count=sizeof(kFz__TZE204_loejka0i)/sizeof(kFz__TZE204_loejka0i[0]),
    .to_zigbee=kTz__TZE204_loejka0i,
    .to_zigbee_count=sizeof(kTz__TZE204_loejka0i)/sizeof(kTz__TZE204_loejka0i[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
