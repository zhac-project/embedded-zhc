// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: NOUS LCD temperature/humidity sensor (_TZE200_whkgqxse, legacy
// nous_lcd_temperature_humidity_sensor). CORE readings only:
//   DP1 temperature /10, DP2 humidity, DP4 battery %.
// z2m also exposes settable config (min/max temp+humidity, alarms, sensitivity,
// report intervals, temp unit) — omitted here (STATE_SET, no tz wired).
// Best-effort, not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(1),            // DP1 /10
    ::zhc::tuya::dp::numeric(2,"humidity",1),   // DP2 raw
    ::zhc::tuya::dp::numeric(4,"battery",1)};   // DP4 %
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,3}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_whkgqxse"};
}
extern const PreparedDefinition kDefTS0601_nous_lcd_th{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_nous_lcd_th",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
