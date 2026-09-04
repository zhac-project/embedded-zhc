// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Lincukoo R12LM-Z10T human motion & presence sensor
// (TS0601 / _TZE284_hqys6frs).
//
// Graduated 2026-09-04 while porting its sibling R12LM-Z20T: the generated
// copy decoded this TS0601 datapoint device as a genOnOff switch with a
// battery, so a real unit published nothing. The per-manufacturer copy
// `Lin__TZE284_hqys6frs` was retired with it.
// z2m-source: lincukoo.ts #R12LM-Z10T.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::lincukoo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kMode[] = { {0,"radar_mode"}, {1,"combine_mode"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBat[]  = { {0,"low"}, {1,"middle"}, {2,"high"}, {3,"usb"} };

struct cfg_z10t {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "presence"),
        ::zhc::tuya::dp::numeric(101, "illuminance", 1),
        ::zhc::tuya::dp::enum_lookup(102, "work_mode", kMode, 2),
        ::zhc::tuya::dp::binary(103, "radar_switch"),
        ::zhc::tuya::dp::numeric(104, "fading_time", 1),
        ::zhc::tuya::dp::numeric(106, "detection_distance", 100),
        ::zhc::tuya::dp::numeric(107, "radar_sensitivity", 1),
        ::zhc::tuya::dp::enum_lookup(108, "battery_state", kBat, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX_z10t = ::zhc::tuya::factory::TuyaRw<cfg_z10t>;

constexpr const char* kModeOpts[] = { "radar_mode", "combine_mode" };
constexpr const char* kBatOpts[]  = { "low", "middle", "high", "usb" };
constexpr Expose kExp_z10t[] = {
    {"presence",    ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, ::zhc::Access::State, "lx", nullptr, nullptr, 0},
    {"work_mode",   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Work mode of device", kModeOpts, 2, ExposeCategory::Config},
    {"radar_switch", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "Radar switch", nullptr, 0, ExposeCategory::Config},
    {"fading_time", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Fading time", nullptr, 0, ExposeCategory::Config, 5, 60, 5},
    {"detection_distance", ExposeType::Numeric, ::zhc::Access::StateSet, "m", "Maximum range (3..9, step 1.5)", nullptr, 0, ExposeCategory::Config, 3, 9, 0},
    {"radar_sensitivity", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Sensitivity of the radar", nullptr, 0, ExposeCategory::Config, 0, 4, 1},
    {"battery_state", ExposeType::Enum,  ::zhc::Access::State, nullptr, "Battery state of the sensor", kBatOpts, 4, ExposeCategory::Diagnostic},
};
constexpr const char* kM_z10t[] = { "TS0601" };
constexpr const char* kN_z10t[] = { "_TZE284_hqys6frs" };
}  // namespace

extern const PreparedDefinition kDef_R12LM_Z10T{
    .zigbee_models=kM_z10t,.zigbee_models_count=sizeof(kM_z10t)/sizeof(kM_z10t[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_z10t,.manufacturer_names_count=sizeof(kN_z10t)/sizeof(kN_z10t[0]),
    .model="R12LM-Z10T",.vendor="Lincukoo",.meta=nullptr,
    .exposes=kExp_z10t,.exposes_count=sizeof(kExp_z10t)/sizeof(kExp_z10t[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX_z10t::fz_list,.from_zigbee_count=FX_z10t::fz_count,
    .to_zigbee=FX_z10t::tz_list,.to_zigbee_count=FX_z10t::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::lincukoo
