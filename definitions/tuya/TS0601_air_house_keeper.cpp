// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: _TZE200_dwcarsat / _TZE204_dwcarsat "Smart Air Housekeeper"
// air-quality monitor (temperature, humidity, VOC, CO2, PM2.5, formaldehyde).
//
// z2m-source: tuya.ts model "TS0601_smart_air_house_keeper", which decodes
// via legacy.fromZigbee.tuya_air_quality (lib/legacy.ts). That legacy
// converter REMAPS two datapoints for the dwcarsat variants specifically:
//   DP22 (tuyaSabFormaldehyd) -> co2   (not formaldehyde)
//   DP2  (tuyaSabCO2)         -> pm25  (not co2)
// and decodes DP20 (tuyaSahkFormaldehyd) -> formaldehyd for every model.
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
struct cfg {
    static constexpr std::int32_t kPm25Max = 1000;   // z2m: "valid range is 0-1000 ug/m3"
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::temperature(18),            // DP18 tuyaSabTemp,     /10
    ::zhc::tuya::dp::numeric(19,"humidity",10),  // DP19 tuyaSabHumidity, /10
    ::zhc::tuya::dp::numeric(21,"voc",1),        // DP21 tuyaSabVOC,      raw (ppb)
    ::zhc::tuya::dp::numeric(22,"co2",1),        // DP22 dwcarsat remap -> co2
    ::zhc::tuya::dp::numeric_max(2,"pm25",&kPm25Max), // DP2 dwcarsat remap -> pm25, >1000 dropped
    ::zhc::tuya::dp::numeric(20,"formaldehyd",1)}; // DP20 tuyaSahkFormaldehyd, raw (µg/m³)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,6}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_dwcarsat","_TZE204_dwcarsat"};
// Exposes WITH UNITS so the local web-ui + cloud (which render units from device exposes)
// show them. Multi-sensor: keep all six readouts (the cloud archetype classifier treats a
// temperature+air-quality device as a multi-sensor `generic`, not a temp/humidity climate_sensor).
constexpr Expose kExp[]={
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "°C",    nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",     nullptr, nullptr, 0 },
    { "voc",         ExposeType::Numeric, ::zhc::Access::State, "ppb",   nullptr, nullptr, 0 },
    { "co2",         ExposeType::Numeric, ::zhc::Access::State, "ppm",   nullptr, nullptr, 0 },
    { "pm25",        ExposeType::Numeric, ::zhc::Access::State, "µg/m³", nullptr, nullptr, 0 },
    { "formaldehyd", ExposeType::Numeric, ::zhc::Access::State, "µg/m³", nullptr, nullptr, 0 },
};
}
extern const PreparedDefinition kDefTS0601_air_house_keeper{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="TS0601_air_house_keeper",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
