// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 air-quality sensors (temperature, humidity, VOC, CO2,
// formaldehyde) that decode via legacy.fromZigbee.tuya_air_quality on its
// DEFAULT branch — i.e. every fingerprint NOT special-cased as dwcarsat /
// mja3fuja / ryfmq5rl. Default mapping:
//   DP18 -> temperature /10   DP19 -> humidity /10   DP21 -> voc (raw)
//   DP2  -> co2 (raw)         DP22 -> formaldehyde (raw)
// Covers z2m models TS0601_air_quality_sensor, TS0601_co2_sensor and
// TS0601_temperature_humidity_co2_sensor. Devices send only the DPs they
// support; unused entries simply never fire. The generator stubbed these
// because the z2m def uses a legacy converter (no meta.tuyaDatapoints) —
// see extra/docs/ZHAC_TOOLS_TICKET_legacy_converter_stubs.md.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(18),               // DP18 tuyaSabTemp,        /10
    ::zhc::tuya::dp::numeric(19,"humidity",10),     // DP19 tuyaSabHumidity,    /10
    ::zhc::tuya::dp::numeric(21,"voc",1),           // DP21 tuyaSabVOC,         raw
    ::zhc::tuya::dp::numeric(2,"co2",1),            // DP2  tuyaSabCO2,         raw (default)
    ::zhc::tuya::dp::numeric(22,"formaldehyde",1)}; // DP22 tuyaSabFormaldehyd, raw (default)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,5}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={
    "_TZE200_3ejwxpmu","_TZE204_3ejwxpmu",
    "_TZE200_ogkdpgy2","_TZE204_ogkdpgy2",
    "_TZE200_blfcpsxz",
    "_TZE200_c2fmom5z","_TZE204_c2fmom5z",
    "_TZE200_yvx5lh6k","_TZE204_yvx5lh6k"};
}
extern const PreparedDefinition kDefTS0601_air_quality{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=9,.model="TS0601_air_quality",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
