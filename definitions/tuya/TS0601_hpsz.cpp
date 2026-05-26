// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 human-presence sensor (legacy hpsz):
//   DP1 presence (value == 1), DP101 duration_of_attendance,
//   DP102 duration_of_absence, DP103 led_state.
// led_state is STATE_SET in z2m; read-only here. Best-effort, not HW-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"presence"),                       // DP1 ==1
    ::zhc::tuya::dp::numeric(101,"duration_of_attendance",1),    // DP101 s
    ::zhc::tuya::dp::numeric(102,"duration_of_absence",1),       // DP102 s
    ::zhc::tuya::dp::binary(103,"led_state")};                   // DP103
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_0u3bj3rc","_TZE200_mx6u6l4y","_TZE200_v6ossqfy"};
}
extern const PreparedDefinition kDefTS0601_hpsz{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=3,.model="TS0601_human_presence_sensor",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
