// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 multi-gang switch (z2m TS0601_switch, 1-4 gang, legacy
// tuya_switch): DP1..DP4 <-> state_l1..state_l4 (bool, ON = 1). Devices with
// fewer gangs never report/accept the higher DPs.
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
    ::zhc::tuya::dp::binary(3,"state_l3"),
    ::zhc::tuya::dp::binary(4,"state_l4")};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_js3mgbjb","_TZE204_ptaqh9tk"};
constexpr Expose kExp[]={
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDefTS0601_switch_4g{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="TS0601_switch",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=4,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
