// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_fzo2pocs).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_fzo2pocs_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_fzo2pocs_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_fzo2pocs_motor_working_mode_dp106[] = {
    { 0, "continuous" },
    { 1, "intermittently" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_fzo2pocs[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_fzo2pocs_state_dp1, sizeof(kEnum__TZE284_fzo2pocs_state_dp1)/sizeof(kEnum__TZE284_fzo2pocs_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_fzo2pocs_reverse_direction_dp5, sizeof(kEnum__TZE284_fzo2pocs_reverse_direction_dp5)/sizeof(kEnum__TZE284_fzo2pocs_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "motor_working_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_fzo2pocs_motor_working_mode_dp106, sizeof(kEnum__TZE284_fzo2pocs_motor_working_mode_dp106)/sizeof(kEnum__TZE284_fzo2pocs_motor_working_mode_dp106[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_fzo2pocs{ kEntries__TZE284_fzo2pocs, 6 };
constexpr FzConverter kFzDp__TZE284_fzo2pocs{
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
    .user_config       = &kMap__TZE284_fzo2pocs,
};
const FzConverter* const kFz__TZE284_fzo2pocs[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_fzo2pocs,
};
constexpr TzConverter kTzDp__TZE284_fzo2pocs{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_fzo2pocs,
};
const TzConverter* const kTz__TZE284_fzo2pocs[] = { &kTzDp__TZE284_fzo2pocs };
constexpr const char* kM__TZE284_fzo2pocs[] = { "TS0601" };
constexpr const char* kN__TZE284_fzo2pocs[] = { "_TZE284_fzo2pocs" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_fzo2pocs_0[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE284_fzo2pocs_2[] = {"forward", "back"};
constexpr const char* kAutoOpts__TZE284_fzo2pocs_4[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE284_fzo2pocs_5[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE284_fzo2pocs_6[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE284_fzo2pocs_7[] = {"continuous", "intermittently"};
constexpr Expose kAutoExposes__TZE284_fzo2pocs[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_fzo2pocs_0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"reverse_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_fzo2pocs_2, 2},
    {"motor_fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"upper_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_fzo2pocs_4, 2, ExposeCategory::Config},
    {"middle_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_fzo2pocs_5, 2, ExposeCategory::Config},
    {"lower_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_fzo2pocs_6, 2, ExposeCategory::Config},
    {"motor_working_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_fzo2pocs_7, 2, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_fzo2pocs[] = {
    {"Lilistore","TS0601_lilistore"},
    {"Zemismart","ZM90E-DT250N/A400"},
    {"Nova Digital","CMR-1"},
};
extern const PreparedDefinition kDefGen__TZE284_fzo2pocs{
    .zigbee_models=kM__TZE284_fzo2pocs,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_fzo2pocs,.manufacturer_names_count=1,
    .model="TS0601__TZE284_fzo2pocs",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_fzo2pocs,.exposes_count=sizeof(kAutoExposes__TZE284_fzo2pocs)/sizeof(kAutoExposes__TZE284_fzo2pocs[0]),
    .white_labels=kWhiteLabels_Gen_TZE284_fzo2pocs, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_fzo2pocs)/sizeof(kWhiteLabels_Gen_TZE284_fzo2pocs[0]),
    .from_zigbee=kFz__TZE284_fzo2pocs,
    .from_zigbee_count=sizeof(kFz__TZE284_fzo2pocs)/sizeof(kFz__TZE284_fzo2pocs[0]),
    .to_zigbee=kTz__TZE284_fzo2pocs,
    .to_zigbee_count=sizeof(kTz__TZE284_fzo2pocs)/sizeof(kTz__TZE284_fzo2pocs[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
