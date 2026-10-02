// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AutomatOn CH8Z underfloor heating controller, 8 zones (z2m v26.113.0, #13275).
// All channels are datapoints on endpoint 1, so `state_l1`..`state_l8` are
// datapoint keys. DP17-26 (metering) and DP28 are in Tuya's spec but not
// implemented by this firmware (upstream's note); DP101/102 schedules not ported.
// z2m-source: automaton.ts #CH8Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::automaton {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior27[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::binary(3, "state_l3"),
        ::zhc::tuya::dp::binary(4, "state_l4"),
        ::zhc::tuya::dp::binary(5, "state_l5"),
        ::zhc::tuya::dp::binary(6, "state_l6"),
        ::zhc::tuya::dp::binary(7, "state_l7"),
        ::zhc::tuya::dp::binary(8, "state_l8"),
        ::zhc::tuya::dp::numeric(9, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(10, "countdown_l2", 1),
        ::zhc::tuya::dp::numeric(11, "countdown_l3", 1),
        ::zhc::tuya::dp::numeric(12, "countdown_l4", 1),
        ::zhc::tuya::dp::numeric(13, "countdown_l5", 1),
        ::zhc::tuya::dp::numeric(14, "countdown_l6", 1),
        ::zhc::tuya::dp::numeric(15, "countdown_l7", 1),
        ::zhc::tuya::dp::numeric(16, "countdown_l8", 1),
        ::zhc::tuya::dp::enum_lookup(27, "power_on_behavior", kPowerOnBehavior27, 3),
        ::zhc::tuya::dp::binary(29, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts16[] = { "off", "on", "previous" };
constexpr Expose kExp[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l6", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l7", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l8", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l3", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l4", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l5", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l6", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l7", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l8", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Controls the behavior when the device is powered on after power loss", kOpts16, 3},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Enables/disables physical input on the device", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_1oft6qso" };
}  // namespace

extern const PreparedDefinition kDef_CH8Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="CH8Z",.vendor="AutomatOn",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::automaton
