// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 power-monitoring switch _TZE204_apiu8k13 (z2m v26.76.0 parity).
// Divisors mirror z2m exactly: energy /100 (kWh), current /1000 (A),
// power & voltage are raw (/1) per z2m tuya.valueConverter.raw.
// z2m-source: tuya.ts #TS0601_power_monitoring_switch
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state"),          // DP1 onOff
    ::zhc::tuya::dp::numeric(7,"countdown",1),   // DP7 countdown (s)
    ::zhc::tuya::dp::numeric(20,"energy",100),   // DP20 divideBy100 (kWh)
    ::zhc::tuya::dp::numeric(21,"current",1000), // DP21 divideBy1000 (A)
    ::zhc::tuya::dp::numeric(22,"power",1),       // DP22 raw (W)
    ::zhc::tuya::dp::numeric(23,"voltage",1)};    // DP23 raw (V)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,6}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE204_apiu8k13"};
constexpr Expose kExp[]={
    {"state",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown", ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr, 0},
    {"energy",    ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"current",   ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"power",     ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage",   ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDef_TS0601_power_monitoring_switch{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_power_monitoring_switch",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
