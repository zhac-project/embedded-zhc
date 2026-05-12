// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_78ioiaml).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_78ioiaml[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 29, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "voltage_l1", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "current_l1", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 105, "power_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "power_factor_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "energy_l1", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 110, "energy_produced_l1", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 112, "voltage_l2", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 113, "current_l2", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 114, "power_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "power_factor_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "energy_l2", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 119, "energy_produced_l2", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_78ioiaml{ kEntries__TZE200_78ioiaml, 17 };
constexpr FzConverter kFzDp__TZE200_78ioiaml{
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
    .user_config       = &kMap__TZE200_78ioiaml,
};
const FzConverter* const kFz__TZE200_78ioiaml[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_78ioiaml,
};
constexpr TzConverter kTzDp__TZE200_78ioiaml{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_78ioiaml,
};
const TzConverter* const kTz__TZE200_78ioiaml[] = { &kTzDp__TZE200_78ioiaml };
constexpr const char* kM__TZE200_78ioiaml[] = { "TS0601" };
constexpr const char* kN__TZE200_78ioiaml[] = { "_TZE200_78ioiaml" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_78ioiaml{
    .zigbee_models=kM__TZE200_78ioiaml,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_78ioiaml,.manufacturer_names_count=1,
    .model="TS0601__TZE200_78ioiaml",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_78ioiaml,
    .from_zigbee_count=sizeof(kFz__TZE200_78ioiaml)/sizeof(kFz__TZE200_78ioiaml[0]),
    .to_zigbee=kTz__TZE200_78ioiaml,
    .to_zigbee_count=sizeof(kTz__TZE200_78ioiaml)/sizeof(kTz__TZE200_78ioiaml[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
