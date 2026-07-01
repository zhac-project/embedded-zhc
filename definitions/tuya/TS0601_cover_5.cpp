// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 cover_5 _TZE200_p6vz3wzt (z2m v26.76.0 parity).
// v26.35->v26.76 delta: adds DP10 quick_calibration + DP14 indicator_mode.
// z2m-source: tuya.ts #TS0601_cover_5
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kState[]={{0,"open"},{1,"stop"},{2,"close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kCalib[]={{0,"START"},{1,"END"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSteer[]={{0,"FORWARD"},{1,"BACKWARD"}};
constexpr ::zhc::tuya::TuyaEnumEntry kInd[]={{0,"relay"},{1,"pos"},{2,"none"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kState,3),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::enum_lookup(3,"calibration",kCalib,2),
    ::zhc::tuya::dp::binary(7,"backlight_mode"),
    ::zhc::tuya::dp::enum_lookup(8,"motor_steering",kSteer,2),
    ::zhc::tuya::dp::numeric(10,"quick_calibration",1),
    ::zhc::tuya::dp::enum_lookup(14,"indicator_mode",kInd,3),
    ::zhc::tuya::dp::binary(103,"child_lock")};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,8}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_p6vz3wzt"};
constexpr const char* kStateOpts[]={"open","stop","close"};
constexpr const char* kCalibOpts[]={"START","END"};
constexpr const char* kSteerOpts[]={"FORWARD","BACKWARD"};
constexpr const char* kIndOpts[]={"relay","pos","none"};
constexpr Expose kExp[]={
    {"state",             ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},
    {"position",          ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,     0},
    {"calibration",       ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kCalibOpts,  2},
    {"backlight_mode",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,     0},
    {"motor_steering",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSteerOpts,  2},
    {"quick_calibration", ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr,     0},
    {"indicator_mode",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kIndOpts,    3},
    {"child_lock",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,     0}};
constexpr WhiteLabel kWL[]={
    {"Homeetec","37022483"},
    {"BSEED","_TZE284_uqfph8ah"}};
}
extern const PreparedDefinition kDef_TS0601_cover_5{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_cover_5",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
