// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ls 756200027 — hand-ported (LS sweep, 2026-04-29c).
// Mec Driver module 1-channel Zigbee (12V)
// z2m-source: ls.ts #756200027.
//
// z2m: extend: [m.light({colorTemp: {range: [153, 500]}})]
//      whiteLabel: 756200028 (24V variant on the same firmware).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ls {
namespace {
const FzConverter* const kFz_D756200027[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D756200027[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D756200027[] = { "Mec Driver module" };

constexpr Expose kExposes_D756200027[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", "153..500", nullptr, 0},
};

constexpr BindingSpec kBind_D756200027[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl (color temp)
};

constexpr WhiteLabel kWhiteLabels_D756200027[] = {
    {"L&S Lighting", "756200028"},   // 24V Mec Driver 1ch variant
};

}  // namespace

extern const PreparedDefinition kDef_D756200027{
    .zigbee_models=kModels_D756200027, .zigbee_models_count=sizeof(kModels_D756200027)/sizeof(kModels_D756200027[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="756200027", .vendor="Ls",
    .meta=nullptr, .exposes=kExposes_D756200027, .exposes_count=sizeof(kExposes_D756200027)/sizeof(kExposes_D756200027[0]),
    .white_labels=kWhiteLabels_D756200027, .white_labels_count=sizeof(kWhiteLabels_D756200027)/sizeof(kWhiteLabels_D756200027[0]),
    .from_zigbee=kFz_D756200027, .from_zigbee_count=sizeof(kFz_D756200027)/sizeof(kFz_D756200027[0]),
    .to_zigbee=kTz_D756200027, .to_zigbee_count=sizeof(kTz_D756200027)/sizeof(kTz_D756200027[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_D756200027, .bindings_count=sizeof(kBind_D756200027)/sizeof(kBind_D756200027[0]),
};

}  // namespace zhc::devices::ls
