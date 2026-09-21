// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_5nldle7w).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_5nldle7w_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_5nldle7w_calibration_dp3[] = {
    { 0, "START" },
    { 1, "END" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_5nldle7w_motor_steering_dp8[] = {
    { 0, "FORWARD" },
    { 1, "BACKWARD" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_5nldle7w[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_5nldle7w_state_dp1, sizeof(kEnum__TZE200_5nldle7w_state_dp1)/sizeof(kEnum__TZE200_5nldle7w_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "calibration", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_5nldle7w_calibration_dp3, sizeof(kEnum__TZE200_5nldle7w_calibration_dp3)/sizeof(kEnum__TZE200_5nldle7w_calibration_dp3[0]) },
    { 7, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "motor_steering", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_5nldle7w_motor_steering_dp8, sizeof(kEnum__TZE200_5nldle7w_motor_steering_dp8)/sizeof(kEnum__TZE200_5nldle7w_motor_steering_dp8[0]) },
    { 101, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_5nldle7w{ kEntries__TZE200_5nldle7w, 8 };
constexpr FzConverter kFzDp__TZE200_5nldle7w{
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
    .user_config       = &kMap__TZE200_5nldle7w,
};
const FzConverter* const kFz__TZE200_5nldle7w[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_5nldle7w,
};
constexpr TzConverter kTzDp__TZE200_5nldle7w{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_5nldle7w,
};
const TzConverter* const kTz__TZE200_5nldle7w[] = { &kTzDp__TZE200_5nldle7w };
constexpr const char* kM__TZE200_5nldle7w[] = { "TS0601" };
constexpr const char* kN__TZE200_5nldle7w[] = { "_TZE200_5nldle7w" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_5nldle7w_0[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE200_5nldle7w_4[] = {"START", "END"};
constexpr const char* kAutoOpts__TZE200_5nldle7w_6[] = {"FORWARD", "BACKWARD"};
constexpr Expose kAutoExposes__TZE200_5nldle7w[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_5nldle7w_0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"calibration", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_5nldle7w_4, 2},
    {"backlight_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"motor_steering", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_5nldle7w_6, 2},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_5nldle7w[] = {
    {"Homeetec","37022173"},
};
extern const PreparedDefinition kDefGen__TZE200_5nldle7w{
    .zigbee_models=kM__TZE200_5nldle7w,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_5nldle7w,.manufacturer_names_count=1,
    .model="TS0601__TZE200_5nldle7w",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_5nldle7w,.exposes_count=sizeof(kAutoExposes__TZE200_5nldle7w)/sizeof(kAutoExposes__TZE200_5nldle7w[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_5nldle7w, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_5nldle7w)/sizeof(kWhiteLabels_Gen_TZE200_5nldle7w[0]),
    .from_zigbee=kFz__TZE200_5nldle7w,
    .from_zigbee_count=sizeof(kFz__TZE200_5nldle7w)/sizeof(kFz__TZE200_5nldle7w[0]),
    .to_zigbee=kTz__TZE200_5nldle7w,
    .to_zigbee_count=sizeof(kTz__TZE200_5nldle7w)/sizeof(kTz__TZE200_5nldle7w[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
