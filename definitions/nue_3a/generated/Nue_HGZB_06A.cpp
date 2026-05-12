// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nue3a HGZB-06A — hand-rewritten 2026-04-28.
// Smart 7W E27 RGB+CCT light bulb.
// z2m-source: nue_3a.ts #HGZB-06A.
//
// z2m extend: m.light({colorTemp: {range: undefined}, color: true}).
// Adds colour (xy/hue/sat) + colour-temperature on top of on/off + level.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nue_3a {
namespace {
const FzConverter* const kFz_HGZB_06A[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_HGZB_06A[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_HGZB_06A[] = { "FNB56-ZCW25FB1.6", "FNB56-ZCW25FB2.1" };

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
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl
};

extern const PreparedDefinition kDef_HGZB_06A{
    .zigbee_models=kModels_HGZB_06A, .zigbee_models_count=sizeof(kModels_HGZB_06A)/sizeof(kModels_HGZB_06A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HGZB-06A", .vendor="Nue3a",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HGZB_06A, .from_zigbee_count=sizeof(kFz_HGZB_06A)/sizeof(kFz_HGZB_06A[0]),
    .to_zigbee=kTz_HGZB_06A, .to_zigbee_count=sizeof(kTz_HGZB_06A)/sizeof(kTz_HGZB_06A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nue_3a
