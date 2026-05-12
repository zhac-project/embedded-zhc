// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_rks0sgb7).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_rks0sgb7[] = {
    { 113, "ac_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "power_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "power_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 107, "current_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 110, "current_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 105, "current", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 109, "power_factor_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "power_factor_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "energy_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 103, "energy_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 102, "energy_produced_a", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 104, "energy_produced_b", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 2, "energy_produced", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_rks0sgb7{ kEntries__TZE200_rks0sgb7, 16 };
constexpr FzConverter kFzDp__TZE200_rks0sgb7{
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
    .user_config       = &kMap__TZE200_rks0sgb7,
};
const FzConverter* const kFz__TZE200_rks0sgb7[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_rks0sgb7,
};
constexpr TzConverter kTzDp__TZE200_rks0sgb7{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_rks0sgb7,
};
const TzConverter* const kTz__TZE200_rks0sgb7[] = { &kTzDp__TZE200_rks0sgb7 };
constexpr const char* kM__TZE200_rks0sgb7[] = { "TS0601" };
constexpr const char* kN__TZE200_rks0sgb7[] = { "_TZE200_rks0sgb7" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_rks0sgb7{
    .zigbee_models=kM__TZE200_rks0sgb7,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_rks0sgb7,.manufacturer_names_count=1,
    .model="TS0601__TZE200_rks0sgb7",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_rks0sgb7,
    .from_zigbee_count=sizeof(kFz__TZE200_rks0sgb7)/sizeof(kFz__TZE200_rks0sgb7[0]),
    .to_zigbee=kTz__TZE200_rks0sgb7,
    .to_zigbee_count=sizeof(kTz__TZE200_rks0sgb7)/sizeof(kTz__TZE200_rks0sgb7[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
