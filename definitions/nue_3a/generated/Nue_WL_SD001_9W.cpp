// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nue3a WL-SD001-9W — hand-rewritten 2026-04-28.
// 9W RGB+CCT LED downlight.
// z2m-source: nue_3a.ts #WL-SD001-9W.
//
// z2m extend: m.light({colorTemp: {range: [153, 370]}, color: true}).
// The mired range narrows the colour-temperature limits but the framework
// uses the same kTzColorTemp converter regardless.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nue_3a {
namespace {
const FzConverter* const kFz_WL_SD001_9W[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_WL_SD001_9W[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_WL_SD001_9W[] = { "LXN60-LS27-Z30", "FEB56-ZCW2CLX1.0" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_WL_SD001_9W{
    .zigbee_models=kModels_WL_SD001_9W, .zigbee_models_count=sizeof(kModels_WL_SD001_9W)/sizeof(kModels_WL_SD001_9W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WL-SD001-9W", .vendor="Nue3a",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WL_SD001_9W, .from_zigbee_count=sizeof(kFz_WL_SD001_9W)/sizeof(kFz_WL_SD001_9W[0]),
    .to_zigbee=kTz_WL_SD001_9W, .to_zigbee_count=sizeof(kTz_WL_SD001_9W)/sizeof(kTz_WL_SD001_9W[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nue_3a
