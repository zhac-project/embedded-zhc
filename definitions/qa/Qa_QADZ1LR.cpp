// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: QA QADZ1LR 1-channel long-range dimmer module (z2m v26.114.0, #13330).
// Brightness DP2/3/5 is 0..1000 on the wire, 0..254 published (scale0_254to0_1000).
// Upstream lists only the `_TZE28C1000000_` spelling; listed as is.
// z2m-source: qa.ts #QADZ1LR.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::qa {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior14[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSwitchType101[] = { {0,"toggle"}, {1,"momentary"} };
constexpr float kBrightnessScale = 1000.0f / 254.0f;  // z2m scale0_254to0_1000
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        { 2, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        { 5, "max_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        ::zhc::tuya::dp::numeric(6, "countdown", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehavior14, 3),
        ::zhc::tuya::dp::enum_lookup(101, "switch_type", kSwitchType101, 2),
        ::zhc::tuya::dp::numeric(102, "dimming_time", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts5[] = { "off", "on", "previous" };
constexpr const char* kOpts6[] = { "toggle", "momentary" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"max_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"countdown", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts5, 3},
    {"switch_type", ExposeType::Enum, Access::StateSet, nullptr, "Type of the external switch", kOpts6, 2, ExposeCategory::Config},
    {"dimming_time", ExposeType::Numeric, Access::StateSet, "s", "Dimming transition time", nullptr, 0, ExposeCategory::Config, 5, 15, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE28C1000000_brx4eku5" };
}  // namespace

extern const PreparedDefinition kDef_QADZ1LR{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="QADZ1LR",.vendor="QA",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::qa
