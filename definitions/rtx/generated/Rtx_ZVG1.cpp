// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RTX ZVG1 Tuya smart water valve.
// Decodes the Tuya DP frames z2m's `legacy.fz.ZVG1` parses on
// manuSpecificTuya cluster: state(1), water_consumed(5), battery(7),
// weather_delay(10), timer_time_left(11), timer_state(12),
// last_valve_open_duration(15). DP 16/17 (cycle/normal_schedule
// timer text payloads) are not decoded — they are 40-byte packed
// blobs that need a bespoke string formatter (see PARITY doc).
// z2m-source: rtx.ts #ZVG1, lib/legacy.ts ZVG1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::rtx {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kWeatherDelayLut[] = {
    {0, "disabled"}, {1, "24h"}, {2, "48h"}, {3, "72h"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kTimerStateLut[] = {
    {0, "disabled"}, {1, "active"}, {2, "enabled"},
};

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry entries[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::numeric(7, "battery"),
        ::zhc::tuya::dp::enum_lookup(10, "weather_delay",
                                     kWeatherDelayLut, 4),
        // DP 11 reports timer_time_left in seconds; emit as Float
        // minutes via /60 divisor so shadow gets the same magnitude
        // z2m publishes.
        ::zhc::tuya::dp::numeric(11, "timer_time_left", 60),
        ::zhc::tuya::dp::enum_lookup(12, "timer_state",
                                     kTimerStateLut, 3),
        // DP 15: last_valve_open_duration also in seconds → minutes.
        ::zhc::tuya::dp::numeric(15, "last_valve_open_duration", 60),
        // DP 5: water_consumed reported in fl-oz, z2m converts to L
        // via /33.8140226. Uses the float-divisor field landed
        // 2026-04-29c in `_shared.{hpp,cpp}` (TuyaDpMapEntry).
        ::zhc::tuya::TuyaDpMapEntry{
            5, "water_consumed", ::zhc::TuyaDpType::Numeric,
            0,        // int divisor unused
            nullptr, 0, 0,
            33.8140226f,
        },
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ entries, 7 };
};

using FX = ::zhc::tuya::factory::TuyaOnOff<cfg>;

// Custom Tz wiring — shape copied from `kTzTuyaDatapointsProto` with
// `user_config` pointed at the same DP map so writes for any mapped
// key (state, weather_delay, timer_state, …) encode as Tuya setData.
extern const TzConverter kTz_ZVG1_DpWrite{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &cfg::dp_map,
};

const TzConverter* const kTz_ZVG1[] = {
    &kTz_ZVG1_DpWrite,
};

constexpr const char* kModels_ZVG1[] = { "TS0601" };
constexpr const char* kManus_ZVG1[] = {
    "_TZE200_akjefhj5", "_TZE200_2wg5qrjy",
    "_TZE200_81isopgh", "_TZE284_xuflgcnz",
};
}  // namespace


// --- hand-authored exposes (z2m presets list) ---
constexpr Expose kAutoExposes[] = {
    {"state",                     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery",                   ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
    {"weather_delay",             ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"timer_state",               ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"timer_time_left",           ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
    {"last_valve_open_duration",  ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
    {"water_consumed",            ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_ZVG1{
    .zigbee_models=kModels_ZVG1, .zigbee_models_count=sizeof(kModels_ZVG1)/sizeof(kModels_ZVG1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZVG1, .manufacturer_names_count=sizeof(kManus_ZVG1)/sizeof(kManus_ZVG1[0]),
    .model="ZVG1", .vendor="RTX",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=kTz_ZVG1, .to_zigbee_count=sizeof(kTz_ZVG1)/sizeof(kTz_ZVG1[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::rtx
