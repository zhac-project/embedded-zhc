// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: VSmart HS-SW200ZB-VNM — uses shared vsmart converters.
// Wall switch 2 gang. PARTIAL coverage — see docs/VSMART_PARITY.md.
// z2m-source: vsmart.ts #HS-SW200ZB-VNM.
//
// Multi-gang state caveat: ZHC's Expose schema has no per-endpoint
// scoping, so the SPA-facing exposes table sees a single `state`. The
// 1/2 deviceEndpoints split is preserved on the wire (kFzOnOff still
// works on each endpoint via the platform's endpoint routing), but
// per-endpoint state names (z2m's `state_1`/`state_2`) collapse.
#include "definitions/_generic/_shared.hpp"
#include "definitions/vsmart/_shared.hpp"

namespace zhc::devices::vsmart {
namespace {
const FzConverter* const kFz_HS_SW200ZB_VNM[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_HS_SW200ZB_VNM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::vsmart::kTzLedIndicatorColorOn,
    &::zhc::vsmart::kTzLedIndicatorColorOff,
    &::zhc::vsmart::kTzVibrationIntensity,
    &::zhc::vsmart::kTzMorningLedIntensity,
    &::zhc::vsmart::kTzEveningLedIntensity,
    &::zhc::vsmart::kTzNightLedIntensity,
};
constexpr const char* kModels_HS_SW200ZB_VNM[] = {
    "HS-SWL200ZB-VNM", "HS-SWN200ZB-VNM", "HS-SWB200ZB-VNM", "HS-SRW200ZB-VNM",
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
    {2, 0x0006},
};

constexpr ::zhc::EndpointLabel kEndpoints_HS_SW200ZB_VNM[] = { {"1", 1}, {"2", 2} };

}  // namespace

extern const PreparedDefinition kDef_HS_SW200ZB_VNM{
    .zigbee_models=kModels_HS_SW200ZB_VNM, .zigbee_models_count=sizeof(kModels_HS_SW200ZB_VNM)/sizeof(kModels_HS_SW200ZB_VNM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS-SW200ZB-VNM", .vendor="VSmart",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS_SW200ZB_VNM, .from_zigbee_count=sizeof(kFz_HS_SW200ZB_VNM)/sizeof(kFz_HS_SW200ZB_VNM[0]),
    .to_zigbee=kTz_HS_SW200ZB_VNM, .to_zigbee_count=sizeof(kTz_HS_SW200ZB_VNM)/sizeof(kTz_HS_SW200ZB_VNM[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_HS_SW200ZB_VNM,
    .endpoint_map_count = sizeof(kEndpoints_HS_SW200ZB_VNM)/sizeof(kEndpoints_HS_SW200ZB_VNM[0]),
};

}  // namespace zhc::devices::vsmart
