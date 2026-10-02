// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_cover_12 curtain motor, `_TZE200_mlglxwp3` (the only
// variant upstream gives a battery, DP103). z2m v26.115.1 parity.
// GRADUATED from definitions/tuya/generated/Gen__TZE200_mlglxwp3.cpp (v26.95.0).
//
// v26.115.1 fixes: model name TS0601_cover_12 (was "TS0601__TZE200_mlglxwp3");
// DP7 is z2m's `motor_state` {opening, closing, stopped} (was `work_state`
// without `stopped`); DP1 coverAction gains CONTINUE; exposes listed. The
// tgl8i2np / a0hirjnh variants live in TS0601_cover_13.cpp.
// z2m-source: tuya.ts #TS0601_cover_12.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"OPEN"},{1,"STOP"},{2,"CLOSE"},{3,"CONTINUE"}};
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]={{0,"normal"},{1,"reversed"}};
constexpr ::zhc::tuya::TuyaEnumEntry kMotor[]={{0,"opening"},{1,"closing"},{2,"stopped"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSit[]={{0,"fully_close"},{1,"fully_open"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,4),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::numeric(3,"position",1),
    ::zhc::tuya::dp::enum_lookup(5,"motor_direction",kDir,2),
    ::zhc::tuya::dp::enum_lookup(7,"motor_state",kMotor,3),
    ::zhc::tuya::dp::numeric(10,"total_time",1),
    ::zhc::tuya::dp::enum_lookup(11,"situation_set",kSit,2),
    ::zhc::tuya::dp::numeric(12,"fault",1),
    ::zhc::tuya::dp::numeric(103,"battery",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_mlglxwp3"};
constexpr const char* kStOpts[]={"OPEN","CLOSE","STOP"};
constexpr const char* kDirOpts[]={"normal","reversed"};
constexpr const char* kMotorOpts[]={"opening","closing","stopped"};
constexpr const char* kSitOpts[]={"fully_close","fully_open"};
constexpr Expose kExp[]={
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,    3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kDirOpts,   2},
    {"motor_state",     ExposeType::Enum,    Access::State,    nullptr, nullptr, kMotorOpts, 3},
    {"total_time",      ExposeType::Numeric, Access::State,    "ms",    "Total running time in milliseconds", nullptr, 0},
    {"situation_set",   ExposeType::Enum,    Access::StateSet, nullptr, "Set fully open or fully close position", kSitOpts, 2},
    {"fault",           ExposeType::Numeric, Access::State,    nullptr, "Fault details", nullptr, 0},
    {"battery",         ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDef_TZE200_mlglxwp3{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="TS0601_cover_12",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
