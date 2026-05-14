// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_wbhaespm).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_wbhaespm[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 17, "overload_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "overload_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "leakage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "leakage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "high_temperature_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "high_temperature_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_current_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_voltage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "under_voltage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "insufficient_balance_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "insufficient_balance_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "leakage_test", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_wbhaespm{ kEntries__TZE200_wbhaespm, 18 };
constexpr FzConverter kFzDp__TZE200_wbhaespm{
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
    .user_config       = &kMap__TZE200_wbhaespm,
};
const FzConverter* const kFz__TZE200_wbhaespm[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_wbhaespm,
};
constexpr TzConverter kTzDp__TZE200_wbhaespm{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_wbhaespm,
};
const TzConverter* const kTz__TZE200_wbhaespm[] = { &kTzDp__TZE200_wbhaespm };
constexpr const char* kM__TZE200_wbhaespm[] = { "TS0601" };
constexpr const char* kN__TZE200_wbhaespm[] = { "_TZE200_wbhaespm" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_wbhaespm[] = {
    {"SUTON","STB3L-125/ZJ"},
};
extern const PreparedDefinition kDefGen__TZE200_wbhaespm{
    .zigbee_models=kM__TZE200_wbhaespm,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_wbhaespm,.manufacturer_names_count=1,
    .model="TS0601__TZE200_wbhaespm",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_wbhaespm, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_wbhaespm)/sizeof(kWhiteLabels_Gen_TZE200_wbhaespm[0]),
    .from_zigbee=kFz__TZE200_wbhaespm,
    .from_zigbee_count=sizeof(kFz__TZE200_wbhaespm)/sizeof(kFz__TZE200_wbhaespm[0]),
    .to_zigbee=kTz__TZE200_wbhaespm,
    .to_zigbee_count=sizeof(kTz__TZE200_wbhaespm)/sizeof(kTz__TZE200_wbhaespm[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
