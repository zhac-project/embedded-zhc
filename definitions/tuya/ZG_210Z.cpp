// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOBEIAN ZG-210Z pressure/bed-occupancy sensor (z2m v26.76.0 parity).
// Matched by zigbeeModel "ZG-210Z" (no manufacturerName fingerprint in z2m).
// temperature /10; temperature_calibration /10 signed (localTempCalibration3);
// humidity_calibration /1 signed (localTempCalibration2); other numerics raw.
// z2m-source: tuya.ts #ZG-210Z
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPs[]={{0,"none"},{1,"sit"},{2,"sedentary"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"pressure_state",kPs,3),
    ::zhc::tuya::dp::numeric(2,"current_pressure",1),
    ::zhc::tuya::dp::numeric(101,"temperature",10),
    ::zhc::tuya::dp::numeric(102,"humidity",1),
    ::zhc::tuya::dp::numeric(15,"battery",1),
    ::zhc::tuya::dp::numeric(105,"pressure_intensity",1),
    ::zhc::tuya::dp::numeric(106,"presence_delay",1),
    ::zhc::tuya::dp::numeric(107,"sedentary_reminder",1),
    ::zhc::tuya::dp::numeric(103,"temperature_calibration",10),
    ::zhc::tuya::dp::numeric(104,"humidity_calibration",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,10}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"ZG-210Z"};
constexpr const char* kPsOpts[]={"none","sit","sedentary"};
constexpr Expose kExp[]={
    {"pressure_state",          ExposeType::Enum,    Access::State, nullptr, nullptr, kPsOpts, 3},
    {"current_pressure",        ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature",             ExposeType::Numeric, Access::State, "C",     nullptr, nullptr, 0},
    {"humidity",                ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"battery",                 ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"pressure_intensity",      ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"presence_delay",          ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
    {"sedentary_reminder",      ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"humidity_calibration",    ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDef_ZG_210Z{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr,.manufacturer_names_count=0,.model="ZG-210Z",
    .vendor="HOBEIAN",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
