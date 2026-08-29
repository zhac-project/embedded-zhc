// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Zemismart ZMZ609-2 2-gang metering wall switch with radar presence
// and an on-device screen — new upstream in z2m v26.101.0.
//
// PARTIAL PORT. 15 of 18 datapoint rows are wired. The three left out drive
// the screen and radar tuning through vendor-local converters with no generic
// counterpart:
//
//   dp104 `radarDistance`  — packed distance/sensitivity payload
//   dp105/106 `screenName` — free text shown per gang on the display; the
//                            converter encodes a length-prefixed string, and
//                            embedded-zhc has no string-writing DP type
//   dp111 `screenOffTime`  — packed timeout payload
//
// Everything the switch is actually for — both relays, per-gang countdowns and
// power-on behaviour, the shared metering counters, child lock, backlight and
// the radar enable — is covered.
//
// `power_on_behavior` appears three times upstream: a device-wide dp14 plus
// per-gang dp29/dp30. All three are kept, since the device exposes all three.
//
// z2m-source: zemismart.ts #ZMZ609-2.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::zemismart {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPob609[] = {
    {0, "off"}, {1, "on"}, {2, "previous"},
};

struct cfg_zmz609 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        ::zhc::tuya::dp::binary(2, "state_l2"),
        ::zhc::tuya::dp::numeric(7, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_l2", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPob609, 3),
        ::zhc::tuya::dp::binary(16, "radar_switch"),
        ::zhc::tuya::dp::numeric(20, "energy", 1000),
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power", 10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        ::zhc::tuya::dp::enum_lookup(29, "power_on_behavior_l1", kPob609, 3),
        ::zhc::tuya::dp::enum_lookup(30, "power_on_behavior_l2", kPob609, 3),
        ::zhc::tuya::dp::binary(101, "child_lock"),
        ::zhc::tuya::dp::numeric(102, "backlight", 1),
        // dp104 radar_distance, dp105/106 name_l1/name_l2, dp111
        // screen_off_time — vendor-local packed/string converters, deferred.
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXZmz609 = ::zhc::tuya::factory::TuyaRw<cfg_zmz609>;

constexpr const char* kPobOpts609[] = { "off", "on", "previous" };

constexpr Expose kExposes_Zmz609[] = {
    { "state_l1",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Gang 1", nullptr, 0 },
    { "state_l2",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Gang 2", nullptr, 0 },
    { "energy",              ExposeType::Numeric, ::zhc::Access::State,    "kWh", nullptr, nullptr, 0 },
    { "current",             ExposeType::Numeric, ::zhc::Access::State,    "A",   nullptr, nullptr, 0 },
    { "power",               ExposeType::Numeric, ::zhc::Access::State,    "W",   nullptr, nullptr, 0 },
    { "voltage",             ExposeType::Numeric, ::zhc::Access::State,    "V",   nullptr, nullptr, 0 },
    { "countdown_l1",        ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Gang 1 switch-off countdown", nullptr, 0, ExposeCategory::Config },
    { "countdown_l2",        ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Gang 2 switch-off countdown", nullptr, 0, ExposeCategory::Config },
    { "power_on_behavior",   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Behaviour after power loss",        kPobOpts609, 3, ExposeCategory::Config },
    { "power_on_behavior_l1",ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Gang 1 behaviour after power loss", kPobOpts609, 3, ExposeCategory::Config },
    { "power_on_behavior_l2",ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Gang 2 behaviour after power loss", kPobOpts609, 3, ExposeCategory::Config },
    { "radar_switch",        ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Radar presence detection", nullptr, 0, ExposeCategory::Config },
    { "child_lock",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Child lock",  nullptr, 0, ExposeCategory::Config },
    { "backlight",           ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Screen backlight level", nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_Zmz609[] = { "TS0601" };
constexpr const char* kN_Zmz609[] = { "_TZE284_o409r73p", "_TZE28C1000000_o409r73p" };

}  // namespace

extern const PreparedDefinition kDef_ZMZ609_2{
    .zigbee_models=kM_Zmz609, .zigbee_models_count=sizeof(kM_Zmz609)/sizeof(kM_Zmz609[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_Zmz609, .manufacturer_names_count=sizeof(kN_Zmz609)/sizeof(kN_Zmz609[0]),
    .model="ZMZ609-2", .vendor="Zemismart",
    .meta=nullptr,
    .exposes=kExposes_Zmz609, .exposes_count=sizeof(kExposes_Zmz609)/sizeof(kExposes_Zmz609[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXZmz609::fz_list, .from_zigbee_count=FXZmz609::fz_count,
    .to_zigbee=FXZmz609::tz_list, .to_zigbee_count=FXZmz609::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::zemismart
