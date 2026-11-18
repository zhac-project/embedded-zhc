// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital SA-WK 4X4 6-channel switch, TS0601 _TZE204_bb5xlia1
// (z2m v26.133.0 window). DPs 1-6 state_l1-l6, 101 state_master, 7-12
// countdown_l1-l6 (raw s), 14 relay_status {off, on, memory}, 15 light_mode
// {none, relay, pos}. Every endpoint maps to EP1 upstream, so the DP keys
// carry the suffix directly (as Nov_ZTS_8B.cpp).
// z2m-source: nova_digital.ts #SA-WK 4X4.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nova_digital {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kRelayStatus[] = { {0, "off"}, {1, "on"}, {2, "memory"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLightMode[]   = { {0, "none"}, {1, "relay"}, {2, "pos"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::binary(3, "state_l3"),
        ::zhc::tuya::dp::binary(4, "state_l4"),
        ::zhc::tuya::dp::binary(5, "state_l5"),
        ::zhc::tuya::dp::binary(6, "state_l6"),
        ::zhc::tuya::dp::binary(101, "state_master"),
        ::zhc::tuya::dp::enum_lookup(14, "relay_status", kRelayStatus, 3),
        ::zhc::tuya::dp::numeric(7, "countdown_l1"),
        ::zhc::tuya::dp::numeric(8, "countdown_l2"),
        ::zhc::tuya::dp::numeric(9, "countdown_l3"),
        ::zhc::tuya::dp::numeric(10, "countdown_l4"),
        ::zhc::tuya::dp::numeric(11, "countdown_l5"),
        ::zhc::tuya::dp::numeric(12, "countdown_l6"),
        ::zhc::tuya::dp::enum_lookup(15, "light_mode", kLightMode, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kRelayStatusOpts[] = { "off", "on", "memory" };
constexpr const char* kLightModeOpts[]   = { "none", "relay", "pos" };
constexpr Expose kExp[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l6", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_master", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"relay_status", ExposeType::Enum, Access::StateSet, nullptr, "Relay state after power restoration", kRelayStatusOpts, 3},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l3", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l4", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l5", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l6", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"light_mode", ExposeType::Enum, Access::StateSet, nullptr, "LED indicator operating mode", kLightModeOpts, 3},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_bb5xlia1" };
}  // namespace

extern const PreparedDefinition kDef_SA_WK_4X4{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="SA-WK 4X4",.vendor="Nova Digital",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::nova_digital
