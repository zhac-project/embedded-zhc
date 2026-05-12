// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DresdenElektronik FLS-PP — hand-edited 2026-04-29c.
// Zigbee Light Link wireless electronic ballast (RGB + white channel).
// z2m: m.deviceEndpoints({rgb:10, white:11}) +
//      m.light({colorTemp:[153,500], color:true, endpointNames:["rgb","white"]}).
// Wire ColorCTLight bundle; endpoint_map keeps the rgb / white labels.
// Note: z2m's source uses ep 10/11; we keep the historic 1/2 mapping
// from the prior generator until a re-extract is performed (TODO).
// z2m-source: dresden_elektronik.ts #FLS-PP.
#include "definitions/dresden_elektronik/_shared.hpp"

namespace zhc::devices::dresden_elektronik {
namespace {
constexpr const char* kModels_FLS_PP[] = { "FLS-PP3", "FLS-PP3 White" };
constexpr ::zhc::EndpointLabel kEndpoints_FLS_PP[] = { {"rgb", 1}, {"white", 2} };
}  // namespace

extern const PreparedDefinition kDef_FLS_PP{
    .zigbee_models=kModels_FLS_PP, .zigbee_models_count=sizeof(kModels_FLS_PP)/sizeof(kModels_FLS_PP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FLS-PP", .vendor="DresdenElektronik",
    .meta=nullptr,
    .exposes=kExposesDresdenColorCTLight, .exposes_count=kExposesDresdenColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzDresdenColorCTLight, .from_zigbee_count=kFzDresdenColorCTLightCount,
    .to_zigbee=kTzDresdenColorCTLight, .to_zigbee_count=kTzDresdenColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsDresdenColorCTLight, .bindings_count=kBindingsDresdenColorCTLightCount,
    .endpoint_map       = kEndpoints_FLS_PP,
    .endpoint_map_count = sizeof(kEndpoints_FLS_PP)/sizeof(kEndpoints_FLS_PP[0]),
};

}  // namespace zhc::devices::dresden_elektronik
