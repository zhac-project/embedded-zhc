// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya MS032Z (`_TZE284_rovbuqdo`) addressable LED stair-light
// controller — new upstream in z2m v26.100.0.
//
// Drives an addressable strip as a staircase effect: motion at either end
// (dp128/dp129) runs the configured animation up or down the strip.
//
// dp127 brightness: z2m scales 0..254 to 0..100. embedded-zhc has no scaling
// DP helper, so the raw 0..254 value is published and the expose declares that
// range — a UI slider reads the bounds off the expose, so it presents
// correctly; only the unit differs from z2m's percentage.
//
// z2m-source: tuya.ts #MS032Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kRgbOrder[] = {
    {0, "RGB"}, {1, "RBG"}, {2, "GRB"}, {3, "GBR"}, {4, "BRG"}, {5, "BGR"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kLedChip[] = {
    {0, "WS2811"}, {1, "DMX512"}, {2, "FW1935"},
    {3, "type_3"}, {4, "type_4"},  {5, "type_5"},  {6, "type_6"},  {7, "type_7"},
    {8, "type_8"}, {9, "type_9"},  {10, "type_10"},{11, "type_11"},
    {12, "type_12"},{13, "type_13"},{14, "type_14"},{15, "type_15"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kEffect[] = {
    {1, "running_water"}, {2, "septum"}, {3, "full_bright"},
    {4, "following"},     {5, "colorful"}, {6, "smear"},
};

struct cfg_ms032z {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::numeric(102, "strip_led_count", 1),
        ::zhc::tuya::dp::enum_lookup(103, "rgb_order", kRgbOrder, 6),
        ::zhc::tuya::dp::enum_lookup(112, "led_chip", kLedChip, 16),
        ::zhc::tuya::dp::numeric(124, "step_count", 1),
        ::zhc::tuya::dp::enum_lookup(125, "effect", kEffect, 6),
        ::zhc::tuya::dp::numeric(126, "speed", 1),
        ::zhc::tuya::dp::numeric(127, "brightness", 1),
        ::zhc::tuya::dp::binary(128, "motion_up"),
        ::zhc::tuya::dp::binary(129, "motion_down"),
        ::zhc::tuya::dp::numeric(134, "light_off_delay", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXMs032z = ::zhc::tuya::factory::TuyaRw<cfg_ms032z>;

constexpr const char* kRgbOrderOpts[] = { "RGB", "RBG", "GRB", "GBR", "BRG", "BGR" };
constexpr const char* kLedChipOpts[]  = {
    "WS2811", "DMX512", "FW1935", "type_3", "type_4", "type_5", "type_6", "type_7",
    "type_8", "type_9", "type_10", "type_11", "type_12", "type_13", "type_14", "type_15",
};
constexpr const char* kEffectOpts[]   = {
    "running_water", "septum", "full_bright", "following", "colorful", "smear",
};

constexpr Expose kExposes_Ms032z[] = {
    { "state",           ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",      ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Strip brightness",
      nullptr, 0, ExposeCategory::State, 0, 254, 1 },
    { "motion_up",       ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Motion at the bottom of the stairs",  nullptr, 0 },
    { "motion_down",     ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Motion at the top of the stairs",     nullptr, 0 },
    { "effect",          ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Animation", kEffectOpts, 6, ExposeCategory::Config },
    { "speed",           ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Animation speed", nullptr, 0, ExposeCategory::Config },
    { "step_count",      ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Number of stair steps", nullptr, 0, ExposeCategory::Config },
    { "strip_led_count", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "LEDs on the strip", nullptr, 0, ExposeCategory::Config },
    { "rgb_order",       ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Colour channel order", kRgbOrderOpts, 6, ExposeCategory::Config },
    { "led_chip",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "LED driver chip", kLedChipOpts, 16, ExposeCategory::Config },
    { "light_off_delay", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Delay before the strip switches off", nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_Ms032z[] = { "TS0601" };
constexpr const char* kN_Ms032z[] = { "_TZE284_rovbuqdo" };

}  // namespace

extern const PreparedDefinition kDef_MS032Z{
    .zigbee_models=kM_Ms032z, .zigbee_models_count=sizeof(kM_Ms032z)/sizeof(kM_Ms032z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_Ms032z, .manufacturer_names_count=sizeof(kN_Ms032z)/sizeof(kN_Ms032z[0]),
    .model="MS032Z", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes_Ms032z, .exposes_count=sizeof(kExposes_Ms032z)/sizeof(kExposes_Ms032z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXMs032z::fz_list, .from_zigbee_count=FXMs032z::fz_count,
    .to_zigbee=FXMs032z::tz_list, .to_zigbee_count=FXMs032z::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
