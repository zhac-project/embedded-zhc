// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_cover_with_1_switch / TS0601_cover_with_2_switch —
// curtain/blind switch with one or two switch gangs (z2m v26.115.1 parity).
//
// * `_TZE200_jhkttplm` (Homeetec 37022493) was wired here as a "T1 DP contact
//   sensor" (DP1 contact, DP2 battery) — a misidentified device: upstream has
//   always mapped it to TS0601_cover_with_1_switch, where DP1 is the cover
//   action and DP2 the position. That stub (TZE200_contact_t1.cpp) is retired.
// * `_TZE200_5nldle7w` graduated from tuya/generated/ for the z2m v26.111.0
//   rename (#13207): DP8 `motor_steering` FORWARD/BACKWARD is now
//   `motor_direction` (tubularMotorDirection {normal: 0, reversed: 1}).
// The switch gangs are datapoints on endpoint 1, so `state_l1` / `state_l2`
// are datapoint keys and need no endpoint map.
// z2m-source: tuya.ts #TS0601_cover_with_1_switch, #TS0601_cover_with_2_switch.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kAction[] = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCalib[]  = { {0,"START"}, {1,"END"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]    = { {0,"normal"}, {1,"reversed"} };

struct cfg1 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 4),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::enum_lookup(3, "calibration", kCalib, 2),
        ::zhc::tuya::dp::binary(7, "backlight_mode"),
        ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kDir, 2),
        ::zhc::tuya::dp::binary(101, "state_l1"),
        ::zhc::tuya::dp::binary(103, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
struct cfg2 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 4),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::enum_lookup(3, "calibration", kCalib, 2),
        ::zhc::tuya::dp::binary(7, "backlight_mode"),
        ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kDir, 2),
        ::zhc::tuya::dp::binary(101, "state_l2"),
        ::zhc::tuya::dp::binary(102, "state_l1"),
        ::zhc::tuya::dp::binary(103, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX1 = ::zhc::tuya::factory::TuyaRw<cfg1>;
using FX2 = ::zhc::tuya::factory::TuyaRw<cfg2>;

constexpr const char* kActionOpts[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kCalibOpts[]  = { "START", "END" };
constexpr const char* kDirOpts[]    = { "normal", "reversed" };
constexpr Expose kExp1[] = {
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kActionOpts, 3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0},
    {"state_l1",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"calibration",     ExposeType::Enum,    Access::StateSet, nullptr, "Calibration", kCalibOpts, 2},
    {"backlight_mode",  ExposeType::Binary,  Access::StateSet, nullptr, "Backlight", nullptr, 0},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, "Motor direction", kDirOpts, 2},
    {"child_lock",      ExposeType::Binary,  Access::StateSet, nullptr, "Child Lock", nullptr, 0},
};
constexpr Expose kExp2[] = {
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kActionOpts, 3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0},
    {"state_l1",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"calibration",     ExposeType::Enum,    Access::StateSet, nullptr, "Calibration", kCalibOpts, 2},
    {"backlight_mode",  ExposeType::Binary,  Access::StateSet, nullptr, "Backlight", nullptr, 0},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, "Motor direction", kDirOpts, 2},
    {"child_lock",      ExposeType::Binary,  Access::StateSet, nullptr, "Child Lock", nullptr, 0},
};
constexpr const char* kM[]  = { "TS0601" };
constexpr const char* kN1[] = { "_TZE200_jhkttplm" };
constexpr const char* kN2[] = { "_TZE200_5nldle7w" };
constexpr WhiteLabel kWL1[] = { {"Homeetec", "37022493"} };
constexpr WhiteLabel kWL2[] = { {"Homeetec", "37022173"} };
}  // namespace

extern const PreparedDefinition kDef_TS0601_cover_with_1_switch{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN1,.manufacturer_names_count=sizeof(kN1)/sizeof(kN1[0]),
    .model="TS0601_cover_with_1_switch",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp1,.exposes_count=sizeof(kExp1)/sizeof(kExp1[0]),
    .white_labels=kWL1,.white_labels_count=sizeof(kWL1)/sizeof(kWL1[0]),
    .from_zigbee=FX1::fz_list,.from_zigbee_count=FX1::fz_count,
    .to_zigbee=FX1::tz_list,.to_zigbee_count=FX1::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_TS0601_cover_with_2_switch{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN2,.manufacturer_names_count=sizeof(kN2)/sizeof(kN2[0]),
    .model="TS0601_cover_with_2_switch",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp2,.exposes_count=sizeof(kExp2)/sizeof(kExp2[0]),
    .white_labels=kWL2,.white_labels_count=sizeof(kWL2)/sizeof(kWL2[0]),
    .from_zigbee=FX2::fz_list,.from_zigbee_count=FX2::fz_count,
    .to_zigbee=FX2::tz_list,.to_zigbee_count=FX2::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
