// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AVATTO ZOT60 smart plug, Tuya datapoints over TS011F (z2m v26.114.0, #13187).
// `_TZ3218_o1slgs0r` is the pcblab.io ZOT60-RF white label (z2m matches white-label
// fingerprints too).
// z2m-source: avatto.ts #ZOT60.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::avatto {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior27[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::numeric(9, "countdown", 1),
        ::zhc::tuya::dp::numeric(17, "energy", 1000),
        ::zhc::tuya::dp::numeric(18, "current", 1000),
        ::zhc::tuya::dp::numeric(19, "power", 10),
        ::zhc::tuya::dp::numeric(20, "voltage", 10),
        ::zhc::tuya::dp::enum_lookup(27, "power_on_behavior", kPowerOnBehavior27, 3),
        ::zhc::tuya::dp::binary(101, "backlight_mode"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts6[] = { "off", "on", "previous" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::StateSet, "kWh", "Sum of consumed energy", nullptr, 0, ExposeCategory::State, 0, 100000, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts6, 3},
    {"backlight_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "TS011F" };
constexpr const char* kN[] = { "_TZ3218_pfnjjx6a", "_TZ3218_fv20refe", "_TZ3218_o1slgs0r" };
constexpr WhiteLabel kWL[] = { {"pcblab.io", "ZOT60-RF"} };
}  // namespace

extern const PreparedDefinition kDef_ZOT60{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZOT60",.vendor="AVATTO",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::avatto
