// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ShenzhenHoma HOMA1001_RGBW — hand-rewritten ColorLight bundle.
// Smart LED driver RGBW.
// z2m extend: m.deviceEndpoints({white: 10, rgb: 11}),
//             m.light({endpointNames: ["white", "rgb"], color: true})
//   → on/off + brightness per endpoint + color (xy) on the rgb endpoint.
// The auto-generator collapsed m.light({color:true}) to on/off + brightness
// only, dropping the 0x0300 color axis — this file restores it. The xy color
// report lands on endpoint 11 (label "rgb"), so dispatch suffixes the keys to
// color_x_rgb / color_y_rgb via the endpoint_map.
// z2m-source: shenzhen_homa.ts #HOMA1001_RGBW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shenzhen_homa {
namespace {
const FzConverter* const kFz_HOMA1001_RGBW[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_HOMA1001_RGBW[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_HOMA1001_RGBW[] = { "HOMA1001" };

constexpr ::zhc::EndpointLabel kEndpoints_HOMA1001_RGBW[] = { {"white", 10}, {"rgb", 11} };

}  // namespace


// Hand-curated ColorLight surface. Exposes stay bare (color_x/color_y); the
// runtime suffixes them to color_x_rgb / color_y_rgb from the endpoint_map.
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {10, 0x0006},  // white: genOnOff
    {10, 0x0008},  // white: genLevelCtrl
    {11, 0x0006},  // rgb:   genOnOff
    {11, 0x0008},  // rgb:   genLevelCtrl
    {11, 0x0300},  // rgb:   lightingColorCtrl (color xy)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HOMA1001_RGBW{
    .zigbee_models=kModels_HOMA1001_RGBW, .zigbee_models_count=sizeof(kModels_HOMA1001_RGBW)/sizeof(kModels_HOMA1001_RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HOMA1001_RGBW", .vendor="ShenzhenHoma",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HOMA1001_RGBW, .from_zigbee_count=sizeof(kFz_HOMA1001_RGBW)/sizeof(kFz_HOMA1001_RGBW[0]),
    .to_zigbee=kTz_HOMA1001_RGBW, .to_zigbee_count=sizeof(kTz_HOMA1001_RGBW)/sizeof(kTz_HOMA1001_RGBW[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_HOMA1001_RGBW,
    .endpoint_map_count = sizeof(kEndpoints_HOMA1001_RGBW)/sizeof(kEndpoints_HOMA1001_RGBW[0]),
};

}  // namespace zhc::devices::shenzhen_homa
