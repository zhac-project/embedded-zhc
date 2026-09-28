// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0505B RGB+CCT light (z2m TS0505B_1, zigbeeModel TS0505B).
// z2m tuyaLight({colorTemp: {range: [153, 500]}, color: true}): light with
// color_temp 153-500 and color_xy, Tuya's 0xF000 brightness reports, effect,
// do_not_disturb and color_power_on_behavior; tuyaLight turns
// power_on_behavior off. For six manufacturers z2m sets
// moveToLevelWithOnOffDisable ("moveToLevelWithOnOff seems to be broken,
// leading to the light randomly switching off for levels lower than some
// threshold"): their brightness goes out as moveToLevel — second definition.
// Not ported: applyRedFix (nudges Home Assistant's exact red 0.701/0.299),
// the TS0505B_1 transition workaround (ZHAC sends no transition), the
// colorCapabilities cache, power source "when unknown".
// z2m-source: tuya.ts #TS0505B_1 (v26.105.0).
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"   // kReportsLightRGBCCT_1ep, tuyaLight extras
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff, &::zhc::generic::kFzBrightness,
    &::zhc::tuya::kFzTuyaBrightness,
    &::zhc::generic::kFzColorTemperature, &::zhc::generic::kFzColor };
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff, &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp, &::zhc::generic::kTzColor,
    &::zhc::generic::kTzEffect, &::zhc::generic::kTzEffectColorLoop,
    &::zhc::tuya::kTzTuyaDoNotDisturb, &::zhc::tuya::kTzTuyaColorPowerOnBehavior };
const TzConverter* const kTzMoveToLevel[] = {
    &::zhc::generic::kTzOnOff, &::zhc::generic::kTzBrightnessMoveToLevel,
    &::zhc::generic::kTzColorTemp, &::zhc::generic::kTzColor,
    &::zhc::generic::kTzEffect, &::zhc::generic::kTzEffectColorLoop,
    &::zhc::tuya::kTzTuyaDoNotDisturb, &::zhc::tuya::kTzTuyaColorPowerOnBehavior };
constexpr const char* kModels[] = { "TS0505B" };
constexpr const char* kManusMoveToLevel[] = {
    "_TZB210_uoiqhjqe", "_TZB210_417ikxay", "_TZB210_qzsxaqqe",
    "_TZB210_u3ri0968", "_TZB210_rs0ufzwg", "_TZ3210_mja6r5ix" };

constexpr const char* kEffects[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect",
    "colorloop", "stop_colorloop" };
constexpr const char* kColorPowerOn[] = { "initial", "previous", "customized" };
// do_not_disturb / color_power_on_behavior: commands the light never
// echoes, so Set (Commands tab), as the Ysrsai RGB+CCT controller.
constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 153, 500, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, 8},
    {"do_not_disturb", ExposeType::Binary, Access::Set, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
    {"color_power_on_behavior", ExposeType::Enum, Access::Set, nullptr, "Power on behavior state",
     kColorPowerOn, 3, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

// z2m TS0505B_1 white labels.
constexpr WhiteLabel kWhiteLabels[] = {
    {"Mercator Ikuü", "SMD4106W-RGB-ZB"},
    {"Tuya", "A5C-21F7-01"},
    {"Mercator Ikuü", "S9E27LED9W-RGB-Z"},
    {"Hatsy", "SDL-312Z"},
    {"Aldi", "L122CB63H11A9.0W"},
    {"Lidl", "14153706L"},
    {"Zemismart", "LXZB-ZB-09A"},
    {"Feconn", "FE-GU10-5W"},
    {"Nedis", "ZBLC1E14"},
    {"Emos", "GoSmart ZQZ516R"},
    {"Emos", "GoSmart ZQZ322R"},
    {"Aldi", "L122FF63H11A5.0W"},
    {"Aldi", "L122AA63H11A6.5W"},
    {"Aldi", "C422AC11D41H140.0W"},
    {"Aldi", "C422AC14D41H140.0W"},
    {"Lidl", "14156506L"},
    {"Lidl", "HG08010"},
    {"Lidl", "HG08008"},
    {"Lidl", "14158704L"},
    {"Lidl", "14158804L"},
    {"Lidl", "HG07834A/HG09155A/HG08131A"},
    {"Lidl", "HG07834B/HG09155B/HG08131B"},
    {"Lidl", "HG07834B"},
    {"Lidl", "HG08131C"},
    {"Lidl", "HG07834C/HG09155C/HG08131C"},
    {"Lidl", "HG08383B"},
    {"Lidl", "HG08383A"},
    {"Garza Smart", "Garza-Standard-A60"},
    {"UR Lighting", "TH008L10RGBCCT"},
    {"Lidl", "HG08007"},
    {"Lidl", "399629_2110"},
    {"Nous", "P3Z"},
    {"Nous", "P4Z"},
    {"Nous", "P8Z"},
    {"Moes", "ZLD-RCW_1"},
    {"Moes", "ZB-TD5-RCW-GU10"},
    {"Moes", "ZB-TDA9-RCW-E27-MS"},
    {"Moes", "ZB-TDA14-RCW-E27-MS"},
    {"Moes", "ZB-LZD10-RCW"},
    {"Moes", "ZB-TDC6-RCW-E14"},
    {"Moes", "ZB-TDD6-RCW-4"},
    {"Moes", "ZB-TD6-RCW-GX53-MS"},
    {"MiBoxer", "E3-ZR"},
    {"MiBoxer", "SZ5"},
    {"MiBoxer", "FUT037Z+"},
    {"MiBoxer", "FUT039Z"},
    {"MiBoxer", "FUT066Z"},
    {"MiBoxer", "FUT068ZR"},
    {"MiBoxer", "FUT103ZR"},
    {"MiBoxer", "FUT105ZR"},
    {"MiBoxer", "FUT106ZR"},
    {"Tuya", "TS0505B_1_1"},
    {"MiBoxer", "FUTC11ZR"},
    {"TechToy", "_TZ3210_iw0zkcu8"},
    {"LUUMR", "10010128"},
    {"KOJIMA", "GX53-RGB-WW-CW-7W-ZGB"},
};
}  // namespace

// Reporting: onOff + currentLevel + colorTemperature + currentX + currentY
// (kReportsLightRGBCCT_1ep); every reported cluster is bound on EP1.
extern const PreparedDefinition kDefTS0505B{
    .zigbee_models=kModels,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="TS0505B",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposes,.exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWhiteLabels,.white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=::zhc::tuya::kReportsLightRGBCCT_1ep,
    .reports_count=::zhc::tuya::kReportsLightRGBCCT_1ep_count,
};

extern const PreparedDefinition kDefTS0505B_moveToLevel{
    .zigbee_models=kModels,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManusMoveToLevel,
    .manufacturer_names_count=sizeof(kManusMoveToLevel)/sizeof(kManusMoveToLevel[0]),
    .model="TS0505B",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposes,.exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTzMoveToLevel,.to_zigbee_count=sizeof(kTzMoveToLevel)/sizeof(kTzMoveToLevel[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=::zhc::tuya::kReportsLightRGBCCT_1ep,
    .reports_count=::zhc::tuya::kReportsLightRGBCCT_1ep_count,
};
}
