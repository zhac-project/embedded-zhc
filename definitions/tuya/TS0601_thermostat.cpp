// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 thermostats / TRVs (legacy tuya_thermostat — shared DP
// layout, no per-mfg branching, so one def covers the whole family).
//   DP2  current_heating_setpoint /10   DP3  local_temperature /10
//   DP7  child_lock (bool)              DP20 valve_detection (bool)
//   DP21 battery                        DP44 local_temperature_calibration /10
//   DP102 min_temperature              DP103 max_temperature
//   DP105 boost_time                   DP107 comfort_temperature
//   DP108 eco_temperature              DP110 battery_low (bool)
//   DP115 window_open (bool)
// READ for all; WRITE (factory::TuyaRw) for setpoint + child_lock (exposed
// StateSet). running_state, mode, schedules, window_detection composite remain
// deferred. WARNING: setpoint write controls heating — encode host-tested,
// on-device behaviour UNVERIFIED (no hardware).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::numeric(2,"current_heating_setpoint",10),
    ::zhc::tuya::dp::numeric(3,"local_temperature",10),
    ::zhc::tuya::dp::binary(7,"child_lock"),
    ::zhc::tuya::dp::binary(20,"valve_detection"),
    ::zhc::tuya::dp::numeric(21,"battery",1),
    ::zhc::tuya::dp::numeric(44,"local_temperature_calibration",10),
    ::zhc::tuya::dp::numeric(102,"min_temperature",1),
    ::zhc::tuya::dp::numeric(103,"max_temperature",1),
    ::zhc::tuya::dp::numeric(105,"boost_time",1),
    ::zhc::tuya::dp::numeric(107,"comfort_temperature",1),
    ::zhc::tuya::dp::numeric(108,"eco_temperature",1),
    ::zhc::tuya::dp::binary(110,"battery_low"),
    ::zhc::tuya::dp::binary(115,"window_open")};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,13}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={
    "_TZE200_2atgpdho","_TZE200_4eeyebrt","_TZE200_7fqkphoq","_TZE200_8thwkzxl",
    "_TZE200_8whxpsiw","_TZE200_9sfg7gm0","_TZE200_ckud7u2l","_TZE200_do5qy8zo",
    "_TZE200_lpwgshtl","_TZE200_pvvbommb","_TZE200_qjp4ynvi","_TZE200_rk1wojce",
    "_TZE200_rndg81sf","_TZE200_rufdtfyv","_TZE200_ywdxldoj","_TZE200_znlqjmih",
    "_TZE284_znlqjmih"};
constexpr Expose kExp[]={
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDefTS0601_thermostat{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=17,.model="TS0601_thermostat",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=2,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
