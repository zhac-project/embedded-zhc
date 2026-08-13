// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital Topazio TO-6 W/B 6-gang wall switch (z2m v26.95.0 parity).
// z2m-source: tuya.ts #TO-6 W/B (added v26.95.0)
// Datapoint map emitted from the upstream tuyaDatapoints triples; each
// divisor is the one that device's own valueConverter uses. Where upstream
// maps ONE datapoint to several keys with different lookups, the emitter
// names both tables after the DP and they collide -- those are suffixed _vN.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_1[] = { {0,"scene_1"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_2[] = { {0,"scene_2"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_3[] = { {0,"scene_3"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_4[] = { {0,"scene_4"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_5[] = { {0,"scene_5"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_6[] = { {0,"scene_6"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_18[] = { {0,"switch_1"},{1,"scene_1"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_19[] = { {0,"switch_2"},{1,"scene_2"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_20[] = { {0,"switch_3"},{1,"scene_3"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_21[] = { {0,"switch_4"},{1,"scene_4"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_22[] = { {0,"switch_5"},{1,"scene_5"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_23[] = { {0,"switch_6"},{1,"scene_6"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_24[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_25[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_26[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_27[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_28[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_29[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_36[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_37[] = { {0,"off"},{1,"relay"},{2,"invert"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_38[] = { {0,"off"},{1,"on"},{2,"previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_rqhnxkqu_104[] = { {0,"off"},{1,"low"},{2,"medium"},{3,"high"} };

struct cfg_TO_6_W_B {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
    ::zhc::tuya::dp::action(1,kEnum_rqhnxkqu_1,1),
    ::zhc::tuya::dp::action(2,kEnum_rqhnxkqu_2,1),
    ::zhc::tuya::dp::action(3,kEnum_rqhnxkqu_3,1),
    ::zhc::tuya::dp::action(4,kEnum_rqhnxkqu_4,1),
    ::zhc::tuya::dp::action(5,kEnum_rqhnxkqu_5,1),
    ::zhc::tuya::dp::action(6,kEnum_rqhnxkqu_6,1),
    ::zhc::tuya::dp::enum_lookup(18,"mode_1",kEnum_rqhnxkqu_18,2),
    ::zhc::tuya::dp::enum_lookup(19,"mode_2",kEnum_rqhnxkqu_19,2),
    ::zhc::tuya::dp::enum_lookup(20,"mode_3",kEnum_rqhnxkqu_20,2),
    ::zhc::tuya::dp::enum_lookup(21,"mode_4",kEnum_rqhnxkqu_21,2),
    ::zhc::tuya::dp::enum_lookup(22,"mode_5",kEnum_rqhnxkqu_22,2),
    ::zhc::tuya::dp::enum_lookup(23,"mode_6",kEnum_rqhnxkqu_23,2),
    ::zhc::tuya::dp::enum_lookup(24,"state_l1",kEnum_rqhnxkqu_24,2),
    ::zhc::tuya::dp::enum_lookup(25,"state_l2",kEnum_rqhnxkqu_25,2),
    ::zhc::tuya::dp::enum_lookup(26,"state_l3",kEnum_rqhnxkqu_26,2),
    ::zhc::tuya::dp::enum_lookup(27,"state_l4",kEnum_rqhnxkqu_27,2),
    ::zhc::tuya::dp::enum_lookup(28,"state_l5",kEnum_rqhnxkqu_28,2),
    ::zhc::tuya::dp::enum_lookup(29,"state_l6",kEnum_rqhnxkqu_29,2),
    ::zhc::tuya::dp::numeric(30,"countdown_l1",1),
    ::zhc::tuya::dp::numeric(31,"countdown_l2",1),
    ::zhc::tuya::dp::numeric(32,"countdown_l3",1),
    ::zhc::tuya::dp::numeric(33,"countdown_l4",1),
    ::zhc::tuya::dp::numeric(34,"countdown_l5",1),
    ::zhc::tuya::dp::numeric(35,"countdown_l6",1),
    ::zhc::tuya::dp::enum_lookup(36,"backlight_mode",kEnum_rqhnxkqu_36,2),
    ::zhc::tuya::dp::enum_lookup(37,"indicator_status",kEnum_rqhnxkqu_37,3),
    ::zhc::tuya::dp::enum_lookup(38,"power_on_behavior",kEnum_rqhnxkqu_38,3),
    ::zhc::tuya::dp::binary(103,"induction"),
    ::zhc::tuya::dp::enum_lookup(104,"vibration_gear",kEnum_rqhnxkqu_104,4),
    ::zhc::tuya::dp::numeric(105,"inching_l1",1),
    ::zhc::tuya::dp::numeric(106,"inching_l2",1),
    ::zhc::tuya::dp::numeric(107,"inching_l3",1),
    ::zhc::tuya::dp::numeric(108,"inching_l4",1),
    ::zhc::tuya::dp::numeric(109,"inching_l5",1),
    ::zhc::tuya::dp::numeric(110,"inching_l6",1)
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_TO_6_W_B>;

constexpr const char* kOpts_TO_6_W_B_mode_1[] = { "switch_1", "scene_1" };
constexpr const char* kOpts_TO_6_W_B_mode_2[] = { "switch_2", "scene_2" };
constexpr const char* kOpts_TO_6_W_B_mode_3[] = { "switch_3", "scene_3" };
constexpr const char* kOpts_TO_6_W_B_mode_4[] = { "switch_4", "scene_4" };
constexpr const char* kOpts_TO_6_W_B_mode_5[] = { "switch_5", "scene_5" };
constexpr const char* kOpts_TO_6_W_B_mode_6[] = { "switch_6", "scene_6" };
constexpr const char* kOpts_TO_6_W_B_state_l1[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_state_l2[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_state_l3[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_state_l4[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_state_l5[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_state_l6[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_backlight_mode[] = { "OFF", "ON" };
constexpr const char* kOpts_TO_6_W_B_indicator_status[] = { "off", "relay", "invert" };
constexpr const char* kOpts_TO_6_W_B_power_on_behavior[] = { "off", "on", "previous" };
constexpr const char* kOpts_TO_6_W_B_vibration_gear[] = { "off", "low", "medium", "high" };

constexpr Expose kExp_TO_6_W_B[] = {
    {"mode_1", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_mode_1, 2},
    {"mode_2", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_mode_2, 2},
    {"mode_3", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_mode_3, 2},
    {"mode_4", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_mode_4, 2},
    {"mode_5", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_mode_5, 2},
    {"mode_6", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_mode_6, 2},
    {"state_l1", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_state_l1, 2},
    {"state_l2", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_state_l2, 2},
    {"state_l3", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_state_l3, 2},
    {"state_l4", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_state_l4, 2},
    {"state_l5", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_state_l5, 2},
    {"state_l6", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_state_l6, 2},
    {"countdown_l1", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l2", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l3", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l4", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l5", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l6", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"backlight_mode", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_backlight_mode, 2},
    {"indicator_status", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_indicator_status, 3},
    {"power_on_behavior", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_power_on_behavior, 3},
    {"induction", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"vibration_gear", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TO_6_W_B_vibration_gear, 4},
    {"inching_l1", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"inching_l2", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"inching_l3", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"inching_l4", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"inching_l5", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"inching_l6", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr const char* kM_TO_6_W_B[] = { "TS0601" };
constexpr const char* kN_TO_6_W_B[] = { "_TZE200_rqhnxkqu" };
}  // namespace

extern const PreparedDefinition kDef_TO_6_W_B{
    .zigbee_models=kM_TO_6_W_B,.zigbee_models_count=sizeof(kM_TO_6_W_B)/sizeof(kM_TO_6_W_B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_TO_6_W_B,.manufacturer_names_count=sizeof(kN_TO_6_W_B)/sizeof(kN_TO_6_W_B[0]),
    .model="TO-6 W/B",.vendor="Nova Digital",.meta=nullptr,
    .exposes=kExp_TO_6_W_B,.exposes_count=sizeof(kExp_TO_6_W_B)/sizeof(kExp_TO_6_W_B[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
