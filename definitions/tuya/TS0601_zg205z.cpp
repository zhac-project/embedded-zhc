// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya ZG-205Z/A 5.8/24GHz human-presence radar (_TZE200_crq3r3la).
// z2m-source: tuya.ts declarative meta.tuyaDatapoints. Core + multi-zone config:
//   DP1 presence (trueFalse1), DP2/105/109 large/medium/small sensitivity,
//   DP4/104/108 large/medium/small detection_distance /100, DP102 fading_time,
//   DP106 illuminance, DP107 indicator (LED), DP122 target_distance /100,
//   DP123 minimum_range /100.
// motion_state enum (DP/lookup uncertain in source) omitted. Config DPs are
// STATE_SET in z2m; read-only here. BEST-EFFORT, not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"presence"),
    ::zhc::tuya::dp::numeric(2,"large_motion_detection_sensitivity",1),
    ::zhc::tuya::dp::numeric(4,"large_motion_detection_distance",100),
    ::zhc::tuya::dp::numeric(102,"fading_time",1),
    ::zhc::tuya::dp::numeric(104,"medium_motion_detection_distance",100),
    ::zhc::tuya::dp::numeric(105,"medium_motion_detection_sensitivity",1),
    ::zhc::tuya::dp::numeric(106,"illuminance",1),
    ::zhc::tuya::dp::binary(107,"indicator"),
    ::zhc::tuya::dp::numeric(108,"small_detection_distance",100),
    ::zhc::tuya::dp::numeric(109,"small_detection_sensitivity",1),
    ::zhc::tuya::dp::numeric(122,"target_distance",100),
    ::zhc::tuya::dp::numeric(123,"minimum_range",100)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,12}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_crq3r3la"};
}
extern const PreparedDefinition kDefTS0601_zg205z{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="ZG-205Z/A",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
