// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_mfamvsdb).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mfamvsdb_action_dp1[] = {
    { 0, "scene_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mfamvsdb_action_dp2[] = {
    { 0, "scene_2" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mfamvsdb_action_dp3[] = {
    { 0, "scene_3" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mfamvsdb_action_dp4[] = {
    { 0, "scene_4" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_mfamvsdb[] = {
    { 1, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mfamvsdb_action_dp1, sizeof(kEnum__TZE200_mfamvsdb_action_dp1)/sizeof(kEnum__TZE200_mfamvsdb_action_dp1[0]) },
    { 2, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mfamvsdb_action_dp2, sizeof(kEnum__TZE200_mfamvsdb_action_dp2)/sizeof(kEnum__TZE200_mfamvsdb_action_dp2[0]) },
    { 3, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mfamvsdb_action_dp3, sizeof(kEnum__TZE200_mfamvsdb_action_dp3)/sizeof(kEnum__TZE200_mfamvsdb_action_dp3[0]) },
    { 4, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mfamvsdb_action_dp4, sizeof(kEnum__TZE200_mfamvsdb_action_dp4)/sizeof(kEnum__TZE200_mfamvsdb_action_dp4[0]) },
    { 108, "vibration", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "approach", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "illumination", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_mfamvsdb{ kEntries__TZE200_mfamvsdb, 8 };
constexpr FzConverter kFzDp__TZE200_mfamvsdb{
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
    .user_config       = &kMap__TZE200_mfamvsdb,
};
const FzConverter* const kFz__TZE200_mfamvsdb[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_mfamvsdb,
};
constexpr TzConverter kTzDp__TZE200_mfamvsdb{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_mfamvsdb,
};
const TzConverter* const kTz__TZE200_mfamvsdb[] = { &kTzDp__TZE200_mfamvsdb };
constexpr const char* kM__TZE200_mfamvsdb[] = { "TS0601" };
constexpr const char* kN__TZE200_mfamvsdb[] = { "_TZE200_mfamvsdb" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_mfamvsdb_4[] = {"scene_1", "scene_2", "scene_3", "scene_4"};
constexpr Expose kAutoExposes__TZE200_mfamvsdb[] = {
    {"vibration", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"approach", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"illumination", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"backlight", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_mfamvsdb_4, 4, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_mfamvsdb{
    .zigbee_models=kM__TZE200_mfamvsdb,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_mfamvsdb,.manufacturer_names_count=1,
    .model="TS0601__TZE200_mfamvsdb",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_mfamvsdb,.exposes_count=sizeof(kAutoExposes__TZE200_mfamvsdb)/sizeof(kAutoExposes__TZE200_mfamvsdb[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_mfamvsdb,
    .from_zigbee_count=sizeof(kFz__TZE200_mfamvsdb)/sizeof(kFz__TZE200_mfamvsdb[0]),
    .to_zigbee=kTz__TZE200_mfamvsdb,
    .to_zigbee_count=sizeof(kTz__TZE200_mfamvsdb)/sizeof(kTz__TZE200_mfamvsdb[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
