// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TLC2206 water-level sensor _TZE200_lvkk0hdg (z2m v26.77.0 parity).
// z2m-source: tuya.ts #TLC2206. Was misclassified in ZHAC as TS0601_soil_th;
// z2m maps _TZE200_lvkk0hdg to a WATER-LEVEL sensor, not soil. Live DPs only:
// DP1 liquid_state (level enum), DP2 liquid_depth /100 (m), DP22
// liquid_level_percent (raw %). The config DPs (7 max_set / 8 min_set /
// 19 installation_height / 21 liquid_depth_max — all SET) are deferred per
// the ZHAC SET-only-config convention.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
// z2m tvc.level = lookup({normal:0, low:1, high:2}) — value-keyed.
constexpr ::zhc::tuya::TuyaEnumEntry kLiquidState[]={{0,"normal"},{1,"low"},{2,"high"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"liquid_state",kLiquidState,3),
    ::zhc::tuya::dp::numeric(2,"liquid_depth",100),          // z2m divideBy100 (m)
    ::zhc::tuya::dp::numeric(22,"liquid_level_percent",1)};  // z2m raw (%)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,3}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;  // read-only sensor: fz decode only, no tz
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_lvkk0hdg"};
constexpr const char* kLiquidStateOpts[]={"low","normal","high"};
constexpr Expose kExp[]={
    {"liquid_state",         ExposeType::Enum,    Access::State, nullptr, "Liquid level status", kLiquidStateOpts, 3},
    {"liquid_depth",         ExposeType::Numeric, Access::State, "m",     "Liquid depth",        nullptr, 0},
    {"liquid_level_percent", ExposeType::Numeric, Access::State, "%",     "Liquid level ratio",  nullptr, 0}};
}
extern const PreparedDefinition kDef_TLC2206{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TLC2206",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
