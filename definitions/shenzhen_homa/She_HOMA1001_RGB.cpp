// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ShenzhenHoma HOMA1001_RGB — hand-rewritten ColorLight bundle.
// Smart LED driver RGB.
// z2m extend: m.light({color: true})
//   → on/off + brightness + color (xy). The light lives on endpoint 11
//     (deviceID 528, inputClusters include 768/0x0300).
// The auto-generator collapsed m.light({color:true}) to on/off + brightness
// only, dropping the 0x0300 color axis — this file restores it. Single light
// (no endpointNames) so keys stay bare (color_x / color_y, no suffix).
// z2m-source: shenzhen_homa.ts #HOMA1001_RGB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shenzhen_homa {
namespace {
const FzConverter* const kFz_HOMA1001_RGB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_HOMA1001_RGB[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_HOMA1001_RGB[] = { "HOMA1001" };

}  // namespace


// Hand-curated ColorLight surface (single endpoint 11 → bare keys).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {11, 0x0006},  // genOnOff
    {11, 0x0008},  // genLevelCtrl
    {11, 0x0300},  // lightingColorCtrl (color xy)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HOMA1001_RGB{
    .zigbee_models=kModels_HOMA1001_RGB, .zigbee_models_count=sizeof(kModels_HOMA1001_RGB)/sizeof(kModels_HOMA1001_RGB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HOMA1001_RGB", .vendor="ShenzhenHoma",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HOMA1001_RGB, .from_zigbee_count=sizeof(kFz_HOMA1001_RGB)/sizeof(kFz_HOMA1001_RGB[0]),
    .to_zigbee=kTz_HOMA1001_RGB, .to_zigbee_count=sizeof(kTz_HOMA1001_RGB)/sizeof(kTz_HOMA1001_RGB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::shenzhen_homa
