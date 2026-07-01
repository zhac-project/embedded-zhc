// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 presence + T/H + illuminance sensor _TZE284_gnpflcoq (z2m v26.76.0 parity).
// temperature /10; humidity, illuminance, battery, sensitivity, fading_time are raw (/1).
// z2m-source: tuya.ts #TS0601_TZE284_gnpflcoq
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary_inv(1,"presence"),      // DP1 trueFalse0
    ::zhc::tuya::dp::numeric(2,"sensitivity",1),    // DP2 raw
    ::zhc::tuya::dp::numeric(4,"battery",1),        // DP4 raw (%)
    ::zhc::tuya::dp::numeric(7,"temperature",10),   // DP7 divideBy10 (C)
    ::zhc::tuya::dp::numeric(8,"humidity",1),       // DP8 raw (%)
    ::zhc::tuya::dp::numeric(11,"illuminance",1),   // DP11 raw (lx)
    ::zhc::tuya::dp::numeric(102,"fading_time",1)}; // DP102 raw (s)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,7}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE284_gnpflcoq"};
constexpr Expose kExp[]={
    {"presence",    ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",     nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0},
    {"fading_time", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDef_TS0601_TZE284_gnpflcoq{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_TZE284_gnpflcoq",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
