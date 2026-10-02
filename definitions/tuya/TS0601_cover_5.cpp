// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_cover_5 curtain/blind switch (z2m v26.115.1 parity).
//
// z2m v26.111.0 (#13207) renamed DP8 `motor_steering` FORWARD/BACKWARD to
// `motor_direction` via valueConverter.tubularMotorDirection {normal: 0,
// reversed: 1}; v26.113.0 added `_TZE284_pxwixtky`.
//
// Upstream branches on the manufacturer name at runtime, so this file holds
// three definitions:
//   * `_TZE284_waa352qv` reports DP1 in another order {STOP: 0, CLOSE: 1, OPEN: 2};
//   * `_TZE284_uqfph8ah` (BSEED) exposes quick_calibration + indicator_mode
//     instead of child_lock;
//   * the rest share the plain table.
// The three per-manufacturer copies under generated/ (b7kbnl6q, uqfph8ah,
// waa352qv) are retired: they still carried motor_steering, and the b7kbnl6q
// one decoded nothing (genOnOff stub).
// z2m-source: tuya.ts #TS0601_cover_5.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kState[]    = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kStateRev[] = { {0,"STOP"}, {1,"CLOSE"}, {2,"OPEN"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCalib[]    = { {0,"START"}, {1,"END"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]      = { {0,"normal"}, {1,"reversed"} };
constexpr ::zhc::tuya::TuyaEnumEntry kInd[]      = { {0,"relay"}, {1,"pos"}, {2,"none"} };

#define ZHC_COVER5_ROWS(STATE)                                              \
    ::zhc::tuya::dp::enum_lookup(1, "state", STATE, 3),                     \
    ::zhc::tuya::dp::numeric(2, "position", 1),                             \
    ::zhc::tuya::dp::enum_lookup(3, "calibration", kCalib, 2),              \
    ::zhc::tuya::dp::binary(7, "backlight_mode"),                           \
    ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kDir, 2),            \
    ::zhc::tuya::dp::numeric(10, "quick_calibration", 1),                   \
    ::zhc::tuya::dp::enum_lookup(14, "indicator_mode", kInd, 3),            \
    ::zhc::tuya::dp::binary(103, "child_lock")

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = { ZHC_COVER5_ROWS(kState) };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
struct cfg_rev {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = { ZHC_COVER5_ROWS(kStateRev) };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
#undef ZHC_COVER5_ROWS
using FX    = ::zhc::tuya::factory::TuyaRw<cfg>;
using FXRev = ::zhc::tuya::factory::TuyaRw<cfg_rev>;

constexpr const char* kStateOpts[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kCalibOpts[] = { "START", "END" };
constexpr const char* kDirOpts[]   = { "normal", "reversed" };
constexpr const char* kIndOpts[]   = { "relay", "pos", "none" };

#define ZHC_COVER5_COMMON_EXPOSES                                                                    \
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},     \
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},     \
    {"calibration",     ExposeType::Enum,    Access::StateSet, nullptr, "Calibration", kCalibOpts, 2}, \
    {"backlight_mode",  ExposeType::Binary,  Access::StateSet, nullptr, "Backlight", nullptr, 0},    \
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, "Motor direction", kDirOpts, 2}

constexpr Expose kExp[] = {
    ZHC_COVER5_COMMON_EXPOSES,
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Child Lock", nullptr, 0},
};
constexpr Expose kExpBseed[] = {
    ZHC_COVER5_COMMON_EXPOSES,
    {"quick_calibration", ExposeType::Numeric, Access::StateSet, "s", "Set quick calibration", nullptr, 0,
     ExposeCategory::State, 1, 120, 0},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr, "LED indicator mode", kIndOpts, 3},
};
#undef ZHC_COVER5_COMMON_EXPOSES

constexpr const char* kM[]      = { "TS0601" };
constexpr const char* kN[]      = { "_TZE200_p6vz3wzt", "_TZE204_p6vz3wzt", "_TZE284_b7kbnl6q", "_TZE284_pxwixtky" };
constexpr const char* kNBseed[] = { "_TZE284_uqfph8ah" };
constexpr const char* kNRev[]   = { "_TZE284_waa352qv" };
constexpr WhiteLabel kWL[]      = { {"Homeetec", "37022483"}, {"Moes", "ZRS-USC-WH"} };
constexpr WhiteLabel kWLBseed[] = { {"BSEED", "_TZE284_uqfph8ah"} };
}  // namespace

extern const PreparedDefinition kDef_TS0601_cover_5{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="TS0601_cover_5",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_TS0601_cover_5_uqfph8ah{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kNBseed,.manufacturer_names_count=sizeof(kNBseed)/sizeof(kNBseed[0]),.model="TS0601_cover_5",
    .vendor="Tuya",.meta=nullptr,.exposes=kExpBseed,.exposes_count=sizeof(kExpBseed)/sizeof(kExpBseed[0]),
    .white_labels=kWLBseed,.white_labels_count=sizeof(kWLBseed)/sizeof(kWLBseed[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_TS0601_cover_5_waa352qv{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kNRev,.manufacturer_names_count=sizeof(kNRev)/sizeof(kNRev[0]),.model="TS0601_cover_5",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FXRev::fz_list,.from_zigbee_count=FXRev::fz_count,
    .to_zigbee=FXRev::tz_list,.to_zigbee_count=FXRev::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
