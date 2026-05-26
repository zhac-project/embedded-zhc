// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_din DIN-rail energy meter / breaker DDS238-2
// (_TZE200_byzdayie, _TZE200_ewxhg6o9, legacy tuya_dinrail_switch):
//   DP1 state (relay on/off), DP17 energy /100 (kWh), DP18 current /1000 (A),
//   DP19 power /10 (W), DP20 voltage /10 (V).
// READ + WRITE: relay control via factory::TuyaRw (state exposed StateSet).
// Energy/power/etc. surface via the shadow. WARNING: this drives a mains
// breaker relay — the write encode is host-tested but on-device switching is
// UNVERIFIED (no hardware); validate before relying on remote control.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state"),          // DP1 relay
    ::zhc::tuya::dp::numeric(17,"energy",100),   // DP17 /100 kWh
    ::zhc::tuya::dp::numeric(18,"current",1000), // DP18 /1000 A
    ::zhc::tuya::dp::numeric(19,"power",10),     // DP19 /10 W
    ::zhc::tuya::dp::numeric(20,"voltage",10)};  // DP20 /10 V
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,5}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_byzdayie","_TZE200_ewxhg6o9"};
constexpr Expose kExp[]={
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDefTS0601_din{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="TS0601_din",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=1,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
