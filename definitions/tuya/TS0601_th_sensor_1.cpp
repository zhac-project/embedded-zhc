// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 temperature/humidity sensor (z2m
// TS0601_temperature_humidity_sensor_1, legacy tuya_temperature_humidity_sensor).
//   DP1 temperature /10 (signed), DP2 humidity raw, DP3 battery_level
//   {low,middle,high}, DP4 battery %.
// z2m also derives battery_low (= battery_level == low); omitted here
// (best-effort). Not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kBattLvl_th1[]={ {0,"low"},{1,"middle"},{2,"high"} };
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(1),                               // DP1 /10
    ::zhc::tuya::dp::numeric(2,"humidity",1),                      // DP2 raw
    ::zhc::tuya::dp::enum_lookup(3,"battery_level",kBattLvl_th1,3),// DP3
    ::zhc::tuya::dp::numeric(4,"battery",1)};                      // DP4 %
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={
    "_TZE200_44af8vyi","_TZE200_qyflbnbj","_TZE204_qyflbnbj","_TZE284_qyflbnbj"};
}
extern const PreparedDefinition kDefTS0601_th_sensor_1{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=4,
    .model="TS0601_temperature_humidity_sensor_1",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
