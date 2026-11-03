// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_cover_15 curtain motor, _TZE200/_TZE204/_TZE284_bjzrowv2
// (z2m v26.125.0 window; upstream moved the three names out of
// TS0601_cover_1). Before this, _TZE200_bjzrowv2 matched a "water boiler
// thermostat" def that no upstream version ever had on this name, and the
// _TZE204_/_TZE284_ twins were generated stubs with no DP map; all three are
// retired.
// DPs as TS0601_cover_12/13: 1 state coverAction, 2 and 3 position (both
// decode, writes hit DP2; coverPosition, not inverted), 5 motor_direction, 6
// auto_power, 7 motor_state, 10 total_time, 12 fault. No DP11 situation_set.
// z2m-source: tuya.ts #TS0601_cover_15.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"OPEN"},{1,"STOP"},{2,"CLOSE"},{3,"CONTINUE"}};
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]={{0,"normal"},{1,"reversed"}};
constexpr ::zhc::tuya::TuyaEnumEntry kMotor[]={{0,"opening"},{1,"closing"},{2,"stopped"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,4),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::numeric(3,"position",1),
    ::zhc::tuya::dp::enum_lookup(5,"motor_direction",kDir,2),
    ::zhc::tuya::dp::binary(6,"auto_power"),
    ::zhc::tuya::dp::enum_lookup(7,"motor_state",kMotor,3),
    ::zhc::tuya::dp::numeric(10,"total_time",1),
    ::zhc::tuya::dp::numeric(12,"fault",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_bjzrowv2","_TZE204_bjzrowv2","_TZE284_bjzrowv2"};
constexpr const char* kStOpts[]={"OPEN","CLOSE","STOP"};
constexpr const char* kDirOpts[]={"normal","reversed"};
constexpr const char* kMotorOpts[]={"opening","closing","stopped"};
constexpr Expose kExp[]={
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,    3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kDirOpts,   2},
    {"motor_state",     ExposeType::Enum,    Access::State,    nullptr, nullptr, kMotorOpts, 3},
    {"total_time",      ExposeType::Numeric, Access::State,    "ms",    "Total running time in milliseconds", nullptr, 0},
    {"fault",           ExposeType::Numeric, Access::State,    nullptr, "Fault details", nullptr, 0},
    {"auto_power",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDef_TS0601_cover_15{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="TS0601_cover_15",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
