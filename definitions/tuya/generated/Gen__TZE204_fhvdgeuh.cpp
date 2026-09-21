// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_fhvdgeuh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_fhvdgeuh[] = {
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 15, "leakage", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_fhvdgeuh{ kEntries__TZE204_fhvdgeuh, 4 };
constexpr FzConverter kFzDp__TZE204_fhvdgeuh{
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
    .user_config       = &kMap__TZE204_fhvdgeuh,
};
const FzConverter* const kFz__TZE204_fhvdgeuh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_fhvdgeuh,
};
constexpr TzConverter kTzDp__TZE204_fhvdgeuh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_fhvdgeuh,
};
const TzConverter* const kTz__TZE204_fhvdgeuh[] = { &kTzDp__TZE204_fhvdgeuh };
constexpr const char* kM__TZE204_fhvdgeuh[] = { "TS0601" };
constexpr const char* kN__TZE204_fhvdgeuh[] = { "_TZE204_fhvdgeuh" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_fhvdgeuh_5[] = {"off", "previous", "on"};
constexpr Expose kAutoExposes__TZE204_fhvdgeuh[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_fhvdgeuh_5, 3, ExposeCategory::Config},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"leakage", ExposeType::Numeric, Access::State, "mA", nullptr, nullptr, 0},
    {"reclosing_allowed_times", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 30, 0},
    {"reclosing_enable", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"timer", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 0},
    {"clear_energy", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"status", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_fhvdgeuh[] = {
    {"RTX","TS0601_RTX_DIN"},
};
extern const PreparedDefinition kDefGen__TZE204_fhvdgeuh{
    .zigbee_models=kM__TZE204_fhvdgeuh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_fhvdgeuh,.manufacturer_names_count=1,
    .model="TS0601__TZE204_fhvdgeuh",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_fhvdgeuh,.exposes_count=sizeof(kAutoExposes__TZE204_fhvdgeuh)/sizeof(kAutoExposes__TZE204_fhvdgeuh[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_fhvdgeuh, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_fhvdgeuh)/sizeof(kWhiteLabels_Gen_TZE204_fhvdgeuh[0]),
    .from_zigbee=kFz__TZE204_fhvdgeuh,
    .from_zigbee_count=sizeof(kFz__TZE204_fhvdgeuh)/sizeof(kFz__TZE204_fhvdgeuh[0]),
    .to_zigbee=kTz__TZE204_fhvdgeuh,
    .to_zigbee_count=sizeof(kTz__TZE204_fhvdgeuh)/sizeof(kTz__TZE204_fhvdgeuh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
