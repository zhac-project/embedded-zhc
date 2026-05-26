// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 mmWave human-presence radar family (legacy
// tuya_smart_human_presense_sensor, "tshps" DPs):
//   DP1 presence, DP2 radar_sensitivity, DP3 minimum_range /100,
//   DP4 maximum_range /100, DP9 target_distance /100, DP101 detection_delay /10,
//   DP102 fading_time /10, DP104 illuminance.
// DP6 self_test (enum, no table available) and DP103 cli (z2m "not recognize")
// are skipped. The config DPs (sensitivity/ranges/delays) are STATE_SET in z2m
// but read-only here (no tz). BEST-EFFORT, not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"presence"),                // DP1
    ::zhc::tuya::dp::numeric(2,"radar_sensitivity",1),    // DP2
    ::zhc::tuya::dp::numeric(3,"minimum_range",100),      // DP3 /100 m
    ::zhc::tuya::dp::numeric(4,"maximum_range",100),      // DP4 /100 m
    ::zhc::tuya::dp::numeric(9,"target_distance",100),    // DP9 /100 m
    ::zhc::tuya::dp::numeric(101,"detection_delay",10),   // DP101 /10 s
    ::zhc::tuya::dp::numeric(102,"fading_time",10),       // DP102 /10 s
    ::zhc::tuya::dp::numeric(104,"illuminance",1)};       // DP104 lux
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,8}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={
    "_TZE200_holel4dk","_TZE200_jva8ink8","_TZE200_lyetpprm","_TZE200_sgpeacqp",
    "_TZE200_wukb7rhc","_TZE200_xpq2rzhq","_TZE204_fwondbzy","_TZE204_xpq2rzhq",
    "_TZE204_xsm7l9xa","_TZE204_ztc6ggyl","_TZE284_fwondbzy"};
}
extern const PreparedDefinition kDefTS0601_tshps_radar{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=11,.model="TS0601_radar_presence",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
