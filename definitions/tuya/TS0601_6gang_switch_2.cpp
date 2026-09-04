// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_6gang_switch_2 (TS0601 / _TZE284_hbxadcl0) -- 6-gang
// touch panel with power monitoring and configurable indicator colours
// (z2m v26.105.0, #13085).
//
// Keys follow upstream's endpoint naming (`state_button_1` .. `_6`,
// `state_all`); per-gang power-on behaviour rides dp29..34, the global one
// dp15. dp107 -- a 61-byte blob of six HSB indicator slots with separate
// on/off brightness -- is NOT ported: ZHC has no composite colour value and
// the write side needs a read-modify-write over the whole blob.
// z2m-source: tuya.ts #TS0601_6gang_switch_2.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPob[] = { {0,"off"}, {1,"on"}, {2,"previous"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1,   "state_button_1"),
        ::zhc::tuya::dp::binary(2,   "state_button_2"),
        ::zhc::tuya::dp::binary(3,   "state_button_3"),
        ::zhc::tuya::dp::binary(4,   "state_button_4"),
        ::zhc::tuya::dp::binary(5,   "state_button_5"),
        ::zhc::tuya::dp::binary(6,   "state_button_6"),
        ::zhc::tuya::dp::enum_lookup(15, "power_on_behavior", kPob, 3),
        ::zhc::tuya::dp::binary(16,  "backlight"),
        ::zhc::tuya::dp::numeric(20, "energy",  100),
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power",   10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        ::zhc::tuya::dp::enum_lookup(29, "power_on_behavior_1", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(30, "power_on_behavior_2", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(31, "power_on_behavior_3", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(32, "power_on_behavior_4", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(33, "power_on_behavior_5", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(34, "power_on_behavior_6", kPob, 3),
        ::zhc::tuya::dp::binary(101, "child_lock"),
        ::zhc::tuya::dp::binary(109, "indicator"),
        ::zhc::tuya::dp::binary(136, "state_all"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kPobOpts[] = { "off", "on", "previous" };
constexpr Expose kExp[] = {
    {"state_button_1", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_button_2", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_button_3", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_button_4", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_button_5", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_button_6", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_all",      ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "All gangs", nullptr, 0},
    {"energy",  ExposeType::Numeric, ::zhc::Access::State, "kWh", nullptr, nullptr, 0},
    {"power",   ExposeType::Numeric, ::zhc::Access::State, "W",   nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, ::zhc::Access::State, "A",   nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, ::zhc::Access::State, "V",   nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"backlight",  ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "Indicator backlight", nullptr, 0, ExposeCategory::Config},
    {"indicator",  ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "LED indicator", nullptr, 0, ExposeCategory::Config},
    {"power_on_behavior",   ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kPobOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_1", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Button 1 power-on behavior", kPobOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_2", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Button 2 power-on behavior", kPobOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_3", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Button 3 power-on behavior", kPobOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_4", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Button 4 power-on behavior", kPobOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_5", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Button 5 power-on behavior", kPobOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_6", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Button 6 power-on behavior", kPobOpts, 3, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_hbxadcl0" };
}  // namespace

extern const PreparedDefinition kDef_TS0601_6gang_switch_2{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_6gang_switch_2",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
