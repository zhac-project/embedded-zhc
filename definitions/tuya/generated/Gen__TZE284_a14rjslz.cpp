// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_a14rjslz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_a14rjslz[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 24, "total_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 29, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 30, "power_reactive", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_a14rjslz{ kEntries__TZE284_a14rjslz, 6 };
constexpr FzConverter kFzDp__TZE284_a14rjslz{
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
    .user_config       = &kMap__TZE284_a14rjslz,
};
const FzConverter* const kFz__TZE284_a14rjslz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_a14rjslz,
};
constexpr TzConverter kTzDp__TZE284_a14rjslz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_a14rjslz,
};
const TzConverter* const kTz__TZE284_a14rjslz[] = { &kTzDp__TZE284_a14rjslz };
constexpr const char* kM__TZE284_a14rjslz[] = { "TS0601" };
constexpr const char* kN__TZE284_a14rjslz[] = { "_TZE284_a14rjslz" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE284_a14rjslz[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"voltage_a", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"total_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_reactive", ExposeType::Numeric, Access::State, "VAR", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_a14rjslz[] = {
    {"Ourtop","ATMS100133Z"},
};
extern const PreparedDefinition kDefGen__TZE284_a14rjslz{
    .zigbee_models=kM__TZE284_a14rjslz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_a14rjslz,.manufacturer_names_count=1,
    .model="TS0601__TZE284_a14rjslz",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_a14rjslz,.exposes_count=sizeof(kAutoExposes__TZE284_a14rjslz)/sizeof(kAutoExposes__TZE284_a14rjslz[0]),
    .white_labels=kWhiteLabels_Gen_TZE284_a14rjslz, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_a14rjslz)/sizeof(kWhiteLabels_Gen_TZE284_a14rjslz[0]),
    .from_zigbee=kFz__TZE284_a14rjslz,
    .from_zigbee_count=sizeof(kFz__TZE284_a14rjslz)/sizeof(kFz__TZE284_a14rjslz[0]),
    .to_zigbee=kTz__TZE284_a14rjslz,
    .to_zigbee_count=sizeof(kTz__TZE284_a14rjslz)/sizeof(kTz__TZE284_a14rjslz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
