// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_fan_5_levels_and_light_switch (z2m v26.115.1 parity).
// Graduated from tuya/generated/Gen__TZE200_lawxy9e2.cpp and
// Gen__TZE204_lawxy9e2.cpp (identical copies).
//
// z2m v26.108.0 (#13151) exposes the device as a fan: DP3 is `speed` (it was
// `fan_speed`), labels "1".."5" over enums 0..4 as upstream's lookup, next to
// the fan's on/off `state` (DP1). DP5 `status_indication` is the light switch,
// DP11 the power-on behaviour {OFF, ON}.
// z2m-source: tuya.ts #TS0601_fan_5_levels_and_light_switch.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSpeed[] = { {0,"1"}, {1,"2"}, {2,"3"}, {3,"4"}, {4,"5"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPob[]   = { {0,"OFF"}, {1,"ON"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::enum_lookup(3, "speed", kSpeed, 5),
        ::zhc::tuya::dp::binary(5, "status_indication"),
        ::zhc::tuya::dp::enum_lookup(11, "power_on_behavior", kPob, 2),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kSpeedOpts[] = { "1", "2", "3", "4", "5" };
constexpr const char* kPobOpts[]   = { "OFF", "ON" };
constexpr Expose kExp[] = {
    {"state",             ExposeType::Binary, Access::StateSet, nullptr, "On/off state of the fan", nullptr, 0},
    {"speed",             ExposeType::Enum,   Access::StateSet, nullptr, "Fan speed", kSpeedOpts, 5},
    {"status_indication", ExposeType::Binary, Access::StateSet, nullptr, "Light switch", nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,   Access::StateSet, nullptr, "Fan On Off", kPobOpts, 2},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_lawxy9e2", "_TZE204_lawxy9e2" };
constexpr WhiteLabel kWL[] = { {"Liwokit", "Fan+Light-01"} };
}  // namespace

extern const PreparedDefinition kDef_TS0601_fan_5_levels_and_light_switch{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_fan_5_levels_and_light_switch",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
