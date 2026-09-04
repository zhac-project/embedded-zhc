// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Lincukoo R12LM-Z20T human motion & presence sensor
// (TS0601 / _TZE284_h7oalfxa; z2m v26.105.0, #13076).
//
// presence is z2m `trueFalse0` (wire 0 = present). scan_result is an upstream
// lookup over plain numbers, i.e. the device reports it on a Numeric
// datapoint; the row is typed accordingly. `scan_environment` is write-only
// (start = 0). `_TZE2841000000_h7oalfxa` resolves through the matcher's
// manufacturer-twin fallback.
// z2m-source: lincukoo.ts #R12LM-Z20T.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::lincukoo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kScan[]  = { {0,"normal"}, {1,"scanning"}, {2,"scan_success"}, {3,"scan_failure"}, {4,"scan_start"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMode[]  = { {0,"radar_mode"}, {1,"fusion_mode"} };
constexpr ::zhc::tuya::TuyaEnumEntry kStart[] = { {0,"start"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBat[]   = { {0,"low"}, {1,"middle"}, {2,"high"}, {3,"USB"} };

struct cfg_z20t {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "presence"),
        { 8, "scan_result", ::zhc::TuyaDpType::Numeric, 1, kScan, 5, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(101, "illuminance", 1),
        ::zhc::tuya::dp::enum_lookup(102, "mode", kMode, 2),
        ::zhc::tuya::dp::binary(103, "radar_switch"),
        ::zhc::tuya::dp::enum_lookup(105, "scan_environment", kStart, 1),
        ::zhc::tuya::dp::numeric(106, "set_detection_distance", 100),
        ::zhc::tuya::dp::numeric(33, "fading_time", 1),
        ::zhc::tuya::dp::enum_lookup(108, "battery_state", kBat, 4),
        ::zhc::tuya::dp::binary(109, "switch_night_light"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX_z20t = ::zhc::tuya::factory::TuyaRw<cfg_z20t>;

constexpr const char* kScanOpts[]  = { "normal", "scanning", "scan_success", "scan_failure", "scan_start" };
constexpr const char* kModeOpts[]  = { "radar_mode", "fusion_mode" };
constexpr const char* kStartOpts[] = { "start" };
constexpr const char* kBatOpts[]   = { "low", "middle", "high", "USB" };
constexpr Expose kExp_z20t[] = {
    {"presence",    ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, ::zhc::Access::State, "lx",    nullptr, nullptr, 0},
    {"scan_environment", ExposeType::Enum, ::zhc::Access::Set, nullptr, "Set no one environment", kStartOpts, 1, ExposeCategory::Config},
    {"scan_result", ExposeType::Enum,    ::zhc::Access::State, nullptr, "Environment scan result", kScanOpts, 5, ExposeCategory::Diagnostic},
    {"mode",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Device mode", kModeOpts, 2, ExposeCategory::Config},
    {"radar_switch", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "Radar switch", nullptr, 0, ExposeCategory::Config},
    {"set_detection_distance", ExposeType::Numeric, ::zhc::Access::StateSet, "m", "Detection distance (3..9, step 1.5)", nullptr, 0, ExposeCategory::Config, 3, 9, 0},
    {"fading_time", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Fading time", nullptr, 0, ExposeCategory::Config, 10, 60, 1},
    {"battery_state", ExposeType::Enum,  ::zhc::Access::State, nullptr, "Battery status", kBatOpts, 4, ExposeCategory::Diagnostic},
    {"switch_night_light", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "Night light switch", nullptr, 0, ExposeCategory::Config},
};
constexpr const char* kM_z20t[] = { "TS0601" };
constexpr const char* kN_z20t[] = { "_TZE284_h7oalfxa", "_TZE2841000000_h7oalfxa" };
}  // namespace

extern const PreparedDefinition kDef_R12LM_Z20T{
    .zigbee_models=kM_z20t,.zigbee_models_count=sizeof(kM_z20t)/sizeof(kM_z20t[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_z20t,.manufacturer_names_count=sizeof(kN_z20t)/sizeof(kN_z20t[0]),
    .model="R12LM-Z20T",.vendor="Lincukoo",.meta=nullptr,
    .exposes=kExp_z20t,.exposes_count=sizeof(kExp_z20t)/sizeof(kExp_z20t[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX_z20t::fz_list,.from_zigbee_count=FX_z20t::fz_count,
    .to_zigbee=FX_z20t::tz_list,.to_zigbee_count=FX_z20t::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::lincukoo
