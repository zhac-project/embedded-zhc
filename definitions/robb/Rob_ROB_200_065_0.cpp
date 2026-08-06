// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: ROBB LED power supply & RGBW controller 24V 100W (z2m v26.92.0 parity).
// z2m-source: robb.ts #ROB_200-065-0 (added v26.91.0)
// Upstream colorTemp range [160, 450] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_065_0[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_ROB_200_065_0[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_ROB_200_065_0[] = { "ROB_200-065-0" };

constexpr Expose kExposes_ROB_200_065_0[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ROB_200_065_0[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

constexpr WhiteLabel kWhiteLabels_ROB_200_065_0[] = {
    {"Sunricher", "srp-zv9105-24-100cvf"},
};

}  // namespace

extern const PreparedDefinition kDef_ROB_200_065_0{
    .zigbee_models=kModels_ROB_200_065_0, .zigbee_models_count=sizeof(kModels_ROB_200_065_0)/sizeof(kModels_ROB_200_065_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-065-0", .vendor="ROBB",
    .meta=nullptr, .exposes=kExposes_ROB_200_065_0, .exposes_count=sizeof(kExposes_ROB_200_065_0)/sizeof(kExposes_ROB_200_065_0[0]),
    .white_labels=kWhiteLabels_ROB_200_065_0, .white_labels_count=sizeof(kWhiteLabels_ROB_200_065_0)/sizeof(kWhiteLabels_ROB_200_065_0[0]),
    .from_zigbee=kFz_ROB_200_065_0, .from_zigbee_count=sizeof(kFz_ROB_200_065_0)/sizeof(kFz_ROB_200_065_0[0]),
    .to_zigbee=kTz_ROB_200_065_0, .to_zigbee_count=sizeof(kTz_ROB_200_065_0)/sizeof(kTz_ROB_200_065_0[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ROB_200_065_0, .bindings_count=sizeof(kBindings_ROB_200_065_0)/sizeof(kBindings_ROB_200_065_0[0]),
};

}  // namespace zhc::devices::robb
