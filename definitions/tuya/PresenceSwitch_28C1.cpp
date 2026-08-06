// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya human-presence-sensor + smart-switch combos, 1/2/3 gang
// (z2m v26.92.0 parity, all three added v26.86.0).
//   _TZE28C1000000_jlbsptkl  1 gang
//   _TZE28C1000000_jaunkx9g  2 gang
//   _TZE28C1000000_usmqzgdm  3 gang
// One file: the three share a DP layout and differ only in gang count and in
// how many channels the turn_on/turn_off_light_for_person enums can name.
// z2m-source: tuya.ts #_TZE28C1000000_jlbsptkl / _jaunkx9g / _usmqzgdm
//
// Divisors: every numeric here is z2m `raw` (countdown/delays_time seconds,
// sensitivity 1-10) so all carry divisor 1. Gang state DPs are z2m enums
// (tuya.enum 0/1), not bools — decoded to the "OFF"/"ON" labels z2m publishes.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kOnOff28C1[]        = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRelayStatus28C1[]  = { {0, "off"}, {1, "on"}, {2, "memory"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLightMode28C1[]    = { {0, "relay"}, {1, "none"}, {2, "pos"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPresence28C1[]     = { {0, "none"}, {1, "presence"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTurnOn1G[]         = { {0, "none"}, {1, "all"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTurnOff1G[]        = { {0, "none"}, {1, "all"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTurnOn2G[]         = { {0, "none"}, {1, "all"}, {2, "on_ch1"}, {3, "on_ch2"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTurnOff2G[]        = { {0, "none"}, {1, "all"}, {2, "off_ch1"}, {3, "off_ch2"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTurnOn3G[]         = { {0, "none"}, {1, "all"}, {2, "on_ch1"}, {3, "on_ch2"}, {4, "on_ch3"},
                                                             {5, "on_1_2ch"}, {6, "on_2_3ch"}, {7, "on_1_3ch"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTurnOff3G[]        = { {0, "none"}, {1, "all"}, {2, "off_ch1"}, {3, "off_ch2"}, {4, "off_ch3"},
                                                             {5, "off_1_2ch"}, {6, "off_2_3ch"}, {7, "off_1_3ch"} };

#define ZHC_28C1_COMMON_TAIL(turn_on_tbl, turn_off_tbl)                                            \
    ::zhc::tuya::dp::enum_lookup(14, "relay_status", kRelayStatus28C1, 3),                         \
    ::zhc::tuya::dp::enum_lookup(15, "light_mode", kLightMode28C1, 3),                             \
    ::zhc::tuya::dp::enum_lookup(101, "presence_state", kPresence28C1, 2),                         \
    ::zhc::tuya::dp::numeric(102, "delays_time", 1),                                               \
    ::zhc::tuya::dp::enum_lookup(103, "turn_on_light_for_person", turn_on_tbl,                     \
                                 sizeof(turn_on_tbl) / sizeof(turn_on_tbl[0])),                    \
    ::zhc::tuya::dp::numeric(104, "sensitivity", 1),                                               \
    ::zhc::tuya::dp::enum_lookup(105, "turn_off_light_for_person", turn_off_tbl,                   \
                                 sizeof(turn_off_tbl) / sizeof(turn_off_tbl[0]))

// ---- 1 gang: _TZE28C1000000_jlbsptkl -----------------------------------
struct cfg_1g {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),          // 1 gang: z2m uses a bool DP here
        ::zhc::tuya::dp::numeric(7, "countdown", 1),
        ZHC_28C1_COMMON_TAIL(kTurnOn1G, kTurnOff1G),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX1G = ::zhc::tuya::factory::TuyaRw<cfg_1g>;

// ---- 2 gang: _TZE28C1000000_jaunkx9g -----------------------------------
struct cfg_2g {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state_l1", kOnOff28C1, 2),
        ::zhc::tuya::dp::enum_lookup(2, "state_l2", kOnOff28C1, 2),
        ::zhc::tuya::dp::numeric(7, "countdown_1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_2", 1),
        ZHC_28C1_COMMON_TAIL(kTurnOn2G, kTurnOff2G),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX2G = ::zhc::tuya::factory::TuyaRw<cfg_2g>;

// ---- 3 gang: _TZE28C1000000_usmqzgdm -----------------------------------
struct cfg_3g {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state_l1", kOnOff28C1, 2),
        ::zhc::tuya::dp::enum_lookup(2, "state_l2", kOnOff28C1, 2),
        ::zhc::tuya::dp::enum_lookup(3, "state_l3", kOnOff28C1, 2),
        ::zhc::tuya::dp::numeric(7, "countdown_1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_2", 1),
        ::zhc::tuya::dp::numeric(9, "countdown_3", 1),
        ZHC_28C1_COMMON_TAIL(kTurnOn3G, kTurnOff3G),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX3G = ::zhc::tuya::factory::TuyaRw<cfg_3g>;

#undef ZHC_28C1_COMMON_TAIL

constexpr const char* kOnOffOpts[]       = { "OFF", "ON" };
constexpr const char* kRelayStatusOpts[] = { "off", "on", "memory" };
constexpr const char* kLightModeOpts[]   = { "relay", "none", "pos" };
constexpr const char* kPresenceOpts[]    = { "none", "presence" };
constexpr const char* kTurnOn1GOpts[]    = { "none", "all" };
constexpr const char* kTurnOff1GOpts[]   = { "none", "all" };
constexpr const char* kTurnOn2GOpts[]    = { "none", "all", "on_ch1", "on_ch2" };
constexpr const char* kTurnOff2GOpts[]   = { "none", "all", "off_ch1", "off_ch2" };
constexpr const char* kTurnOn3GOpts[]    = { "none", "all", "on_ch1", "on_ch2", "on_ch3",
                                             "on_1_2ch", "on_2_3ch", "on_1_3ch" };
constexpr const char* kTurnOff3GOpts[]   = { "none", "all", "off_ch1", "off_ch2", "off_ch3",
                                             "off_1_2ch", "off_2_3ch", "off_1_3ch" };

#define ZHC_28C1_COMMON_EXPOSES(turn_on_opts, n_on, turn_off_opts, n_off)                                                       \
    { "presence_state",            ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Presence status",              kPresenceOpts,    2 },     \
    { "relay_status",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Power-on status",              kRelayStatusOpts, 3 },     \
    { "light_mode",                ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Indicator light status",       kLightModeOpts,   3 },     \
    { "delays_time",               ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Unoccupied delay time",        nullptr,          0 },     \
    { "sensitivity",               ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Trigger sensitivity",          nullptr,          0 },     \
    { "turn_on_light_for_person",  ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Turn on light when human detected",     turn_on_opts,  n_on },  \
    { "turn_off_light_for_person", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Turn off light when human not detected", turn_off_opts, n_off }

constexpr Expose kExp1G[] = {
    { "state",     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Switch state",     nullptr, 0 },
    { "countdown", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch countdown", nullptr, 0 },
    ZHC_28C1_COMMON_EXPOSES(kTurnOn1GOpts, 2, kTurnOff1GOpts, 2),
};
constexpr Expose kExp2G[] = {
    { "state_l1",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 1 state",     kOnOffOpts, 2 },
    { "state_l2",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 2 state",     kOnOffOpts, 2 },
    { "countdown_1", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 1 countdown", nullptr,    0 },
    { "countdown_2", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 2 countdown", nullptr,    0 },
    ZHC_28C1_COMMON_EXPOSES(kTurnOn2GOpts, 4, kTurnOff2GOpts, 4),
};
constexpr Expose kExp3G[] = {
    { "state_l1",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 1 state",     kOnOffOpts, 2 },
    { "state_l2",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 2 state",     kOnOffOpts, 2 },
    { "state_l3",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 3 state",     kOnOffOpts, 2 },
    { "countdown_1", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 1 countdown", nullptr,    0 },
    { "countdown_2", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 2 countdown", nullptr,    0 },
    { "countdown_3", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch 3 countdown", nullptr,    0 },
    ZHC_28C1_COMMON_EXPOSES(kTurnOn3GOpts, 8, kTurnOff3GOpts, 8),
};

#undef ZHC_28C1_COMMON_EXPOSES

constexpr const char* kM28C1[]  = { "TS0601" };
constexpr const char* kN1G[]    = { "_TZE28C1000000_jlbsptkl" };
constexpr const char* kN2G[]    = { "_TZE28C1000000_jaunkx9g" };
constexpr const char* kN3G[]    = { "_TZE28C1000000_usmqzgdm" };
}  // namespace

extern const PreparedDefinition kDef_PresenceSwitch1G{
    .zigbee_models=kM28C1,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN1G,.manufacturer_names_count=1,
    .model="_TZE28C1000000_jlbsptkl",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp1G,.exposes_count=sizeof(kExp1G)/sizeof(kExp1G[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX1G::fz_list,.from_zigbee_count=FX1G::fz_count,
    .to_zigbee=FX1G::tz_list,.to_zigbee_count=FX1G::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_PresenceSwitch2G{
    .zigbee_models=kM28C1,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN2G,.manufacturer_names_count=1,
    .model="_TZE28C1000000_jaunkx9g",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp2G,.exposes_count=sizeof(kExp2G)/sizeof(kExp2G[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX2G::fz_list,.from_zigbee_count=FX2G::fz_count,
    .to_zigbee=FX2G::tz_list,.to_zigbee_count=FX2G::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_PresenceSwitch3G{
    .zigbee_models=kM28C1,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN3G,.manufacturer_names_count=1,
    .model="_TZE28C1000000_usmqzgdm",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp3G,.exposes_count=sizeof(kExp3G)/sizeof(kExp3G[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX3G::fz_list,.from_zigbee_count=FX3G::fz_count,
    .to_zigbee=FX3G::tz_list,.to_zigbee_count=FX3G::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
