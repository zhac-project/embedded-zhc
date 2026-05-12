// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AuroraLighting AU-A1GUZB5/30 — shared plain-light bundle.
// AOne 4.8W smart dimmable GU10 lamp 3000K
// z2m-source: aurora_lighting.ts #AU-A1GUZB5/30  (m.light())
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1GUZB5_30[] = { "FWGU10Bulb50AU", "FWGU10Bulb01UK" };
}  // namespace

extern const PreparedDefinition kDef_AU_A1GUZB5_30{
    .zigbee_models=kModels_AU_A1GUZB5_30,
    .zigbee_models_count=sizeof(kModels_AU_A1GUZB5_30)/sizeof(kModels_AU_A1GUZB5_30[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1GUZB5/30", .vendor="AuroraLighting",
    .meta=nullptr,
    .exposes=kExposesAuroraLight, .exposes_count=kExposesAuroraLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAuroraLight, .from_zigbee_count=kFzAuroraLightCount,
    .to_zigbee=kTzAuroraLight,   .to_zigbee_count=kTzAuroraLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAuroraLight, .bindings_count=kBindingsAuroraLightCount,
};

}  // namespace zhc::devices::aurora_lighting
