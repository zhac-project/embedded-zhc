// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS130F_xbexmf8h blind/curtain motor controller, TS130F
// _TZE20C_xbexmf8h (z2m v26.125.0 window renamed motor_reversal to
// motor_direction). A datapoint device: before this def it fell to the
// model-only ZCL TS130F def and decoded nothing.
// DPs: 1 state coverAction, 2 position (not inverted), 3 calibration and 14
// backlight_mode onOffEnumOn0 (ON = 0), 8 motor_direction, 10
// calibration_time /10, 12 switch_type (upstream: "needs confirmation").
// z2m-source: tuya.ts #TS130F_xbexmf8h.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"OPEN"},{1,"STOP"},{2,"CLOSE"},{3,"CONTINUE"}};
constexpr ::zhc::tuya::TuyaEnumEntry kOnOffOn0[]={{0,"ON"},{1,"OFF"}};
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]={{0,"normal"},{1,"reversed"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSwitchType[]={{0,"momentary"},{1,"toggle"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,4),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::enum_lookup(3,"calibration",kOnOffOn0,2),
    ::zhc::tuya::dp::enum_lookup(8,"motor_direction",kDir,2),
    ::zhc::tuya::dp::numeric(10,"calibration_time",10),
    ::zhc::tuya::dp::enum_lookup(12,"switch_type",kSwitchType,2),
    ::zhc::tuya::dp::enum_lookup(14,"backlight_mode",kOnOffOn0,2)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS130F"};
constexpr const char* kN[]={"_TZE20C_xbexmf8h"};
constexpr const char* kStOpts[]={"OPEN","CLOSE","STOP"};
constexpr const char* kDirOpts[]={"normal","reversed"};
constexpr const char* kSwitchOpts[]={"momentary","toggle"};
constexpr Expose kExp[]={
    {"state",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts, 3},
    {"position",         ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"calibration_time", ExposeType::Numeric, Access::State,    "s",     "Calibration time measured by motor", nullptr, 0, ExposeCategory::Diagnostic},
    {"motor_direction",  ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kDirOpts, 2},
    {"calibration",      ExposeType::Binary,  Access::StateSet, nullptr, "Calibration mode", nullptr, 0, ExposeCategory::Config},
    {"switch_type",      ExposeType::Enum,    Access::StateSet, nullptr, "Type of the installed switch", kSwitchOpts, 2, ExposeCategory::Config},
    {"backlight_mode",   ExposeType::Binary,  Access::StateSet, nullptr, "Switch backlight", nullptr, 0, ExposeCategory::Config}};
}
extern const PreparedDefinition kDef_TS130F_xbexmf8h{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="TS130F_xbexmf8h",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
