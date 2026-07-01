// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 cover + 2-gang switch combo _TZE204_trwaxi57 (z2m v26.76.0 parity).
// z2m-source: tuya.ts #TS0601_cover_switch_2
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"open"},{1,"stop"},{2,"close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kCal[]={{0,"start"},{1,"end"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSteer[]={{0,"forward"},{1,"backward"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,3),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::enum_lookup(3,"calibration",kCal,2),
    ::zhc::tuya::dp::binary(7,"backlight_mode"),
    ::zhc::tuya::dp::enum_lookup(8,"motor_steering",kSteer,2),
    ::zhc::tuya::dp::binary(102,"child_lock"),
    ::zhc::tuya::dp::binary(107,"state_l2"),
    ::zhc::tuya::dp::binary(108,"state_l1")};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,8}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE204_trwaxi57"};
constexpr const char* kStOpts[]={"open","stop","close"};
constexpr const char* kCalOpts[]={"start","end"};
constexpr const char* kSteerOpts[]={"forward","backward"};
constexpr Expose kExp[]={
    {"state",          ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,    3},
    {"position",       ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},
    {"calibration",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kCalOpts,   2},
    {"backlight_mode", ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,    0},
    {"motor_steering", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSteerOpts, 2},
    {"child_lock",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,    0},
    {"state_l1",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,    0},
    {"state_l2",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,    0}};
}
extern const PreparedDefinition kDef_TS0601_cover_switch_2{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_cover_switch_2",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
