// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_nslr42tt).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_nslr42tt[] = {
    { 132, "ac_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 133, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "energy_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 111, "energy_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 121, "energy_c", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 131, "current", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 9, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "power_factor_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "power_factor_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 122, "power_factor_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 134, "device_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 135, "ac_frequency_high_precision", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_nslr42tt{ kEntries__TZE200_nslr42tt, 13 };
constexpr FzConverter kFzDp__TZE200_nslr42tt{
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
    .user_config       = &kMap__TZE200_nslr42tt,
};
const FzConverter* const kFz__TZE200_nslr42tt[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_nslr42tt,
};
constexpr TzConverter kTzDp__TZE200_nslr42tt{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_nslr42tt,
};
const TzConverter* const kTz__TZE200_nslr42tt[] = { &kTzDp__TZE200_nslr42tt };
constexpr const char* kM__TZE200_nslr42tt[] = { "TS0601" };
constexpr const char* kN__TZE200_nslr42tt[] = { "_TZE200_nslr42tt" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_nslr42tt[] = {
    {"MatSee Plus","PC321-Z-TY"},
    {"OWON","PC321-Z-TY"},
};
extern const PreparedDefinition kDefGen__TZE200_nslr42tt{
    .zigbee_models=kM__TZE200_nslr42tt,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_nslr42tt,.manufacturer_names_count=1,
    .model="TS0601__TZE200_nslr42tt",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_nslr42tt, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_nslr42tt)/sizeof(kWhiteLabels_Gen_TZE200_nslr42tt[0]),
    .from_zigbee=kFz__TZE200_nslr42tt,
    .from_zigbee_count=sizeof(kFz__TZE200_nslr42tt)/sizeof(kFz__TZE200_nslr42tt[0]),
    .to_zigbee=kTz__TZE200_nslr42tt,
    .to_zigbee_count=sizeof(kTz__TZE200_nslr42tt)/sizeof(kTz__TZE200_nslr42tt[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
