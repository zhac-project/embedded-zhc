// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_e2bedvo9).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_e2bedvo9_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_e2bedvo9[] = {
    { 1, "smoke", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 2, "smoke_concentration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 11, "fault_alarm", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_e2bedvo9_battery_state_dp14, sizeof(kEnum__TZE200_e2bedvo9_battery_state_dp14)/sizeof(kEnum__TZE200_e2bedvo9_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "self_test", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_e2bedvo9{ kEntries__TZE200_e2bedvo9, 7 };
constexpr FzConverter kFzDp__TZE200_e2bedvo9{
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
    .user_config       = &kMap__TZE200_e2bedvo9,
};
const FzConverter* const kFz__TZE200_e2bedvo9[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_e2bedvo9,
};
constexpr TzConverter kTzDp__TZE200_e2bedvo9{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_e2bedvo9,
};
const TzConverter* const kTz__TZE200_e2bedvo9[] = { &kTzDp__TZE200_e2bedvo9 };
constexpr const char* kM__TZE200_e2bedvo9[] = { "TS0601" };
constexpr const char* kN__TZE200_e2bedvo9[] = { "_TZE200_e2bedvo9" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_e2bedvo9_2[] = {"low", "medium", "high"};
constexpr Expose kAutoExposes__TZE200_e2bedvo9[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"fault_alarm", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_e2bedvo9_2, 3, ExposeCategory::Diagnostic},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"self_test", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"smoke_concentration", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_e2bedvo9{
    .zigbee_models=kM__TZE200_e2bedvo9,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_e2bedvo9,.manufacturer_names_count=1,
    .model="TS0601__TZE200_e2bedvo9",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_e2bedvo9,.exposes_count=sizeof(kAutoExposes__TZE200_e2bedvo9)/sizeof(kAutoExposes__TZE200_e2bedvo9[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_e2bedvo9,
    .from_zigbee_count=sizeof(kFz__TZE200_e2bedvo9)/sizeof(kFz__TZE200_e2bedvo9[0]),
    .to_zigbee=kTz__TZE200_e2bedvo9,
    .to_zigbee_count=sizeof(kTz__TZE200_e2bedvo9)/sizeof(kTz__TZE200_e2bedvo9[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
