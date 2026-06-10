// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Javis JS-MC-SENSOR-ZB — _TZE200_kagkgk0i DP-remap variant.
// Microwave sensor.
//
// Parity fix split-out: z2m's `legacy.fz.javis_microwave_sensor` remaps three
// DPs for `_TZE200_kagkgk0i` only:
//   DP102 -> illuminance_calibration  (other variants: keep_time)
//   DP106 -> keep_time                (other variants: unused)
//   DP107 -> led_enable               (other variants: unused)
// DP103 (led_enable) and DP105 (illuminance_calibration) decode for every
// variant. Because the embedded DP map is per-definition, this manufacturer
// gets its own def; find_definition Pass-1 routes _TZE200_kagkgk0i here while
// the sibling Jav_JS_MC_SENSOR_ZB.cpp handles _TZE200_lgstepha/_i0b1dbqu.
//
// z2m-source: javis.ts #JS-MC-SENSOR-ZB; lib/legacy.ts fz/tz javis_microwave_sensor.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::javis {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPresenceK[] = {
    {0, "no_motion"},
    {1, "big_motion"},
    {2, "minor_motion"},
    {3, "breathing"},
    {4, "abnormal_state"},
    {5, "initializing"},
    {6, "initialization_completed"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kOccupiedK[] = {
    {1, "x"}, {2, "x"}, {3, "x"}, {4, "x"},
};

struct cfgK {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "states", kPresenceK,
                                     sizeof(kPresenceK) / sizeof(kPresenceK[0])),
        {1, "occupancy", ::zhc::TuyaDpType::Enum, 1, kOccupiedK,
         sizeof(kOccupiedK) / sizeof(kOccupiedK[0]), ::zhc::tuya::kTuyaDpFlagEnumBool},
        ::zhc::tuya::dp::numeric(2, "sensitivity"),
        ::zhc::tuya::dp::numeric(101, "illuminance"),
        ::zhc::tuya::dp::numeric(102, "illuminance_calibration"),
        ::zhc::tuya::dp::binary(103, "led_enable"),
        ::zhc::tuya::dp::numeric(104, "illuminance"),
        ::zhc::tuya::dp::numeric(105, "illuminance_calibration"),
        ::zhc::tuya::dp::numeric(106, "keep_time"),
        ::zhc::tuya::dp::binary(107, "led_enable"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{
        e, sizeof(e) / sizeof(e[0])};
};
using FXK = ::zhc::tuya::factory::TuyaRw<cfgK>;

constexpr const char* kModels_JS_MC_SENSOR_ZB_K[] = { "TS0601" };
constexpr const char* kManus_JS_MC_SENSOR_ZB_K[] = { "_TZE200_kagkgk0i" };
}  // namespace

constexpr Expose kAutoExposesK[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"states", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"led_enable", ExposeType::Binary, Access::StateSet, nullptr, "Enabled LED", nullptr, 0},
    {"keep_time", ExposeType::Enum, Access::StateSet, nullptr, "PIR keep time", nullptr, 0},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"illuminance_calibration", ExposeType::Numeric, Access::StateSet, nullptr, "Illuminance calibration", nullptr, 0},
};

extern const PreparedDefinition kDef_JS_MC_SENSOR_ZB_kagkgk0i{
    .zigbee_models=kModels_JS_MC_SENSOR_ZB_K, .zigbee_models_count=sizeof(kModels_JS_MC_SENSOR_ZB_K)/sizeof(kModels_JS_MC_SENSOR_ZB_K[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_JS_MC_SENSOR_ZB_K, .manufacturer_names_count=sizeof(kManus_JS_MC_SENSOR_ZB_K)/sizeof(kManus_JS_MC_SENSOR_ZB_K[0]),
    .model="JS-MC-SENSOR-ZB", .vendor="Javis",
    .meta=nullptr, .exposes=kAutoExposesK, .exposes_count=sizeof(kAutoExposesK)/sizeof(kAutoExposesK[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXK::fz_list, .from_zigbee_count=FXK::fz_count,
    .to_zigbee=FXK::tz_list, .to_zigbee_count=FXK::tz_count,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::javis
