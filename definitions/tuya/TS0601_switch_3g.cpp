// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 3-gang switch (z2m TS0601_switch_3_gang, legacy
// tuya_switch): DP1..DP3 <-> state_l1..state_l3 (bool, ON = 1).
// READ + WRITE (control via tz_tuya_datapoints / factory::TuyaRw). Write encode
// host-tested; on-device behaviour unverified (no hardware).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state_l1"),
    ::zhc::tuya::dp::binary(2,"state_l2"),
    ::zhc::tuya::dp::binary(3,"state_l3")};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,3}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={
    "_TZE200_2hf7x9n3","_TZE200_atpwqgml","_TZE204_atpwqgml",
    "_TZE200_bynnczcb","_TZE200_kyfqmmyl"};
constexpr Expose kExp[]={
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDefTS0601_switch_3g{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=5,.model="TS0601_switch_3_gang",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=3,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
