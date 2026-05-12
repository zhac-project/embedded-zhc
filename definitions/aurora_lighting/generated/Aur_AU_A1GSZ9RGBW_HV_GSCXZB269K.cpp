// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AuroraLighting AU-A1GSZ9RGBW_HV-GSCXZB269K — shared full-RGBW bundle.
// AOne 9.5W smart RGBW GLS E27/B22
// z2m-source: aurora_lighting.ts #AU-A1GSZ9RGBW_HV-GSCXZB269K  (m.light({colorTemp:undefined, color:true}))
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1GSZ9RGBW_HV_GSCXZB269K[] = { "RGBBulb01UK", "RGBBulb02UK", "RGBBulb51AU" };
}  // namespace

extern const PreparedDefinition kDef_AU_A1GSZ9RGBW_HV_GSCXZB269K{
    .zigbee_models=kModels_AU_A1GSZ9RGBW_HV_GSCXZB269K,
    .zigbee_models_count=sizeof(kModels_AU_A1GSZ9RGBW_HV_GSCXZB269K)/sizeof(kModels_AU_A1GSZ9RGBW_HV_GSCXZB269K[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1GSZ9RGBW_HV-GSCXZB269K", .vendor="AuroraLighting",
    .meta=nullptr,
    .exposes=kExposesAuroraColorCTLight, .exposes_count=kExposesAuroraColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAuroraColorCTLight, .from_zigbee_count=kFzAuroraColorCTLightCount,
    .to_zigbee=kTzAuroraColorCTLight,   .to_zigbee_count=kTzAuroraColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAuroraColorCTLight, .bindings_count=kBindingsAuroraColorCTLightCount,
};

}  // namespace zhc::devices::aurora_lighting
