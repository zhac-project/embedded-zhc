// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_6teua268).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_6teua268[] = {
    { 16, "alarm_state", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "setup_mode", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "alarm_siren", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "alarm_siren_duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "vibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "vibration_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "vibration_siren_duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "vibration_siren", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "close_signal", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "transmission_power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "magnetic_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_6teua268{ kEntries__TZE284_6teua268, 12 };
constexpr FzConverter kFzDp__TZE284_6teua268{
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
    .user_config       = &kMap__TZE284_6teua268,
};
const FzConverter* const kFz__TZE284_6teua268[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_6teua268,
};
constexpr TzConverter kTzDp__TZE284_6teua268{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_6teua268,
};
const TzConverter* const kTz__TZE284_6teua268[] = { &kTzDp__TZE284_6teua268 };
constexpr const char* kM__TZE284_6teua268[] = { "TS0601" };
constexpr const char* kN__TZE284_6teua268[] = { "_TZE284_6teua268" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_6teua268_0[] = {"open", "closed", "tilted"};
constexpr Expose kAutoExposes__TZE284_6teua268[] = {
    {"opening_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_6teua268_0, 3},
    {"alarm_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"setup_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_siren", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_siren_duration", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 5, 180, 1},
    {"vibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"vibration_limit", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"vibration_siren", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"vibration_siren_duration", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 5, 180, 1},
    {"close_signal", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"transmission_power", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 11, 19, 1},
    {"magnetic_status", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_6teua268{
    .zigbee_models=kM__TZE284_6teua268,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_6teua268,.manufacturer_names_count=1,
    .model="TS0601__TZE284_6teua268",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_6teua268,.exposes_count=sizeof(kAutoExposes__TZE284_6teua268)/sizeof(kAutoExposes__TZE284_6teua268[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_6teua268,
    .from_zigbee_count=sizeof(kFz__TZE284_6teua268)/sizeof(kFz__TZE284_6teua268[0]),
    .to_zigbee=kTz__TZE284_6teua268,
    .to_zigbee_count=sizeof(kTz__TZE284_6teua268)/sizeof(kTz__TZE284_6teua268[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
