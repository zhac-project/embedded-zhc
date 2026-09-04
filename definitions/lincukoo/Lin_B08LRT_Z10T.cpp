// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Lincukoo B08LRT-Z10T 5-in-1 sensor button switch
// (TS0601 / _TZE284_cipobaav; z2m v26.105.0, #13076).
//
// dp104 is mapped twice upstream: once as `action` (single_click /
// double_click / long_press) and once as the `scene_switch` enum; both rows
// fire on decode. presence is `trueFalse0`; scan_result is a plain-number
// lookup on a Numeric datapoint.
// z2m-source: lincukoo.ts #B08LRT-Z10T.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::lincukoo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kClick[] = { {0,"single_click"}, {1,"double_click"}, {2,"long_press"} };
constexpr ::zhc::tuya::TuyaEnumEntry kScan[]  = { {0,"normal"}, {1,"scanning"}, {2,"scan_success"}, {3,"scan_failure"}, {4,"scan_start"} };
constexpr ::zhc::tuya::TuyaEnumEntry kStart[] = { {0,"start"} };
constexpr ::zhc::tuya::TuyaEnumEntry kUnit[]  = { {0,"c"}, {1,"f"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBat[]   = { {0,"low"}, {1,"middle"}, {2,"high"} };

struct cfg_b08 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "presence"),
        ::zhc::tuya::dp::numeric(101, "illuminance", 1),
        ::zhc::tuya::dp::temperature(22),
        ::zhc::tuya::dp::numeric(23, "humidity", 1),
        ::zhc::tuya::dp::action(104, kClick, 3),
        ::zhc::tuya::dp::enum_lookup(104, "scene_switch", kClick, 3),
        ::zhc::tuya::dp::binary(103, "radar_switch"),
        ::zhc::tuya::dp::enum_lookup(105, "scan_environment", kStart, 1),
        { 8, "scan_result", ::zhc::TuyaDpType::Numeric, 1, kScan, 5, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(106, "set_detection_distance", 100),
        ::zhc::tuya::dp::numeric(33, "hold_time", 1),
        ::zhc::tuya::dp::enum_lookup(26, "temp_unit", kUnit, 2),
        ::zhc::tuya::dp::enum_lookup(108, "battery_state", kBat, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX_b08 = ::zhc::tuya::factory::TuyaRw<cfg_b08>;

constexpr const char* kClickOpts[] = { "single_click", "double_click", "long_press" };
constexpr const char* kScanOpts[]  = { "normal", "scanning", "scan_success", "scan_failure", "scan_start" };
constexpr const char* kStartOpts[] = { "start" };
constexpr const char* kUnitOpts[]  = { "c", "f" };
constexpr const char* kBatOpts[]   = { "low", "middle", "high" };
constexpr Expose kExp_b08[] = {
    {"presence",    ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, ::zhc::Access::State, "lx", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, ::zhc::Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0},
    {"action",      ExposeType::Enum,    ::zhc::Access::State, nullptr, "Scene switch action", kClickOpts, 3},
    {"scene_switch", ExposeType::Enum,   ::zhc::Access::StateSet, nullptr, "Scene switch click", kClickOpts, 3},
    {"radar_switch", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "Radar switch", nullptr, 0, ExposeCategory::Config},
    {"scan_environment", ExposeType::Enum, ::zhc::Access::Set, nullptr, "Set no one environment", kStartOpts, 1, ExposeCategory::Config},
    {"scan_result", ExposeType::Enum,    ::zhc::Access::State, nullptr, "Environment scan result", kScanOpts, 5, ExposeCategory::Diagnostic},
    {"set_detection_distance", ExposeType::Numeric, ::zhc::Access::StateSet, "m", "Detection distance (3..6, step 1.5)", nullptr, 0, ExposeCategory::Config, 3, 6, 0},
    {"hold_time",   ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Hold time", nullptr, 0, ExposeCategory::Config, 10, 60, 5},
    {"temp_unit",   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Temperature unit", kUnitOpts, 2, ExposeCategory::Config},
    {"battery_state", ExposeType::Enum,  ::zhc::Access::State, nullptr, "Battery state", kBatOpts, 3, ExposeCategory::Diagnostic},
};
constexpr const char* kM_b08[] = { "TS0601" };
constexpr const char* kN_b08[] = { "_TZE284_cipobaav", "_TZE2841000000_cipobaav" };
}  // namespace

extern const PreparedDefinition kDef_B08LRT_Z10T{
    .zigbee_models=kM_b08,.zigbee_models_count=sizeof(kM_b08)/sizeof(kM_b08[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_b08,.manufacturer_names_count=sizeof(kN_b08)/sizeof(kN_b08[0]),
    .model="B08LRT-Z10T",.vendor="Lincukoo",.meta=nullptr,
    .exposes=kExp_b08,.exposes_count=sizeof(kExp_b08)/sizeof(kExp_b08[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX_b08::fz_list,.from_zigbee_count=FX_b08::fz_count,
    .to_zigbee=FX_b08::tz_list,.to_zigbee_count=FX_b08::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::lincukoo
