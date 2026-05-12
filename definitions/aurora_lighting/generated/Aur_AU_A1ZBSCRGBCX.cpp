// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AuroraLighting AU-A1ZBSCRGBCX — shared full-RGBW bundle.
// RGBW LED strip controller
// z2m-source: aurora_lighting.ts #AU-A1ZBSCRGBCX  (m.light({colorTemp:[166,400], color:true}))
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1ZBSCRGBCX[] = { "RGBCXStrip50AU" };
}  // namespace

extern const PreparedDefinition kDef_AU_A1ZBSCRGBCX{
    .zigbee_models=kModels_AU_A1ZBSCRGBCX,
    .zigbee_models_count=sizeof(kModels_AU_A1ZBSCRGBCX)/sizeof(kModels_AU_A1ZBSCRGBCX[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBSCRGBCX", .vendor="AuroraLighting",
    .meta=nullptr,
    .exposes=kExposesAuroraColorCTLight, .exposes_count=kExposesAuroraColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAuroraColorCTLight, .from_zigbee_count=kFzAuroraColorCTLightCount,
    .to_zigbee=kTzAuroraColorCTLight,   .to_zigbee_count=kTzAuroraColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAuroraColorCTLight, .bindings_count=kBindingsAuroraColorCTLightCount,
};

}  // namespace zhc::devices::aurora_lighting
