// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya OXT-4CH-RELAY-AC 4-channel relay module, TS0601
// _TZE28C1000000_f5efvtbv + _TZE204_m9dzckna (white-label OXT-4CH-RELAY-DC)
// (z2m v26.125.0 window). DPs 1-4 state_l1-l4, 7-10 countdown_l1-l4, 14-17
// power_on_behavior_l1-l4 {off, on, previous}. The exact twin name now beats
// the Pass-1b alias that sent it to the relays-only AVATTO WSMD-4 def; plain
// _TZE204_/_TZE284_f5efvtbv stay on that def, as upstream.
// z2m-source: tuya.ts #OXT-4CH-RELAY-AC.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior[] = { {0, "off"}, {1, "on"}, {2, "previous"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::binary(3, "state_l3"),
        ::zhc::tuya::dp::binary(4, "state_l4"),
        ::zhc::tuya::dp::numeric(7, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_l2", 1),
        ::zhc::tuya::dp::numeric(9, "countdown_l3", 1),
        ::zhc::tuya::dp::numeric(10, "countdown_l4", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior_l1", kPowerOnBehavior, 3),
        ::zhc::tuya::dp::enum_lookup(15, "power_on_behavior_l2", kPowerOnBehavior, 3),
        ::zhc::tuya::dp::enum_lookup(16, "power_on_behavior_l3", kPowerOnBehavior, 3),
        ::zhc::tuya::dp::enum_lookup(17, "power_on_behavior_l4", kPowerOnBehavior, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kPowerOnOpts[] = { "off", "on", "previous" };
constexpr Expose kExp[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l3", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l4", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power_on_behavior_l1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPowerOnOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_l2", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPowerOnOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_l3", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPowerOnOpts, 3, ExposeCategory::Config},
    {"power_on_behavior_l4", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPowerOnOpts, 3, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE28C1000000_f5efvtbv", "_TZE204_m9dzckna" };
constexpr WhiteLabel kWL[] = { {"Tuya", "OXT-4CH-RELAY-DC"} };
}  // namespace

extern const PreparedDefinition kDef_OXT_4CH_RELAY_AC{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="OXT-4CH-RELAY-AC",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
