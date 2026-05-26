// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: _TZE200_dwcarsat / _TZE204_dwcarsat "Smart Air Housekeeper"
// air-quality monitor (temperature, humidity, VOC, CO2, PM2.5).
//
// z2m-source: tuya.ts model "TS0601_smart_air_house_keeper", which decodes
// via legacy.fromZigbee.tuya_air_quality (lib/legacy.ts). That legacy
// converter REMAPS two datapoints for the dwcarsat variants specifically:
//   DP22 (tuyaSabFormaldehyd) -> co2   (not formaldehyde)
//   DP2  (tuyaSabCO2)         -> pm25  (not co2)
// The generic Tuya air box uses the opposite mapping, so this is a
// device-specific port — the zhac-tools generator can't translate the
// legacy JS branches and left a placeholder (see zhac-tools ticket:
// legacy-converter stub coverage). Keep this hand-port until the
// generator learns to emit legacy-converter device maps.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(18),            // DP18 tuyaSabTemp,     /10
    ::zhc::tuya::dp::numeric(19,"humidity",10),  // DP19 tuyaSabHumidity, /10
    ::zhc::tuya::dp::numeric(21,"voc",1),        // DP21 tuyaSabVOC,      raw (ppb)
    ::zhc::tuya::dp::numeric(22,"co2",1),        // DP22 dwcarsat remap -> co2
    ::zhc::tuya::dp::numeric(2,"pm25",1)};       // DP2  dwcarsat remap -> pm25
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,5}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_dwcarsat","_TZE204_dwcarsat"};
}
extern const PreparedDefinition kDefTS0601_air_house_keeper{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="TS0601_air_house_keeper",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
