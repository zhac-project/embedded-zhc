// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 illuminance + temperature + humidity sensor
// (_TZE200_vzqtvljm, legacy tuya_illuminance_temperature_humidity_sensor):
//   DP8 temperature /10, DP9 humidity, DP3 battery %, DP7 illuminance (lux).
// Best-effort, not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(8),             // DP8 /10
    ::zhc::tuya::dp::numeric(9,"humidity",1),    // DP9 raw
    ::zhc::tuya::dp::numeric(3,"battery",1),     // DP3 %
    ::zhc::tuya::dp::numeric(7,"illuminance",1)};// DP7 lux
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_vzqtvljm"};
}
extern const PreparedDefinition kDefTS0601_ith_sensor{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,
    .model="TS0601_illuminance_temperature_humidity_sensor_1",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
