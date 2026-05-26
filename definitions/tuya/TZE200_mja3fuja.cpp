// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: _TZE200_mja3fuja "Smart Air Box" air-quality monitor
// (temperature, humidity, VOC, CO2, formaldehyde).
//
// z2m-source: tuya.ts via legacy.fromZigbee.tuya_air_quality (lib/legacy.ts).
// The mja3fuja branch remaps two datapoints: DP22 (tuyaSabFormaldehyd) -> co2,
// DP2 (tuyaSabCO2) -> formaldehyde. (Its dwcarsat sibling instead maps
// DP2 -> pm25; see TS0601_air_house_keeper.cpp.) The generator emitted only a
// placeholder for this legacy-converter device — see zhac-tools ticket
// extra/docs/ZHAC_TOOLS_TICKET_legacy_converter_stubs.md.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(18),              // DP18 tuyaSabTemp,     /10
    ::zhc::tuya::dp::numeric(19,"humidity",10),    // DP19 tuyaSabHumidity, /10
    ::zhc::tuya::dp::numeric(21,"voc",1),          // DP21 tuyaSabVOC,      raw
    ::zhc::tuya::dp::numeric(22,"co2",1),          // DP22 mja3fuja remap -> co2
    ::zhc::tuya::dp::numeric(2,"formaldehyde",1)}; // DP2  mja3fuja remap -> formaldehyde
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,5}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_mja3fuja"};
}
extern const PreparedDefinition kDefTZE200_mja3fuja{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_smart_air_box",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
