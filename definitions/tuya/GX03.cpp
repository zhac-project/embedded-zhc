// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: GIEX GX03 2-zone watering timer _TZE284_8zizsafo / _TZE284_iilebqoo
// (z2m v26.77.0 parity). z2m-source: tuya.ts #GX03. Graduated from two
// generated stubs that mis-decoded DP104/DP105 as the valve on/off: z2m moved
// valve control to DP1/DP2 (onOff) and repurposed DP104/DP105 as the valve
// STATE enum (GX03ValveState {0:Manual,1:Auto,2:Closed}). timer_1/timer_2
// (SET, dual-mapped on DP13/DP14) deferred per the SET-only-config convention.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kValveState[]={{0,"Manual"},{1,"Auto"},{2,"Closed"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"valve_1"),
    ::zhc::tuya::dp::binary(2,"valve_2"),
    ::zhc::tuya::dp::enum_lookup(104,"state_1",kValveState,3),
    ::zhc::tuya::dp::enum_lookup(105,"state_2",kValveState,3),
    ::zhc::tuya::dp::numeric(13,"countdown_1",1),   // z2m countdown = raw (min)
    ::zhc::tuya::dp::numeric(14,"countdown_2",1),
    ::zhc::tuya::dp::numeric(25,"last_duration_1",1),
    ::zhc::tuya::dp::numeric(26,"last_duration_2",1),
    ::zhc::tuya::dp::numeric(59,"battery",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,9}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;  // valves (DP1/DP2) writable
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE284_8zizsafo","_TZE284_iilebqoo"};
constexpr const char* kValveStateOpts[]={"Manual","Auto","Closed"};
constexpr WhiteLabel kWL[]={{"Nova Digital","ZVL-DUAL"}};
constexpr Expose kExp[]={
    {"valve_1",         ExposeType::Binary,  Access::StateSet, nullptr, "State of the valve 1",                nullptr, 0},
    {"state_1",         ExposeType::Enum,    Access::State,    nullptr, "State of the valve 1",                kValveStateOpts, 3},
    {"countdown_1",     ExposeType::Numeric, Access::State,    "min",   "Time remaining for the open valve 1", nullptr, 0},
    {"last_duration_1", ExposeType::Numeric, Access::State,    "s",     "Last open duration for the valve 1",  nullptr, 0},
    {"valve_2",         ExposeType::Binary,  Access::StateSet, nullptr, "State of the valve 2",                nullptr, 0},
    {"state_2",         ExposeType::Enum,    Access::State,    nullptr, "State of the valve 2",                kValveStateOpts, 3},
    {"countdown_2",     ExposeType::Numeric, Access::State,    "min",   "Time remaining for the open valve 2", nullptr, 0},
    {"last_duration_2", ExposeType::Numeric, Access::State,    "s",     "Last open duration for the valve 2",  nullptr, 0},
    {"battery",         ExposeType::Numeric, Access::State,    "%",     "Battery",                             nullptr, 0}};
}
extern const PreparedDefinition kDef_GX03{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="GX03",
    .vendor="GIEX",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
