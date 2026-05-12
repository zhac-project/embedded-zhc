// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes ZHT-SR — Smart ring thermostat
// z2m-source: moes.ts #ZHT-SR (line 1539, fingerprint TS0601 / _TZE204_lpedvtvr)
//
// Phase-2 fix (2026-04-27 / Phase-2 cleanup): replaced the misclassified
// Battery+OnOff stub with a real Tuya DP map matching z2m's
// `meta.tuyaDatapoints` table. The earlier port spuriously decoded random
// frames as `state=true` because it referenced kFzOnOff on a TS0601 (Tuya
// MCU) device that doesn't speak ZCL OnOff at all.
//
// DP map ported from z2m source (lines ~1605-1635). Special cases:
//   * DP 101 local_temperature_calibration uses /256 (non-typical Tuya
//     scaling). Encoded with divisor=256.
//   * DP 47 running_state is reversed lookup (heat:0, idle:1) — encoded as
//     enum.
//   * DP 39 child_lock uses lockUnlock semantics; emitted as Bool, the
//     receiving side maps to LOCK/UNLOCK.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZHT_SR_preset[] = {
    { 0, "Manual"           },
    { 1, "Temporary manual" },
    { 2, "Program"          },
    { 3, "Eco"              },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZHT_SR_sensor_mode[] = {
    { 0, "IN" },
    { 1, "AL" },
    { 2, "OU" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZHT_SR_running_state[] = {
    { 0, "heat" },
    { 1, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZHT_SR_screen_time[] = {
    { 0, "10_seconds" },
    { 1, "20_seconds" },
    { 2, "30_seconds" },
    { 3, "40_seconds" },
    { 4, "50_seconds" },
    { 5, "60_seconds" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_ZHT_SR[] = {
    { 1,   "system_mode",                    ::zhc::TuyaDpType::Bool,    1,   nullptr, 0, 0 },
    { 2,   "preset",                         ::zhc::TuyaDpType::Enum,    1,
        kEnum_ZHT_SR_preset, sizeof(kEnum_ZHT_SR_preset)/sizeof(kEnum_ZHT_SR_preset[0]), 0 },
    { 16,  "local_temperature",              ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 18,  "min_temperature_limit",          ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 32,  "sensor_mode",                    ::zhc::TuyaDpType::Enum,    1,
        kEnum_ZHT_SR_sensor_mode, sizeof(kEnum_ZHT_SR_sensor_mode)/sizeof(kEnum_ZHT_SR_sensor_mode[0]), 0 },
    { 34,  "max_temperature_limit",          ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 39,  "child_lock",                     ::zhc::TuyaDpType::Bool,    1,   nullptr, 0, 0 },
    { 47,  "running_state",                  ::zhc::TuyaDpType::Enum,    1,
        kEnum_ZHT_SR_running_state, sizeof(kEnum_ZHT_SR_running_state)/sizeof(kEnum_ZHT_SR_running_state[0]), 0 },
    { 48,  "backlight_brightness",           ::zhc::TuyaDpType::Numeric, 1,   nullptr, 0, 0 },
    { 50,  "current_heating_setpoint",       ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 101, "local_temperature_calibration",  ::zhc::TuyaDpType::Numeric, 256, nullptr, 0, 0 },
    { 109, "floor_temperature",              ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 110, "deadzone_temperature",           ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 111, "high_protect_temperature",       ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 112, "low_protect_temperature",        ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 113, "eco_temperature",                ::zhc::TuyaDpType::Numeric, 10,  nullptr, 0, 0 },
    { 114, "screen_time",                    ::zhc::TuyaDpType::Enum,    1,
        kEnum_ZHT_SR_screen_time, sizeof(kEnum_ZHT_SR_screen_time)/sizeof(kEnum_ZHT_SR_screen_time[0]), 0 },
    { 115, "rgb_light",                      ::zhc::TuyaDpType::Bool,    1,   nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_ZHT_SR{
    kEntries_ZHT_SR,
    sizeof(kEntries_ZHT_SR)/sizeof(kEntries_ZHT_SR[0])
};

constexpr FzConverter kFzDp_ZHT_SR{
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
    .user_config       = &kMap_ZHT_SR,
};
constexpr TzConverter kTzDp_ZHT_SR{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_ZHT_SR,
};

const FzConverter* const kFz_ZHT_SR[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZHT_SR,
};
const TzConverter* const kTz_ZHT_SR[] = { &kTzDp_ZHT_SR };

constexpr const char* kModels_ZHT_SR[] = { "TS0601" };
constexpr const char* kManus_ZHT_SR[] = { "_TZE204_lpedvtvr" };

constexpr Expose kExp_ZHT_SR[] = {
    { "current_heating_setpoint",       ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "local_temperature",              ExposeType::Numeric, ::zhc::Access::State,    "C",  nullptr, nullptr, 0 },
    { "local_temperature_calibration",  ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "system_mode",                    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "running_state",                  ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "preset",                         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "eco_temperature",                ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "child_lock",                     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "deadzone_temperature",           ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "max_temperature_limit",          ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "min_temperature_limit",          ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "sensor_mode",                    ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "floor_temperature",              ExposeType::Numeric, ::zhc::Access::State,    "C",  nullptr, nullptr, 0 },
    { "high_protect_temperature",       ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "low_protect_temperature",        ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0 },
    { "backlight_brightness",           ExposeType::Numeric, ::zhc::Access::StateSet, "%",  nullptr, nullptr, 0 },
    { "screen_time",                    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "rgb_light",                      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_ZHT_SR[] = { { 1, 0xEF00 } };

}  // namespace
extern const PreparedDefinition kDef_ZHT_SR{
    .zigbee_models=kModels_ZHT_SR, .zigbee_models_count=sizeof(kModels_ZHT_SR)/sizeof(kModels_ZHT_SR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZHT_SR, .manufacturer_names_count=sizeof(kManus_ZHT_SR)/sizeof(kManus_ZHT_SR[0]),
    .model="ZHT-SR", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_ZHT_SR, .exposes_count=sizeof(kExp_ZHT_SR)/sizeof(kExp_ZHT_SR[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZHT_SR, .from_zigbee_count=sizeof(kFz_ZHT_SR)/sizeof(kFz_ZHT_SR[0]),
    .to_zigbee=kTz_ZHT_SR, .to_zigbee_count=sizeof(kTz_ZHT_SR)/sizeof(kTz_ZHT_SR[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_ZHT_SR, .bindings_count=sizeof(kBind_ZHT_SR)/sizeof(kBind_ZHT_SR[0]),
};

}  // namespace zhc::devices::moes
