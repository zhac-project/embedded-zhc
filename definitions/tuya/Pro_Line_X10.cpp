// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tervix Pro Line X10 underfloor heating controller, 8 zones
// (z2m v26.115.0, #13342). Pump and boiler are read-only booleans (upstream
// publishes ON / OFF from `Number(v) === 1`); DP111 system_mode heat (1) / cool (0).
// z2m-source: tuya.ts #Pro Line X10.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode111[] = { {0,"cool"}, {1,"heat"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(33, "power"),
        ::zhc::tuya::dp::binary(101, "zone_1"),
        ::zhc::tuya::dp::binary(102, "zone_2"),
        ::zhc::tuya::dp::binary(103, "zone_3"),
        ::zhc::tuya::dp::binary(104, "zone_4"),
        ::zhc::tuya::dp::binary(105, "zone_5"),
        ::zhc::tuya::dp::binary(106, "zone_6"),
        ::zhc::tuya::dp::binary(107, "zone_7"),
        ::zhc::tuya::dp::binary(108, "zone_8"),
        ::zhc::tuya::dp::binary(109, "pump"),
        ::zhc::tuya::dp::binary(110, "boiler"),
        ::zhc::tuya::dp::enum_lookup(111, "system_mode", kSystemMode111, 2),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts1[] = { "heat", "cool" };
constexpr Expose kExp[] = {
    {"power", ExposeType::Binary, Access::StateSet, nullptr, "Main power, OFF switches all zones off", nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, "Heating or cooling mode", kOpts1, 2},
    {"zone_1", ExposeType::Binary, Access::StateSet, nullptr, "Zone 1 actuator", nullptr, 0},
    {"zone_2", ExposeType::Binary, Access::StateSet, nullptr, "Zone 2 actuator", nullptr, 0},
    {"zone_3", ExposeType::Binary, Access::StateSet, nullptr, "Zone 3 actuator", nullptr, 0},
    {"zone_4", ExposeType::Binary, Access::StateSet, nullptr, "Zone 4 actuator", nullptr, 0},
    {"zone_5", ExposeType::Binary, Access::StateSet, nullptr, "Zone 5 actuator", nullptr, 0},
    {"zone_6", ExposeType::Binary, Access::StateSet, nullptr, "Zone 6 actuator", nullptr, 0},
    {"zone_7", ExposeType::Binary, Access::StateSet, nullptr, "Zone 7 actuator", nullptr, 0},
    {"zone_8", ExposeType::Binary, Access::StateSet, nullptr, "Zone 8 actuator", nullptr, 0},
    {"pump", ExposeType::Binary, Access::State, nullptr, "Pump output, switches on 60 s after a zone demands", nullptr, 0},
    {"boiler", ExposeType::Binary, Access::State, nullptr, "Boiler dry contact, only active in heat mode", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_rfpyqax9" };
}  // namespace

extern const PreparedDefinition kDef_Pro_Line_X10{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="Pro Line X10",.vendor="Tervix",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
