// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital ZTS-8B 8-gang switch, TS0601 _TZE284_kow4ok3t (z2m
// v26.125.0 window). DPs 1-6 + 101/102 state_l1-l8, 14 power_on_behavior
// {off, on, previous}, 16 backlight_switch. Sibling of ZTS-8W-B
// (tuya/generated/Gen__TZE204_nvxorhcj.cpp).
// z2m-source: nova_digital.ts #ZTS-8B.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nova_digital {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior[] = { {0, "off"}, {1, "on"}, {2, "previous"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::binary(3, "state_l3"),
        ::zhc::tuya::dp::binary(4, "state_l4"),
        ::zhc::tuya::dp::binary(5, "state_l5"),
        ::zhc::tuya::dp::binary(6, "state_l6"),
        ::zhc::tuya::dp::binary(101, "state_l7"),
        ::zhc::tuya::dp::binary(102, "state_l8"),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehavior, 3),
        ::zhc::tuya::dp::binary(16, "backlight_switch"),
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
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l6", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l7", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l8", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Power-on behavior", kPowerOnOpts, 3, ExposeCategory::Config},
    {"backlight_switch", ExposeType::Binary, Access::StateSet, nullptr, "Backlight switch", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_kow4ok3t" };
}  // namespace

extern const PreparedDefinition kDef_ZTS_8B{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZTS-8B",.vendor="Nova Digital",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::nova_digital
