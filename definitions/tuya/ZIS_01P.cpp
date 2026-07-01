// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 radar+PIR presence ZIS-01P _TZE284_vceqncho (z2m v26.76.0 parity).
// v26.35->v26.76 delta: adds DP106 delay_time + DP160-165 PIR/radar threshold/timing config.
// z2m-source: tuya.ts #ZIS-01P
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"occupancy"),
    ::zhc::tuya::dp::numeric(101,"radar_delay",1),
    ::zhc::tuya::dp::numeric(102,"presence_distance",1),
    ::zhc::tuya::dp::numeric(103,"presence_sensitivity",1),
    ::zhc::tuya::dp::binary(104,"radar_switch"),
    ::zhc::tuya::dp::numeric(105,"pir_sensitivity",1),
    ::zhc::tuya::dp::numeric(106,"delay_time",1),
    ::zhc::tuya::dp::binary(107,"led_switch"),
    ::zhc::tuya::dp::numeric(108,"illuminance",1),
    ::zhc::tuya::dp::numeric(109,"battery",1),
    ::zhc::tuya::dp::numeric(160,"pir_threshold",1),
    ::zhc::tuya::dp::numeric(161,"pir_trigger_pulses",1),
    ::zhc::tuya::dp::numeric(162,"pir_trigger_time",1),
    ::zhc::tuya::dp::numeric(163,"pir_lock_time",1),
    ::zhc::tuya::dp::numeric(164,"radar_threshold",1),
    ::zhc::tuya::dp::numeric(165,"radar_distance_door_test",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,16}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE284_vceqncho"};
constexpr Expose kExp[]={
    {"occupancy",                ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"illuminance",              ExposeType::Numeric, Access::State,    "lx",    nullptr, nullptr, 0},
    {"battery",                  ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"radar_delay",              ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr, 0},
    {"presence_distance",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"presence_sensitivity",     ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"radar_switch",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pir_sensitivity",          ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"delay_time",               ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr, 0},
    {"led_switch",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pir_threshold",            ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pir_trigger_pulses",       ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pir_trigger_time",         ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr, 0},
    {"pir_lock_time",            ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr, 0},
    {"radar_threshold",          ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"radar_distance_door_test", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0}};
}
extern const PreparedDefinition kDef_ZIS_01P{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="ZIS-01P",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
