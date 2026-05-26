// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 temperature/humidity sensor, _TZE200_zl1kmjqx variant
// (legacy tuya_temperature_humidity_sensor). Identical to the _1 sensor EXCEPT
// the converter divides humidity by 10 for this manufacturer:
//   DP1 temperature /10, DP2 humidity /10, DP3 battery_level, DP4 battery %.
// Not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kBattLvl_zl[]={ {0,"low"},{1,"middle"},{2,"high"} };
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(1),                              // DP1 /10
    ::zhc::tuya::dp::numeric(2,"humidity",10),                    // DP2 /10
    ::zhc::tuya::dp::enum_lookup(3,"battery_level",kBattLvl_zl,3),// DP3
    ::zhc::tuya::dp::numeric(4,"battery",1)};                     // DP4 %
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_zl1kmjqx"};
}
extern const PreparedDefinition kDefTS0601_th_sensor_zl{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,
    .model="TS0601_temperature_humidity_sensor_zl1kmjqx",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
