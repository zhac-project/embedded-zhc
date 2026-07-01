// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 illuminance sensor _TZE284_aaeasoll (z2m v26.76.0 parity).
// z2m-source: tuya.ts #TZE284_aaeasoll
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kRep[]={
    {0,"5m"},{1,"10m"},{2,"15m"},{3,"20m"},{4,"30m"},{5,"1h"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::numeric(2,"illuminance",1),                // DP2 raw (lx)
    ::zhc::tuya::dp::numeric(4,"battery",1),                    // DP4 raw (%)
    ::zhc::tuya::dp::enum_lookup(101,"report_interval",kRep,6)};// DP101 lookup
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,3}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE284_aaeasoll"};
constexpr const char* kRepOpts[]={"5m","10m","15m","20m","30m","1h"};
constexpr Expose kExp[]={
    {"illuminance",     ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr,  0},
    {"battery",         ExposeType::Numeric, Access::State, "%",     nullptr, nullptr,  0},
    {"report_interval", ExposeType::Enum,    Access::State, nullptr, nullptr, kRepOpts, 6}};
}
extern const PreparedDefinition kDef_TZE284_aaeasoll{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TZE284_aaeasoll",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
