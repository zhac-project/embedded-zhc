// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya 5-gang touch switch _TZE284_0kihjsys (z2m v26.92.0 parity,
// added v26.80.0).
// z2m-source: tuya.ts #_TZE284_0kihjsys  (fingerprint TS0601 / _TZE284_0kihjsys)
//
// Divisors: every numeric is z2m `valueConverter.countdown`, which is
// `valueConverterBasic.raw()` — plain seconds, divisor 1. (The DP-map emitter
// flags `countdown` as unhandled; it resolves to raw in lib/tuya.ts:1522.)
// Gang states are tuya enums (0/1), decoded to the "OFF"/"ON" labels z2m
// publishes; per-gang restart_status is powerOnBehaviorEnum {off,on,previous}.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kOnOff[]         = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kIndicatorMode[] = { {0, "off"}, {1, "on_off_status"}, {2, "switch_position"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehav[]  = { {0, "off"}, {1, "on"}, {2, "previous"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state_l1", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(2, "state_l2", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(3, "state_l3", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(4, "state_l4", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(5, "state_l5", kOnOff, 2),
        ::zhc::tuya::dp::numeric(7,  "timer_1", 1),
        ::zhc::tuya::dp::numeric(8,  "timer_2", 1),
        ::zhc::tuya::dp::numeric(9,  "timer_3", 1),
        ::zhc::tuya::dp::numeric(10, "timer_4", 1),
        ::zhc::tuya::dp::numeric(11, "timer_5", 1),
        ::zhc::tuya::dp::binary(14, "restart_status"),
        ::zhc::tuya::dp::enum_lookup(15, "indicator_mode", kIndicatorMode, 3),
        ::zhc::tuya::dp::enum_lookup(29, "restart_status_1", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(30, "restart_status_2", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(31, "restart_status_3", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(32, "restart_status_4", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(33, "restart_status_5", kPowerOnBehav, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kOnOffOpts[]         = { "OFF", "ON" };
constexpr const char* kIndicatorModeOpts[] = { "off", "on_off_status", "switch_position" };
constexpr const char* kPowerOnBehavOpts[]  = { "off", "on", "previous" };

constexpr Expose kExp[] = {
    { "state_l1",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 1 state",           kOnOffOpts,         2 },
    { "state_l2",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 2 state",           kOnOffOpts,         2 },
    { "state_l3",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 3 state",           kOnOffOpts,         2 },
    { "state_l4",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 4 state",           kOnOffOpts,         2 },
    { "state_l5",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 5 state",           kOnOffOpts,         2 },
    { "timer_1",          ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 1 countdown",       nullptr,            0 },
    { "timer_2",          ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 2 countdown",       nullptr,            0 },
    { "timer_3",          ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 3 countdown",       nullptr,            0 },
    { "timer_4",          ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 4 countdown",       nullptr,            0 },
    { "timer_5",          ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 5 countdown",       nullptr,            0 },
    { "restart_status",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Global power-on status",   nullptr,            0 },
    { "indicator_mode",   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Indicator light mode",     kIndicatorModeOpts, 3 },
    { "restart_status_1", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 1 power-on status", kPowerOnBehavOpts,  3 },
    { "restart_status_2", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 2 power-on status", kPowerOnBehavOpts,  3 },
    { "restart_status_3", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 3 power-on status", kPowerOnBehavOpts,  3 },
    { "restart_status_4", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 4 power-on status", kPowerOnBehavOpts,  3 },
    { "restart_status_5", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 5 power-on status", kPowerOnBehavOpts,  3 },
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_0kihjsys" };
}  // namespace

extern const PreparedDefinition kDef_TZE284_0kihjsys{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,
    .model="_TZE284_0kihjsys",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
