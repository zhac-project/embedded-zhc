// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0301_cover_2 cover motor (z2m v26.95.0 parity, added v26.93.0).
// z2m-source: tuya.ts #TS0301_cover_2  (TS0301 / _TZE210_xgzzuerd)
//
// Every enum here comes from a NAMED z2m converter rather than an inline
// lookup, so the tables are transcribed from lib/tuya.ts directly:
//   coverAction            {OPEN:0, STOP:1, CLOSE:2, CONTINUE:3}
//   motorState             {opening:0, closing:1, stopped:2}
//   onOffEnumOn1           {OFF:0, ON:1}
//   tubularMotorDirection  {normal:0, reversed:1}      (NOT forward/back)
//   coverType              {roman_pole:0, roller_blind:1, canopy_curtain:2,
//                           roman_blind:3, honeycomb_curtain:4}
//   coverLimit             {set_up:0, set_down:1, delete_up:2, delete_down:3,
//                           delete_both:4}
//   clickControl           {up:0, down:1}
// coverPosition is the identity map (divisor 1).
//
// DP8 and DP9 both carry "position" -- upstream maps both, and ZHC fires every
// matching row on decode, so both are listed deliberately.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kAction[]   = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotorSt[]  = { {0,"opening"}, {1,"closing"}, {2,"stopped"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSlowMode[] = { {0,"OFF"}, {1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]      = { {0,"normal"}, {1,"reversed"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCoverType[]= { {0,"roman_pole"}, {1,"roller_blind"}, {2,"canopy_curtain"},
                                                      {3,"roman_blind"}, {4,"honeycomb_curtain"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLimit[]    = { {0,"set_up"}, {1,"set_down"}, {2,"delete_up"},
                                                      {3,"delete_down"}, {4,"delete_both"} };
constexpr ::zhc::tuya::TuyaEnumEntry kClick[]    = { {0,"up"}, {1,"down"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 4),
        ::zhc::tuya::dp::enum_lookup(3, "motor_state", kMotorSt, 3),
        ::zhc::tuya::dp::enum_lookup(7, "slow_mode", kSlowMode, 2),
        ::zhc::tuya::dp::numeric(8, "position", 1),
        ::zhc::tuya::dp::numeric(9, "position", 1),
        ::zhc::tuya::dp::enum_lookup(11, "motor_direction", kDir, 2),
        ::zhc::tuya::dp::numeric(13, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(15, "cover_type", kCoverType, 5),
        ::zhc::tuya::dp::enum_lookup(16, "cover_limit", kLimit, 5),
        ::zhc::tuya::dp::numeric(19, "favorite_position", 1),
        ::zhc::tuya::dp::enum_lookup(20, "click_control", kClick, 2),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kActionOpts[]={"OPEN","STOP","CLOSE","CONTINUE"};
constexpr const char* kMotorStOpts[]={"opening","closing","stopped"};
constexpr const char* kSlowOpts[]={"OFF","ON"};
constexpr const char* kDirOpts[]={"normal","reversed"};
constexpr const char* kTypeOpts[]={"roman_pole","roller_blind","canopy_curtain","roman_blind","honeycomb_curtain"};
constexpr const char* kLimitOpts[]={"set_up","set_down","delete_up","delete_down","delete_both"};
constexpr const char* kClickOpts[]={"up","down"};
constexpr Expose kExp[] = {
    {"state",             ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover action",       kActionOpts,  4},
    {"position",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover position",     nullptr,      0},
    {"motor_state",       ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Motor state",        kMotorStOpts, 3},
    {"motor_direction",   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Motor direction",    kDirOpts,     2},
    {"slow_mode",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Slow mode",          kSlowOpts,    2},
    {"cover_type",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover type",         kTypeOpts,    5},
    {"cover_limit",       ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Limit setting",      kLimitOpts,   5},
    {"click_control",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Step control",       kClickOpts,   2},
    {"favorite_position", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Favourite position", nullptr,      0},
    {"battery",           ExposeType::Numeric, ::zhc::Access::State,    "%",     "Battery percentage", nullptr,      0},
};
constexpr const char* kM[] = { "TS0301" };
constexpr const char* kN[] = { "_TZE210_xgzzuerd" };
}  // namespace
extern const PreparedDefinition kDef_TS0301_cover_2{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0301_cover_2",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
