// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes ZS-SF-EUC-WH-MS "Star feather" Zigbee curtain switch —
// GRADUATED from generated/ and rewritten for z2m v26.101.0.
//
// TWO changes in one, and the first is a pre-existing bug the new window
// exposed:
//
//   1. This is a Tuya DATAPOINT device — z2m gives it
//      `tuya.modernExtend.tuyaBase({dp: true})` and a `meta.tuyaDatapoints`
//      table on cluster 0xEF00. The auto-generated definition instead wired
//      the GENERIC ZCL cover converters against closuresWindowCovering
//      (0x0102) and bound that cluster. The device never speaks 0x0102, so
//      `position` never decoded and a position write went nowhere. Rewired
//      onto the Tuya DP factory.
//
//   2. z2m v26.101.0 added datapoints 3, 7, 8 and 10 (calibration,
//      backlight_switch, motor_direction, motor_working_time) plus the second
//      manufacturer `_TZE28C1000000_i8sdouy0`.
//
// `motor_direction` uses the normal/reversed labels z2m standardised on in
// v26.95.0 (`tubularMotorDirection`), not the older forward/back pair.
//
// DP1 `state` follows z2m's `coverAction` lookup, which carries a fourth
// CONTINUE value alongside OPEN/STOP/CLOSE.
//
// z2m-source: moes.ts #ZS-SF-EUC-WH-MS.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kCoverAction[] = {
    {0, "OPEN"}, {1, "STOP"}, {2, "CLOSE"}, {3, "CONTINUE"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kCalibration[] = { {0, "start"}, {1, "end"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotorDirection[] = { {0, "normal"}, {1, "reversed"} };

struct cfg_zs_sf {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kCoverAction, 4),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::enum_lookup(3, "calibration", kCalibration, 2),
        ::zhc::tuya::dp::binary(7, "backlight_switch"),
        ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kMotorDirection, 2),
        ::zhc::tuya::dp::numeric(10, "motor_working_time", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXZsSf = ::zhc::tuya::factory::TuyaRw<cfg_zs_sf>;

constexpr const char* kStateOpts[]      = { "OPEN", "STOP", "CLOSE", "CONTINUE" };
constexpr const char* kCalibrationOpts[] = { "start", "end" };
constexpr const char* kDirectionOpts[]  = { "normal", "reversed" };

constexpr Expose kExposes_ZS_SF[] = {
    { "state",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover command",
      kStateOpts, 4 },
    { "position",           ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover position",
      nullptr, 0, ExposeCategory::State, 0, 100, 1 },
    { "calibration",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Calibration mode",
      kCalibrationOpts, 2, ExposeCategory::Config },
    { "backlight_switch",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,
      "Enable or disable button backlight", nullptr, 0, ExposeCategory::Config },
    { "motor_direction",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr,
      "Direction of motor movement", kDirectionOpts, 2, ExposeCategory::Config },
    { "motor_working_time", ExposeType::Numeric, ::zhc::Access::StateSet, "s",
      "Full travel time of the motor (10-180s)", nullptr, 0, ExposeCategory::Config, 10, 180, 1 },
};

constexpr const char* kModels_ZS_SF[] = { "TS0601" };
constexpr const char* kManus_ZS_SF[]  = { "_TZE284_upt8lzi0", "_TZE28C1000000_i8sdouy0" };

}  // namespace

extern const PreparedDefinition kDef_ZS_SF_EUC_WH_MS{
    .zigbee_models=kModels_ZS_SF, .zigbee_models_count=sizeof(kModels_ZS_SF)/sizeof(kModels_ZS_SF[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZS_SF, .manufacturer_names_count=sizeof(kManus_ZS_SF)/sizeof(kManus_ZS_SF[0]),
    .model="ZS-SF-EUC-WH-MS", .vendor="Moes",
    .meta=nullptr,
    .exposes=kExposes_ZS_SF, .exposes_count=sizeof(kExposes_ZS_SF)/sizeof(kExposes_ZS_SF[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXZsSf::fz_list, .from_zigbee_count=FXZsSf::fz_count,
    .to_zigbee=FXZsSf::tz_list, .to_zigbee_count=FXZsSf::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::moes
