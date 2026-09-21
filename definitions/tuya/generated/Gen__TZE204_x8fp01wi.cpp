// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_x8fp01wi).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_x8fp01wi[] = {
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 2, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 9, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_x8fp01wi{ kEntries__TZE204_x8fp01wi, 4 };
constexpr FzConverter kFzDp__TZE204_x8fp01wi{
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
    .user_config       = &kMap__TZE204_x8fp01wi,
};
const FzConverter* const kFz__TZE204_x8fp01wi[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_x8fp01wi,
};
constexpr TzConverter kTzDp__TZE204_x8fp01wi{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_x8fp01wi,
};
const TzConverter* const kTz__TZE204_x8fp01wi[] = { &kTzDp__TZE204_x8fp01wi };
constexpr const char* kM__TZE204_x8fp01wi[] = { "TS0601" };
constexpr const char* kN__TZE204_x8fp01wi[] = { "_TZE204_x8fp01wi" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE204_x8fp01wi[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"voltage_a", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_x8fp01wi[] = {
    {"Wenzhou Taiye Electric","TAC7361C BI"},
};
extern const PreparedDefinition kDefGen__TZE204_x8fp01wi{
    .zigbee_models=kM__TZE204_x8fp01wi,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_x8fp01wi,.manufacturer_names_count=1,
    .model="TS0601__TZE204_x8fp01wi",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_x8fp01wi,.exposes_count=sizeof(kAutoExposes__TZE204_x8fp01wi)/sizeof(kAutoExposes__TZE204_x8fp01wi[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_x8fp01wi, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_x8fp01wi)/sizeof(kWhiteLabels_Gen_TZE204_x8fp01wi[0]),
    .from_zigbee=kFz__TZE204_x8fp01wi,
    .from_zigbee_count=sizeof(kFz__TZE204_x8fp01wi)/sizeof(kFz__TZE204_x8fp01wi[0]),
    .to_zigbee=kTz__TZE204_x8fp01wi,
    .to_zigbee_count=sizeof(kTz__TZE204_x8fp01wi)/sizeof(kTz__TZE204_x8fp01wi[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
