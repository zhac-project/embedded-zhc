// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BOX EWS1154-Z (TS0601 / _TZE200_yp5tsi3y) — wall switch.
// Hand-spot-checked against z2m devices/box.ts during 2026-04-28 BOX sweep.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::box {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp1[] = { { 0, "scene_1" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp2[] = { { 0, "scene_2" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp3[] = { { 0, "scene_3" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp4[] = { { 0, "scene_4" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp5[] = { { 0, "scene_5" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp6[] = { { 0, "scene_6" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp7[] = { { 0, "scene_7" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp8[] = { { 0, "scene_8" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp9[] = { { 0, "scene_9" } };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_action_dp10[] = { { 0, "scene_10" } };

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_record_rf_dp102[] = {
    { 0, "record" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_clear_rf_dp102[] = {
    { 1, "clear" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_rf_status_dp103[] = {
    { 0, "ok" },
    { 1, "error" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_record_scene_dp104[] = {
    { 0, "scene_1" },
    { 1, "scene_2" },
    { 2, "scene_3" },
    { 3, "scene_4" },
    { 4, "scene_5" },
    { 5, "scene_6" },
    { 6, "scene_7" },
    { 7, "scene_8" },
    { 8, "scene_9" },
    { 9, "scene_10" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_clear_scene_dp104[] = {
    { 10, "scene_1" },
    { 11, "scene_2" },
    { 12, "scene_3" },
    { 13, "scene_4" },
    { 14, "scene_5" },
    { 15, "scene_6" },
    { 16, "scene_7" },
    { 17, "scene_8" },
    { 18, "scene_9" },
    { 19, "scene_10" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yp5tsi3y_rf_scene_status_dp105[] = {
    { 0, "ok" },
    { 1, "error" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_yp5tsi3y[] = {
    { 19, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 1, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp1, sizeof(kEnum__TZE200_yp5tsi3y_action_dp1)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp1[0]) },
    { 2, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp2, sizeof(kEnum__TZE200_yp5tsi3y_action_dp2)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp2[0]) },
    { 3, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp3, sizeof(kEnum__TZE200_yp5tsi3y_action_dp3)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp3[0]) },
    { 4, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp4, sizeof(kEnum__TZE200_yp5tsi3y_action_dp4)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp4[0]) },
    { 5, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp5, sizeof(kEnum__TZE200_yp5tsi3y_action_dp5)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp5[0]) },
    { 6, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp6, sizeof(kEnum__TZE200_yp5tsi3y_action_dp6)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp6[0]) },
    { 7, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp7, sizeof(kEnum__TZE200_yp5tsi3y_action_dp7)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp7[0]) },
    { 8, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp8, sizeof(kEnum__TZE200_yp5tsi3y_action_dp8)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp8[0]) },
    { 9, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp9, sizeof(kEnum__TZE200_yp5tsi3y_action_dp9)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp9[0]) },
    { 10, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_action_dp10, sizeof(kEnum__TZE200_yp5tsi3y_action_dp10)/sizeof(kEnum__TZE200_yp5tsi3y_action_dp10[0]) },
    { 102, "record_rf", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_record_rf_dp102, sizeof(kEnum__TZE200_yp5tsi3y_record_rf_dp102)/sizeof(kEnum__TZE200_yp5tsi3y_record_rf_dp102[0]) },
    { 102, "clear_rf", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_clear_rf_dp102, sizeof(kEnum__TZE200_yp5tsi3y_clear_rf_dp102)/sizeof(kEnum__TZE200_yp5tsi3y_clear_rf_dp102[0]) },
    { 103, "rf_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_rf_status_dp103, sizeof(kEnum__TZE200_yp5tsi3y_rf_status_dp103)/sizeof(kEnum__TZE200_yp5tsi3y_rf_status_dp103[0]) },
    { 104, "record_scene", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_record_scene_dp104, sizeof(kEnum__TZE200_yp5tsi3y_record_scene_dp104)/sizeof(kEnum__TZE200_yp5tsi3y_record_scene_dp104[0]) },
    { 104, "clear_scene", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_clear_scene_dp104, sizeof(kEnum__TZE200_yp5tsi3y_clear_scene_dp104)/sizeof(kEnum__TZE200_yp5tsi3y_clear_scene_dp104[0]) },
    { 105, "rf_scene_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yp5tsi3y_rf_scene_status_dp105, sizeof(kEnum__TZE200_yp5tsi3y_rf_scene_status_dp105)/sizeof(kEnum__TZE200_yp5tsi3y_rf_scene_status_dp105[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_yp5tsi3y{ kEntries__TZE200_yp5tsi3y, sizeof(kEntries__TZE200_yp5tsi3y)/sizeof(kEntries__TZE200_yp5tsi3y[0]) };
constexpr FzConverter kFzDp__TZE200_yp5tsi3y{
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
    .user_config       = &kMap__TZE200_yp5tsi3y,
};
const FzConverter* const kFz__TZE200_yp5tsi3y[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_yp5tsi3y,
};
constexpr TzConverter kTzDp__TZE200_yp5tsi3y{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_yp5tsi3y,
};
const TzConverter* const kTz__TZE200_yp5tsi3y[] = { &kTzDp__TZE200_yp5tsi3y };
constexpr const char* kM__TZE200_yp5tsi3y[] = { "TS0601" };
constexpr const char* kN__TZE200_yp5tsi3y[] = { "_TZE200_yp5tsi3y" };
}  // namespace

// --- hand-tuned exposes (BOX sweep 2026-04-28p) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"record_rf", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"clear_rf", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rf_status", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"record_scene", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"clear_scene", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rf_scene_status", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end hand-tuned block ---

extern const PreparedDefinition kDefBox__TZE200_yp5tsi3y{
    .zigbee_models=kM__TZE200_yp5tsi3y,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_yp5tsi3y,.manufacturer_names_count=1,
    .model="EWS1154-Z",.vendor="BOX",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_yp5tsi3y,
    .from_zigbee_count=sizeof(kFz__TZE200_yp5tsi3y)/sizeof(kFz__TZE200_yp5tsi3y[0]),
    .to_zigbee=kTz__TZE200_yp5tsi3y,
    .to_zigbee_count=sizeof(kTz__TZE200_yp5tsi3y)/sizeof(kTz__TZE200_yp5tsi3y[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::box
