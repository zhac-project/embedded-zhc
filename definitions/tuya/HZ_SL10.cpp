// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Haozee HZ-SL10 soil moisture sensor (z2m v26.76.0 parity).
// Matched by zigbeeModel "ZG-303B"/"ZG-303L" (no manufacturerName fingerprint in z2m).
// temperature /10; temperature_calibration /10 signed (localTempCalibration3);
// soil_calibration /1 signed (localTempCalibration2); other numerics raw.
// z2m-source: tuya.ts #HZ-SL10
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kUnit[]={{0,"celsius"},{1,"fahrenheit"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(106,"dry"),
    ::zhc::tuya::dp::numeric(5,"temperature",10),
    ::zhc::tuya::dp::numeric(3,"soil_moisture",1),
    ::zhc::tuya::dp::numeric(15,"battery",1),
    ::zhc::tuya::dp::enum_lookup(9,"temperature_unit",kUnit,2),
    ::zhc::tuya::dp::numeric(104,"temperature_calibration",10),
    ::zhc::tuya::dp::numeric(102,"soil_calibration",1),
    ::zhc::tuya::dp::numeric(111,"temperature_sampling",1),
    ::zhc::tuya::dp::numeric(112,"soil_sampling",1),
    ::zhc::tuya::dp::numeric(110,"soil_warning",1),
    ::zhc::tuya::dp::numeric(103,"illuminance",1),
    ::zhc::tuya::dp::numeric(107,"illuminance_sampling",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,12}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"ZG-303B","ZG-303L"};
constexpr const char* kUnitOpts[]={"celsius","fahrenheit"};
constexpr Expose kExp[]={
    {"dry",                     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr,   0},
    {"temperature",             ExposeType::Numeric, Access::State, "C",     nullptr, nullptr,   0},
    {"soil_moisture",           ExposeType::Numeric, Access::State, "%",     nullptr, nullptr,   0},
    {"battery",                 ExposeType::Numeric, Access::State, "%",     nullptr, nullptr,   0},
    {"temperature_unit",        ExposeType::Enum,    Access::State, nullptr, nullptr, kUnitOpts, 2},
    {"illuminance",             ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr,   0},
    {"temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr,   0},
    {"soil_calibration",        ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr,   0},
    {"temperature_sampling",    ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr,   0},
    {"soil_sampling",           ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr,   0},
    {"soil_warning",            ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr,   0},
    {"illuminance_sampling",    ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr,   0}};
}
extern const PreparedDefinition kDef_HZ_SL10{
    .zigbee_models=kM,.zigbee_models_count=2,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr,.manufacturer_names_count=0,.model="HZ-SL10",
    .vendor="Haozee",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
