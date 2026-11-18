// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya SP107E-Zigbee (`_TZE204_zajeikkt`) addressable LED music
// controller — triaged in R9 (N47), ported in R10 (N51).
//
// Not ported: DP5 `color` and DP113 `falling_dot_color` — HHHHSSSSVVVV hex
// string datapoints (string DP codec gap), so `state` is the only light
// feature exposed. rgb_order / led_chip values outside the tables abstain
// where z2m publishes "unknown".
//
// z2m-source: tuya.ts #SP107E-Zigbee (+ ms032zColorOrder, sp107eLedChips).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kSp107eRgbOrder[] = {
    {0, "RGB"}, {1, "RBG"}, {2, "GRB"}, {3, "GBR"}, {4, "BRG"}, {5, "BGR"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kSp107eLedChip[] = {
    {0, "WS2811"}, {1, "DMX512"}, {2, "FW1935"}, {3, "APA102"}, {4, "SC6803"},
    {5, "WS2801"}, {6, "UCS1903"}, {7, "TM1812"}, {8, "TM1814"}, {9, "SK6812"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kSp107eMode[] = {
    {1, "solid"}, {2, "effects"}, {3, "music_strip"}, {4, "music_screen"},
};

struct cfg_sp107e {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::numeric(101, "segment_count", 1),
        ::zhc::tuya::dp::numeric(102, "segment_pixels", 1),
        ::zhc::tuya::dp::enum_lookup(103, "rgb_order", kSp107eRgbOrder, 6),
        ::zhc::tuya::dp::enum_lookup(104, "mode", kSp107eMode, 4),
        ::zhc::tuya::dp::numeric(106, "brightness_percent", 1),
        ::zhc::tuya::dp::numeric(107, "speed", 1),
        ::zhc::tuya::dp::numeric(108, "effect", 1),
        ::zhc::tuya::dp::numeric(109, "music_strip_pattern", 1),
        ::zhc::tuya::dp::numeric(110, "sensitivity", 1),
        ::zhc::tuya::dp::enum_lookup(112, "led_chip", kSp107eLedChip, 10),
        ::zhc::tuya::dp::numeric(114, "music_screen_pattern", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXSp107e = ::zhc::tuya::factory::TuyaRw<cfg_sp107e>;

constexpr const char* kSp107eModeOpts[]     = { "solid", "effects", "music_strip", "music_screen" };
constexpr const char* kSp107eRgbOrderOpts[] = { "RGB", "RBG", "GRB", "GBR", "BRG", "BGR" };
constexpr const char* kSp107eLedChipOpts[]  = {
    "WS2811", "DMX512", "FW1935", "APA102", "SC6803", "WS2801", "UCS1903", "TM1812", "TM1814", "SK6812",
};

constexpr Expose kExposes_Sp107e[] = {
    { "state", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "On/off state of this light", nullptr, 0 },
    { "brightness_percent", ExposeType::Numeric, ::zhc::Access::StateSet, "%", "Brightness",
      nullptr, 0, ExposeCategory::State, 0, 100, 1 },
    { "mode", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Mode", kSp107eModeOpts, 4 },
    { "speed", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Effect speed (1 = fastest)",
      nullptr, 0, ExposeCategory::State, 1, 100, 1 },
    { "effect", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Effect (0 = auto cycle)",
      nullptr, 0, ExposeCategory::State, 0, 180, 1 },
    { "music_strip_pattern", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,
      "Music strip pattern (0 = auto cycle)", nullptr, 0, ExposeCategory::State, 0, 22, 1 },
    { "music_screen_pattern", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,
      "Music screen pattern (0 = auto cycle)", nullptr, 0, ExposeCategory::State, 0, 30, 1 },
    { "sensitivity", ExposeType::Numeric, ::zhc::Access::StateSet, "%", "Microphone sensitivity",
      nullptr, 0, ExposeCategory::State, 1, 100, 1 },
    { "segment_count", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Number of segments",
      nullptr, 0, ExposeCategory::Config, 1, 100, 1 },
    { "segment_pixels", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Pixels per segment",
      nullptr, 0, ExposeCategory::Config, 1, 300, 1 },
    { "rgb_order", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Colour channel order of the strip",
      kSp107eRgbOrderOpts, 6, ExposeCategory::Config },
    { "led_chip", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "LED driver chip used by the strip",
      kSp107eLedChipOpts, 10, ExposeCategory::Config },
};

constexpr const char* kM_Sp107e[] = { "TS0601" };
constexpr const char* kN_Sp107e[] = { "_TZE204_zajeikkt" };

}  // namespace

extern const PreparedDefinition kDef_SP107E_Zigbee{
    .zigbee_models=kM_Sp107e, .zigbee_models_count=sizeof(kM_Sp107e)/sizeof(kM_Sp107e[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_Sp107e, .manufacturer_names_count=sizeof(kN_Sp107e)/sizeof(kN_Sp107e[0]),
    .model="SP107E-Zigbee", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes_Sp107e, .exposes_count=sizeof(kExposes_Sp107e)/sizeof(kExposes_Sp107e[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXSp107e::fz_list, .from_zigbee_count=FXSp107e::fz_count,
    .to_zigbee=FXSp107e::tz_list, .to_zigbee_count=FXSp107e::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
