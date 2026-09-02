// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Zemismart ZMS-206US-4 smart screen switch, 4 gang.
//
// Graduated 2026-09-02 (z2m v26.103.0 / v26.102.0 detect additions). The
// registry used to carry a genOnOff STUB listing every fingerprint of this
// model — which decoded nothing, the device speaks Tuya datapoints — next to
// identical per-manufacturer generated datapoint copies. Both shapes are
// retired for this one parent, which also wires the enum datapoints the
// generated copies dropped: indicator status, per-gang relay (power-on)
// status, the two switch colours, the delay-off colour and the radar config.
//
// NOT wired: the per-gang display names (dp105+) and `cycle_schedule`
// (dp2xx). Both are free-form byte strings and the datapoint decoder has no
// string type; z2m sends them as raw UTF-8 byte arrays.
//
// Colour labels follow upstream's CONVERTER table (warmwhite / warmyellow),
// which is what z2m actually publishes; its expose declares warm_white /
// warm_yellow, a mismatch on their side.
//
// z2m-source: zemismart.ts #ZMS-206US-4.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::zemismart {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kIndicator[] = {
    {0, "off"}, {1, "on_off_status"}, {2, "switch_position"} };
constexpr ::zhc::tuya::TuyaEnumEntry kColour[] = {
    {0, "red"}, {1, "blue"}, {2, "green"}, {3, "white"}, {4, "yellow"},
    {5, "magenta"}, {6, "cyan"}, {7, "warmwhite"}, {8, "warmyellow"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRelay[] = {
    {0, "power_off"}, {1, "power_on"}, {2, "restart_memory"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRadar[] = {
    {0, "none"}, {1, "10s"}, {2, "20s"}, {3, "30s"}, {4, "45s"}, {5, "60s"} };

constexpr const char* kIndicatorOpts[] = { "off", "on_off_status", "switch_position" };
constexpr const char* kColourOpts[] = { "red", "blue", "green", "white", "yellow",
                                        "magenta", "cyan", "warmwhite", "warmyellow" };
constexpr const char* kRelayOpts[] = { "power_off", "power_on", "restart_memory" };
constexpr const char* kRadarOpts[] = { "none", "10s", "20s", "30s", "45s", "60s" };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1,  "state_l1"),
        ::zhc::tuya::dp::binary(2,  "state_l2"),
        ::zhc::tuya::dp::binary(3,  "state_l3"),
        ::zhc::tuya::dp::binary(4,  "state_l4"),
        ::zhc::tuya::dp::numeric(7,  "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(8,  "countdown_l2", 1),
        ::zhc::tuya::dp::numeric(9,  "countdown_l3", 1),
        ::zhc::tuya::dp::numeric(10, "countdown_l4", 1),
        ::zhc::tuya::dp::binary(13, "state"),
        ::zhc::tuya::dp::numeric(14, "relay_status", 1),
        ::zhc::tuya::dp::enum_lookup(15, "indicator_status", kIndicator, 3),
        ::zhc::tuya::dp::binary(16, "backlight_mode"),
        ::zhc::tuya::dp::enum_lookup(19, "delay_off_schedule", kColour, 9),
        ::zhc::tuya::dp::numeric(24, "test_bit", 1),
        ::zhc::tuya::dp::enum_lookup(29, "relay_status_l1", kRelay, 3),
        ::zhc::tuya::dp::enum_lookup(30, "relay_status_l2", kRelay, 3),
        ::zhc::tuya::dp::enum_lookup(31, "relay_status_l3", kRelay, 3),
        ::zhc::tuya::dp::enum_lookup(32, "relay_status_l4", kRelay, 3),
        ::zhc::tuya::dp::binary(101, "child_lock"),
        ::zhc::tuya::dp::numeric(102, "backlight_brightness", 1),
        ::zhc::tuya::dp::enum_lookup(103, "switch_color_on",  kColour, 9),
        ::zhc::tuya::dp::enum_lookup(104, "switch_color_off", kColour, 9),
        ::zhc::tuya::dp::enum_lookup(111, "radar_config", kRadar, 6),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr Expose kExposes[] = {
    { "state",    ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "All gangs", nullptr, 0 },
    { "state_l1", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l3", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l4", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "countdown_l1", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Countdown for switch 1", nullptr, 0, ExposeCategory::Config, 0, 43200, 1 },
    { "countdown_l2", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Countdown for switch 2", nullptr, 0, ExposeCategory::Config, 0, 43200, 1 },
    { "countdown_l3", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Countdown for switch 3", nullptr, 0, ExposeCategory::Config, 0, 43200, 1 },
    { "countdown_l4", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Countdown for switch 4", nullptr, 0, ExposeCategory::Config, 0, 43200, 1 },
    { "relay_status_l1", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Relay status for switch 1", kRelayOpts, 3, ExposeCategory::Config },
    { "relay_status_l2", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Relay status for switch 2", kRelayOpts, 3, ExposeCategory::Config },
    { "relay_status_l3", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Relay status for switch 3", kRelayOpts, 3, ExposeCategory::Config },
    { "relay_status_l4", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Relay status for switch 4", kRelayOpts, 3, ExposeCategory::Config },
    { "relay_status",    ExposeType::Numeric, ::zhc::Access::State, nullptr, "Raw device-wide relay status word (dp14)", nullptr, 0, ExposeCategory::Diagnostic },
    { "backlight_mode",  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Mode of the backlight", nullptr, 0, ExposeCategory::Config },
    { "backlight_brightness", ExposeType::Numeric, ::zhc::Access::StateSet, "%", "Brightness of the light", nullptr, 0, ExposeCategory::Config, 0, 100, 1 },
    { "child_lock",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "indicator_status", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Indicator light status", kIndicatorOpts, 3, ExposeCategory::Config },
    { "switch_color_on",  ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Switch light colour when on",  kColourOpts, 9, ExposeCategory::Config },
    { "switch_color_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Switch light colour when off", kColourOpts, 9, ExposeCategory::Config },
    { "delay_off_schedule", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Switch light colour while delayed", kColourOpts, 9, ExposeCategory::Config },
    { "radar_config",    ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Radar config", kRadarOpts, 6, ExposeCategory::Config },
    { "test_bit",        ExposeType::Numeric, ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE204_wwaeqnrf", "_TZE284_wwaeqnrf", "_TZE204_y4jqpry8", "_TZE284_y4jqpry8",
    "_TZE204_xibaabmu", "_TZE284_xibaabmu", "_TZE28C1000000_xibaabmu", "_TZE204_08qc13ct",
    "_TZE28C1000000_y4jqpry8" };

}  // namespace

extern const PreparedDefinition kDef_ZMS_206US_4{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="ZMS-206US-4", .vendor="Zemismart",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::zemismart
