// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AuroraLighting AU-A1GSZ9CX — shared CT-light bundle.
// AOne GLS lamp 9w tunable dimmable 2200-5000K
// z2m-source: aurora_lighting.ts #AU-A1GSZ9CX  (m.light({colorTemp:[200,454]}))
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1GSZ9CX[] = { "TWBulb51AU" };
}  // namespace

extern const PreparedDefinition kDef_AU_A1GSZ9CX{
    .zigbee_models=kModels_AU_A1GSZ9CX,
    .zigbee_models_count=sizeof(kModels_AU_A1GSZ9CX)/sizeof(kModels_AU_A1GSZ9CX[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1GSZ9CX", .vendor="AuroraLighting",
    .meta=nullptr,
    .exposes=kExposesAuroraCTLight, .exposes_count=kExposesAuroraCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAuroraCTLight, .from_zigbee_count=kFzAuroraCTLightCount,
    .to_zigbee=kTzAuroraCTLight,   .to_zigbee_count=kTzAuroraCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAuroraCTLight, .bindings_count=kBindingsAuroraCTLightCount,
};

}  // namespace zhc::devices::aurora_lighting
