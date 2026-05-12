// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_eaac7dkw).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_eaac7dkw[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "total_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 102, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 103, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 109, "energy_reactive", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 110, "power_reactive", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "power_factor", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_eaac7dkw{ kEntries__TZE200_eaac7dkw, 9 };
constexpr FzConverter kFzDp__TZE200_eaac7dkw{
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
    .user_config       = &kMap__TZE200_eaac7dkw,
};
const FzConverter* const kFz__TZE200_eaac7dkw[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_eaac7dkw,
};
constexpr TzConverter kTzDp__TZE200_eaac7dkw{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_eaac7dkw,
};
const TzConverter* const kTz__TZE200_eaac7dkw[] = { &kTzDp__TZE200_eaac7dkw };
constexpr const char* kM__TZE200_eaac7dkw[] = { "TS0601" };
constexpr const char* kN__TZE200_eaac7dkw[] = { "_TZE200_eaac7dkw" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_eaac7dkw{
    .zigbee_models=kM__TZE200_eaac7dkw,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_eaac7dkw,.manufacturer_names_count=1,
    .model="TS0601__TZE200_eaac7dkw",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_eaac7dkw,
    .from_zigbee_count=sizeof(kFz__TZE200_eaac7dkw)/sizeof(kFz__TZE200_eaac7dkw[0]),
    .to_zigbee=kTz__TZE200_eaac7dkw,
    .to_zigbee_count=sizeof(kTz__TZE200_eaac7dkw)/sizeof(kTz__TZE200_eaac7dkw[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
