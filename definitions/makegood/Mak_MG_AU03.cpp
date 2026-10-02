// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: MakeGood MG-AU03 GPO with energy monitoring and RGB backlights (z2m v26.110.0, #12586).
// Not ported: DP20 `energy` — the device reports an increment since its last report and
// upstream accumulates the running total in Zigbee2MQTT state, which a stateless
// decoder cannot keep; DP107 `backlight_color` — a per-socket HSB composite written
// read-modify-write over the current state. Channels are datapoints on endpoint 1.
// z2m-source: makegood.ts #MG-AU03.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::makegood {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehaviorL129[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehaviorL230[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehaviorL331[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::binary(3, "state_l3"),
        ::zhc::tuya::dp::numeric(7, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_l2", 1),
        ::zhc::tuya::dp::numeric(9, "countdown_l3", 1),
        ::zhc::tuya::dp::binary(16, "backlight_mode"),
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power", 10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        ::zhc::tuya::dp::enum_lookup(29, "power_on_behavior_l1", kPowerOnBehaviorL129, 3),
        ::zhc::tuya::dp::enum_lookup(30, "power_on_behavior_l2", kPowerOnBehaviorL230, 3),
        ::zhc::tuya::dp::enum_lookup(31, "power_on_behavior_l3", kPowerOnBehaviorL331, 3),
        ::zhc::tuya::dp::binary(101, "child_lock"),
        ::zhc::tuya::dp::binary(136, "all_on_off"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts6[] = { "off", "on", "previous" };
constexpr const char* kOpts7[] = { "off", "on", "previous" };
constexpr const char* kOpts8[] = { "off", "on", "previous" };
constexpr Expose kExp[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l3", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power_on_behavior_l1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts6, 3, ExposeCategory::Config},
    {"power_on_behavior_l2", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts7, 3, ExposeCategory::Config},
    {"power_on_behavior_l3", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts8, 3, ExposeCategory::Config},
    {"all_on_off", ExposeType::Binary, Access::StateSet, nullptr, "Turn all channels on or off simultaneously", nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"backlight_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_4jvmbiph" };
constexpr WhiteLabel kWL[] = { {"Sparkelec", "SGPO2XTZ"} };
}  // namespace

extern const PreparedDefinition kDef_MG_AU03{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="MG-AU03",.vendor="MakeGood",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::makegood
