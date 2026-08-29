// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya MG-AU03GPOZLP-XX (`_TZE284_lq0ffndf`) 2-gang metering wall
// socket — new upstream in z2m v26.100.0.
//
// Two independently switched outlets on datapoints 1 and 2, reported as
// `state_left` / `state_right`. Note those are z2m's literal key names, NOT
// endpoint-suffixed keys: this is a datapoint device with a single ZCL
// endpoint, so there is no endpoint_map and the runtime must not rewrite them.
//
// The metering half shares one set of counters across both outlets (dp20-23).
//
// z2m-source: tuya.ts #MG-AU03GPOZLP-XX.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPobMgAu03[] = {
    {0, "off"}, {1, "on"}, {2, "previous"},
};

struct cfg_mg_au03 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_left"),
        ::zhc::tuya::dp::binary(2, "state_right"),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPobMgAu03, 3),
        ::zhc::tuya::dp::binary(16, "backlight_mode"),
        ::zhc::tuya::dp::numeric(20, "energy", 1000),
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power", 10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        // z2m maps dp101 through `lockUnlock`, which publishes LOCK/UNLOCK
        // strings; embedded-zhc models child_lock as a plain binary, matching
        // every other Tuya def in this tree.
        ::zhc::tuya::dp::binary(101, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXMgAu03 = ::zhc::tuya::factory::TuyaRw<cfg_mg_au03>;

constexpr const char* kPobOpts[] = { "off", "on", "previous" };

constexpr Expose kExposes_MgAu03[] = {
    { "state_left",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Left outlet",  nullptr, 0 },
    { "state_right",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Right outlet", nullptr, 0 },
    { "energy",           ExposeType::Numeric, ::zhc::Access::State,    "kWh", nullptr, nullptr, 0 },
    { "current",          ExposeType::Numeric, ::zhc::Access::State,    "A",   nullptr, nullptr, 0 },
    { "power",            ExposeType::Numeric, ::zhc::Access::State,    "W",   nullptr, nullptr, 0 },
    { "voltage",          ExposeType::Numeric, ::zhc::Access::State,    "V",   nullptr, nullptr, 0 },
    { "power_on_behavior",ExposeType::Enum,    ::zhc::Access::StateSet, nullptr,
      "Controls the behavior when the device is powered on after power loss",
      kPobOpts, 3, ExposeCategory::Config },
    { "backlight_mode",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,
      "Indicator backlight", nullptr, 0, ExposeCategory::Config },
    { "child_lock",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,
      "Child lock", nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_MgAu03[] = { "TS0601" };
constexpr const char* kN_MgAu03[] = { "_TZE284_lq0ffndf" };

}  // namespace

extern const PreparedDefinition kDef_MG_AU03GPOZLP{
    .zigbee_models=kM_MgAu03, .zigbee_models_count=sizeof(kM_MgAu03)/sizeof(kM_MgAu03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_MgAu03, .manufacturer_names_count=sizeof(kN_MgAu03)/sizeof(kN_MgAu03[0]),
    .model="MG-AU03GPOZLP-XX", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes_MgAu03, .exposes_count=sizeof(kExposes_MgAu03)/sizeof(kExposes_MgAu03[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXMgAu03::fz_list, .from_zigbee_count=FXMgAu03::fz_count,
    .to_zigbee=FXMgAu03::tz_list, .to_zigbee_count=FXMgAu03::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
