// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_m64smti7).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_m64smti7[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 17, "high_temperature_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "high_temperature_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_current_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_voltage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "under_voltage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_m64smti7{ kEntries__TZE204_m64smti7, 11 };
constexpr FzConverter kFzDp__TZE204_m64smti7{
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
    .user_config       = &kMap__TZE204_m64smti7,
};
const FzConverter* const kFz__TZE204_m64smti7[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_m64smti7,
};
constexpr TzConverter kTzDp__TZE204_m64smti7{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_m64smti7,
};
const TzConverter* const kTz__TZE204_m64smti7[] = { &kTzDp__TZE204_m64smti7 };
constexpr const char* kM__TZE204_m64smti7[] = { "TS0601" };
constexpr const char* kN__TZE204_m64smti7[] = { "_TZE204_m64smti7" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_m64smti7{
    .zigbee_models=kM__TZE204_m64smti7,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_m64smti7,.manufacturer_names_count=1,
    .model="TS0601__TZE204_m64smti7",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_m64smti7,
    .from_zigbee_count=sizeof(kFz__TZE204_m64smti7)/sizeof(kFz__TZE204_m64smti7[0]),
    .to_zigbee=kTz__TZE204_m64smti7,
    .to_zigbee_count=sizeof(kTz__TZE204_m64smti7)/sizeof(kTz__TZE204_m64smti7[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
