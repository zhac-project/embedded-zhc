// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_it9utkro).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_it9utkro[] = {
    { 2, "co2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "humidity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 20, "co2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "voc", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 22, "formaldehyd", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_it9utkro{ kEntries__TZE284_it9utkro, 6 };
constexpr FzConverter kFzDp__TZE284_it9utkro{
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
    .user_config       = &kMap__TZE284_it9utkro,
};
const FzConverter* const kFz__TZE284_it9utkro[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_it9utkro,
};
constexpr TzConverter kTzDp__TZE284_it9utkro{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_it9utkro,
};
const TzConverter* const kTz__TZE284_it9utkro[] = { &kTzDp__TZE284_it9utkro };
constexpr const char* kM__TZE284_it9utkro[] = { "TS0601" };
constexpr const char* kN__TZE284_it9utkro[] = { "_TZE284_it9utkro" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_it9utkro{
    .zigbee_models=kM__TZE284_it9utkro,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_it9utkro,.manufacturer_names_count=1,
    .model="TS0601__TZE284_it9utkro",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_it9utkro,
    .from_zigbee_count=sizeof(kFz__TZE284_it9utkro)/sizeof(kFz__TZE284_it9utkro[0]),
    .to_zigbee=kTz__TZE284_it9utkro,
    .to_zigbee_count=sizeof(kTz__TZE284_it9utkro)/sizeof(kTz__TZE284_it9utkro[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
