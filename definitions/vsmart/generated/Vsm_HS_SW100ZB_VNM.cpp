// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: VSmart HS-SW100ZB-VNM — uses shared vsmart converters.
// Wall switch 1 gang (LED indicator + vibration + per-period intensity).
// z2m-source: vsmart.ts #HS-SW100ZB-VNM.
//
// Coverage shape (z2m extends → ZHC converters):
//   - m.onOff                         → kFzOnOff / kTzOnOff (genOnOff 0x0006)
//   - mLocal.customCluster            → registered implicitly via the
//                                       vsmart Tz converters (cluster id
//                                       0x0000, manu code 0x1379, fc=0x15,
//                                       cluster-specific raw cmds).
//   - mLocal.ledColorControl          → kTzLedIndicatorColorOn/Off (cmd 0xF3)
//   - mLocal.vibrationIntensityControl→ kTzVibrationIntensity     (cmd 0xF0)
//   - mLocal.ledIntensityControl      → kTzMorningLedIntensity,
//                                       kTzEveningLedIntensity,
//                                       kTzNightLedIntensity      (cmd 0xF2)
//
// Composite-only extends (`time_periods`, `led_brightness_levels`) are
// PARTIAL — see docs/VSMART_PARITY.md. Their multi-field scalar inputs
// aren't representable in the runtime's flat string-keyed Value model
// today, so the converters are intentionally not wired.
#include "definitions/_generic/_shared.hpp"
#include "definitions/vsmart/_shared.hpp"

namespace zhc::devices::vsmart {
namespace {
const FzConverter* const kFz_HS_SW100ZB_VNM[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_HS_SW100ZB_VNM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::vsmart::kTzLedIndicatorColorOn,
    &::zhc::vsmart::kTzLedIndicatorColorOff,
    &::zhc::vsmart::kTzVibrationIntensity,
    &::zhc::vsmart::kTzMorningLedIntensity,
    &::zhc::vsmart::kTzEveningLedIntensity,
    &::zhc::vsmart::kTzNightLedIntensity,
};
constexpr const char* kModels_HS_SW100ZB_VNM[] = {
    "HS-SWL100ZB-VNM", "HS-SWN100ZB-VNM", "HS-SWB100ZB-VNM", "HS-SRW100ZB-VNM",
};

constexpr Expose kAutoExposes[] = {
    {"state",                   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_indicator_color_on",  ExposeType::String,  Access::Set,      nullptr, "LED indicator colour when ON (#RRGGBB).", nullptr, 0,
        ExposeCategory::Config},
    {"led_indicator_color_off", ExposeType::String,  Access::Set,      nullptr, "LED indicator colour when OFF (#RRGGBB).", nullptr, 0,
        ExposeCategory::Config},
    {"vibration_intensity",     ExposeType::Numeric, Access::Set,      "%",     "Button vibration intensity (0-100%).", nullptr, 0,
        ExposeCategory::Config},
    {"morning_led_intensity",   ExposeType::Numeric, Access::Set,      "%",     "LED intensity for morning period (0-100%).", nullptr, 0,
        ExposeCategory::Config},
    {"evening_led_intensity",   ExposeType::Numeric, Access::Set,      "%",     "LED intensity for evening period (0-100%).", nullptr, 0,
        ExposeCategory::Config},
    {"night_led_intensity",     ExposeType::Numeric, Access::Set,      "%",     "LED intensity for night period (0-100%).", nullptr, 0,
        ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_HS_SW100ZB_VNM{
    .zigbee_models=kModels_HS_SW100ZB_VNM, .zigbee_models_count=sizeof(kModels_HS_SW100ZB_VNM)/sizeof(kModels_HS_SW100ZB_VNM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS-SW100ZB-VNM", .vendor="VSmart",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS_SW100ZB_VNM, .from_zigbee_count=sizeof(kFz_HS_SW100ZB_VNM)/sizeof(kFz_HS_SW100ZB_VNM[0]),
    .to_zigbee=kTz_HS_SW100ZB_VNM, .to_zigbee_count=sizeof(kTz_HS_SW100ZB_VNM)/sizeof(kTz_HS_SW100ZB_VNM[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vsmart
