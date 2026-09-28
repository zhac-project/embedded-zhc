// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0601_soil soil sensor, `_TZE284_2nhqasjh` — the one ID of
// z2m's nine with no definition here (the other eight are generated, one per
// ID, with the same datapoints). Without it the sensor fell to a Lincukoo
// radar stub and showed no soil data.
// z2m-source: tuya.ts #TS0601_soil (v26.105.0), tuyaBase({dp: true}):
//   3 soil_moisture raw, 5 temperature raw, 9 temperature_unit,
//   14 battery_state, 15 battery raw.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kUnit[]    = { {0, "celsius"}, {1, "fahrenheit"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBattery[] = { {0, "low"}, {1, "medium"}, {2, "high"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(3, "soil_moisture"),
        ::zhc::tuya::dp::numeric(5, "temperature"),
        ::zhc::tuya::dp::enum_lookup(9, "temperature_unit", kUnit, 2),
        ::zhc::tuya::dp::enum_lookup(14, "battery_state", kBattery, 3),
        ::zhc::tuya::dp::numeric(15, "battery"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kUnitValues[]    = { "celsius", "fahrenheit" };
constexpr const char* kBatteryValues[] = { "low", "medium", "high" };
constexpr Expose kExposes[] = {
    {"temperature",      ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0},
    {"soil_moisture",    ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"temperature_unit", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kUnitValues, 2},
    {"battery",          ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0,
     ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_state",    ExposeType::Enum,    Access::State,    nullptr, nullptr, kBatteryValues, 3,
     ExposeCategory::Diagnostic},
};
constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE284_2nhqasjh" };
}  // namespace

extern const PreparedDefinition kDefTS0601_soil{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="TS0601_soil", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
};

}  // namespace zhc::devices::tuya
