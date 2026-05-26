// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 1-gang switch (z2m TS0601_switch_1_gang, legacy
// tuya_switch, non-multiEndpoint): DP1 <-> state (bool, ON = 1).
// READ + WRITE (control via tz_tuya_datapoints / factory::TuyaRw). The write
// encode is host-tested; on-device on/off behaviour is unverified (no hardware).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state")};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,1}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_ojtqawav","_TZE204_ojtqawav","_TZE204_gbagoilo"};
constexpr Expose kExp[]={
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDefTS0601_switch_1g{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=3,.model="TS0601_switch_1_gang",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=1,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
