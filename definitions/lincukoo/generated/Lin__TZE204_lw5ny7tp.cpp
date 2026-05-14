// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/lincukoo.ts (fingerprint TS0601 / _TZE204_lw5ny7tp).
// Lincukoo SZLR08 — 24GHz millimeter wave radar.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::lincukoo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lw5ny7tp_relay_mode_dp106[] = {
    { 0, "auto" },
    { 1, "manual" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lw5ny7tp_radar_mode_dp107[] = {
    { 0, "people_on" },
    { 1, "people_off" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_lw5ny7tp[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 20, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "installation_height", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "detection_distance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "indicator", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 104, "relay_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "radar_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "relay_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lw5ny7tp_relay_mode_dp106, sizeof(kEnum__TZE204_lw5ny7tp_relay_mode_dp106)/sizeof(kEnum__TZE204_lw5ny7tp_relay_mode_dp106[0]) },
    { 107, "radar_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lw5ny7tp_radar_mode_dp107, sizeof(kEnum__TZE204_lw5ny7tp_radar_mode_dp107)/sizeof(kEnum__TZE204_lw5ny7tp_radar_mode_dp107[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_lw5ny7tp{ kEntries__TZE204_lw5ny7tp, 11 };
constexpr FzConverter kFzDp__TZE204_lw5ny7tp{
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
    .user_config       = &kMap__TZE204_lw5ny7tp,
};
const FzConverter* const kFz__TZE204_lw5ny7tp[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_lw5ny7tp,
};
constexpr TzConverter kTzDp__TZE204_lw5ny7tp{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_lw5ny7tp,
};
const TzConverter* const kTz__TZE204_lw5ny7tp[] = { &kTzDp__TZE204_lw5ny7tp };
constexpr const char* kM__TZE204_lw5ny7tp[] = { "TS0601" };
constexpr const char* kN__TZE204_lw5ny7tp[] = { "_TZE204_lw5ny7tp" };
}  // namespace

// --- hand-authored exposes for SZLR08 ---
constexpr Expose kAutoExposes[] = {
    {"presence", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"detection_distance", ExposeType::Numeric, Access::State, "cm", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"installation_height", ExposeType::Numeric, Access::StateSet, "m", nullptr, nullptr, 0},
    {"radar_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fading_time", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
    {"relay_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"radar_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"indicator", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"relay_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"radar_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end ---

extern const PreparedDefinition kDefLin__TZE204_lw5ny7tp{
    .zigbee_models=kM__TZE204_lw5ny7tp,.zigbee_models_count=sizeof(kM__TZE204_lw5ny7tp)/sizeof(kM__TZE204_lw5ny7tp[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_lw5ny7tp,.manufacturer_names_count=1,
    .model="SZLR08",.vendor="Lincukoo",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_lw5ny7tp,
    .from_zigbee_count=sizeof(kFz__TZE204_lw5ny7tp)/sizeof(kFz__TZE204_lw5ny7tp[0]),
    .to_zigbee=kTz__TZE204_lw5ny7tp,
    .to_zigbee_count=sizeof(kTz__TZE204_lw5ny7tp)/sizeof(kTz__TZE204_lw5ny7tp[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::lincukoo
