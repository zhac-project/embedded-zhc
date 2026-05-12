// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/lincukoo.ts (fingerprint TS0601 / _TZE284_ajhu0zqb).
// Lincukoo SZW08 — Smart water leakage/lack alarm sensor.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::lincukoo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_ajhu0zqb_alarm_status_dp102[] = {
    { 0, "normal" },
    { 1, "alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_ajhu0zqb_mode_dp101[] = {
    { 0, "leakage" },
    { 1, "shortage" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_ajhu0zqb_alarm_ringtone_dp103[] = {
    { 0, "mute" },
    { 1, "ring1" },
    { 2, "ring2" },
    { 3, "ring3" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_ajhu0zqb[] = {
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_ajhu0zqb_mode_dp101, sizeof(kEnum__TZE284_ajhu0zqb_mode_dp101)/sizeof(kEnum__TZE284_ajhu0zqb_mode_dp101[0]) },
    { 102, "alarm_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_ajhu0zqb_alarm_status_dp102, sizeof(kEnum__TZE284_ajhu0zqb_alarm_status_dp102)/sizeof(kEnum__TZE284_ajhu0zqb_alarm_status_dp102[0]) },
    { 103, "alarm_ringtone", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_ajhu0zqb_alarm_ringtone_dp103, sizeof(kEnum__TZE284_ajhu0zqb_alarm_ringtone_dp103)/sizeof(kEnum__TZE284_ajhu0zqb_alarm_ringtone_dp103[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_ajhu0zqb{ kEntries__TZE284_ajhu0zqb, 4 };
constexpr FzConverter kFzDp__TZE284_ajhu0zqb{
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
    .user_config       = &kMap__TZE284_ajhu0zqb,
};
const FzConverter* const kFz__TZE284_ajhu0zqb[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_ajhu0zqb,
};
constexpr TzConverter kTzDp__TZE284_ajhu0zqb{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_ajhu0zqb,
};
const TzConverter* const kTz__TZE284_ajhu0zqb[] = { &kTzDp__TZE284_ajhu0zqb };
constexpr const char* kM__TZE284_ajhu0zqb[] = { "TS0601" };
constexpr const char* kN__TZE284_ajhu0zqb[] = { "_TZE284_ajhu0zqb" };
}  // namespace

// --- hand-authored exposes for SZW08 ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"alarm_status", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_ringtone", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end ---

extern const PreparedDefinition kDefLin__TZE284_ajhu0zqb{
    .zigbee_models=kM__TZE284_ajhu0zqb,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_ajhu0zqb,.manufacturer_names_count=1,
    .model="SZW08",.vendor="Lincukoo",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_ajhu0zqb,
    .from_zigbee_count=sizeof(kFz__TZE284_ajhu0zqb)/sizeof(kFz__TZE284_ajhu0zqb[0]),
    .to_zigbee=kTz__TZE284_ajhu0zqb,
    .to_zigbee_count=sizeof(kTz__TZE284_ajhu0zqb)/sizeof(kTz__TZE284_ajhu0zqb[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::lincukoo
