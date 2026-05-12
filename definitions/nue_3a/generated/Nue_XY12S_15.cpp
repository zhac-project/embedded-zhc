// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nue3a XY12S-15 — hand-rewritten 2026-04-28.
// Smart RGB+CCT light controller.
// z2m-source: nue_3a.ts #XY12S-15.
//
// z2m extend: m.light({colorTemp: {range: undefined}, color: true}).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nue_3a {
namespace {
const FzConverter* const kFz_XY12S_15[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_XY12S_15[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_XY12S_15[] = { "FNB56-ZCW25FB1.9" };

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

extern const PreparedDefinition kDef_XY12S_15{
    .zigbee_models=kModels_XY12S_15, .zigbee_models_count=sizeof(kModels_XY12S_15)/sizeof(kModels_XY12S_15[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="XY12S-15", .vendor="Nue3a",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_XY12S_15, .from_zigbee_count=sizeof(kFz_XY12S_15)/sizeof(kFz_XY12S_15[0]),
    .to_zigbee=kTz_XY12S_15, .to_zigbee_count=sizeof(kTz_XY12S_15)/sizeof(kTz_XY12S_15[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nue_3a
