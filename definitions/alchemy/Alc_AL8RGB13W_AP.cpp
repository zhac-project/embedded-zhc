// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Alchemy AL8RGB13W-AP — hand-rewritten RGB+CCT light bundle.
// Downlight RGBW.
// z2m extend: m.light({colorTemp: {range: [153, 370]}, color: true})
//   → on/off + level + colorTemp (mireds) + full xy/hs colour axis.
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping both the colorTemp axis and the lightingColorCtrl (0x0300)
// colour channel — this file restores them.
// z2m-source: alchemy.ts #AL8RGB13W-AP.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::alchemy {
namespace {
const FzConverter* const kFz_AL8RGB13W_AP[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_AL8RGB13W_AP[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_AL8RGB13W_AP[] = { "AL8RGB13W-AP" };

}  // namespace


// Hand-curated RGB+CCT light surface. colorTemp range [153, 370] mireds;
// colour reported via lightingColorCtrl hue/sat (0x0000/0x0001) and
// CIE 1931 currentX/currentY (0x0003/0x0004).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_xy",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (color + colorTemp)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_AL8RGB13W_AP{
    .zigbee_models=kModels_AL8RGB13W_AP, .zigbee_models_count=sizeof(kModels_AL8RGB13W_AP)/sizeof(kModels_AL8RGB13W_AP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AL8RGB13W-AP", .vendor="Alchemy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AL8RGB13W_AP, .from_zigbee_count=sizeof(kFz_AL8RGB13W_AP)/sizeof(kFz_AL8RGB13W_AP[0]),
    .to_zigbee=kTz_AL8RGB13W_AP, .to_zigbee_count=sizeof(kTz_AL8RGB13W_AP)/sizeof(kTz_AL8RGB13W_AP[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::alchemy
