// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 garage-door opener (z2m TS0601_garage_door_opener,
// modernExtend dp*): DP1 trigger (open=true/close=false), DP2 countdown,
// DP3 garage_door_contact (closed=true/open=false), DP4 run_time,
// DP5 open_alarm_time. (DP12 status enum omitted — lookup not extracted.)
// READ + WRITE via factory::TuyaRw (trigger exposed StateSet).
// *** SECURITY: `trigger` opens/closes a physical garage door. The write
// encode is host-tested but on-device actuation is UNVERIFIED — validate on
// hardware AND review remote-access exposure before enabling control. ***
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"trigger"),               // DP1 open/close
    ::zhc::tuya::dp::numeric(2,"countdown",1),          // DP2
    ::zhc::tuya::dp::binary(3,"garage_door_contact"),   // DP3 closed=true
    ::zhc::tuya::dp::numeric(4,"run_time",1),           // DP4
    ::zhc::tuya::dp::numeric(5,"open_alarm_time",1)};   // DP5
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,5}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={
    "_TZE200_nklqjk62","_TZE204_nklqjk62","_TZE284_nklqjk62","_TZE204_jktmrpoj"};
constexpr Expose kExp[]={
    {"trigger", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDefTS0601_garage{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=4,.model="TS0601_garage_door_opener",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=1,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
