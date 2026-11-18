// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital TS0601_ND_4Gang 4-gang touch switch, TS0601
// _TZE204_tayx2ud6 (z2m v26.133.0 window). tuyaBase({timeStart: "2000"}).
// DPs 1-4 state_l1-l4, 14 power_on_behavior {off, on, previous},
// 15 backlight_mode {off, relay, position}, 16 backlight. Every endpoint
// maps to EP1 upstream, so the DP keys carry the suffix (as Nov_ZTS_8B.cpp).
// z2m-source: nova_digital.ts #TS0601_ND_4Gang.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nova_digital {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior[] = { {0, "off"}, {1, "on"}, {2, "previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBacklightMode[]   = { {0, "off"}, {1, "relay"}, {2, "position"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::binary(3, "state_l3"),
        ::zhc::tuya::dp::binary(4, "state_l4"),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehavior, 3),
        ::zhc::tuya::dp::enum_lookup(15, "backlight_mode", kBacklightMode, 3),
        ::zhc::tuya::dp::binary(16, "backlight"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kPowerOnOpts[]       = { "off", "on", "previous" };
constexpr const char* kBacklightModeOpts[] = { "off", "relay", "position" };
constexpr Expose kExp[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Power-on behavior", kPowerOnOpts, 3},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, "Backlight mode", kBacklightModeOpts, 3},
    {"backlight", ExposeType::Binary, Access::StateSet, nullptr, "Backlight on/off", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_tayx2ud6" };
}  // namespace

extern const PreparedDefinition kDef_TS0601_ND_4Gang{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_ND_4Gang",.vendor="Nova Digital",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=2 };

}  // namespace zhc::devices::nova_digital
