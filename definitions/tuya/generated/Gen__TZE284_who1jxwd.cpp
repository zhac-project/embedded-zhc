// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_who1jxwd).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_who1jxwd[] = {
    { 1, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "presence_distance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "radar_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 104, "pir_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "delay_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "led_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_who1jxwd{ kEntries__TZE284_who1jxwd, 9 };
constexpr FzConverter kFzDp__TZE284_who1jxwd{
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
    .user_config       = &kMap__TZE284_who1jxwd,
};
const FzConverter* const kFz__TZE284_who1jxwd[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_who1jxwd,
};
constexpr TzConverter kTzDp__TZE284_who1jxwd{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_who1jxwd,
};
const TzConverter* const kTz__TZE284_who1jxwd[] = { &kTzDp__TZE284_who1jxwd };
constexpr const char* kM__TZE284_who1jxwd[] = { "TS0601" };
constexpr const char* kN__TZE284_who1jxwd[] = { "_TZE284_who1jxwd" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE284_who1jxwd[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"presence_distance", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 3, 1},
    {"presence_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 3, 1},
    {"radar_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pir_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 3, 1},
    {"delay_time", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 10, 9600, 1},
    {"led_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"radar_threshold", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config, 5, 255, 1},
    {"pir_threshold", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config, 1, 250, 1},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_who1jxwd{
    .zigbee_models=kM__TZE284_who1jxwd,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_who1jxwd,.manufacturer_names_count=1,
    .model="TS0601__TZE284_who1jxwd",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_who1jxwd,.exposes_count=sizeof(kAutoExposes__TZE284_who1jxwd)/sizeof(kAutoExposes__TZE284_who1jxwd[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_who1jxwd,
    .from_zigbee_count=sizeof(kFz__TZE284_who1jxwd)/sizeof(kFz__TZE284_who1jxwd[0]),
    .to_zigbee=kTz__TZE284_who1jxwd,
    .to_zigbee_count=sizeof(kTz__TZE284_who1jxwd)/sizeof(kTz__TZE284_who1jxwd[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
