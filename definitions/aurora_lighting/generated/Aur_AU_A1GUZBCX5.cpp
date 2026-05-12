// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AuroraLighting AU-A1GUZBCX5 — shared CT-light bundle.
// AOne 5.4W smart tuneable GU10 lamp
// z2m-source: aurora_lighting.ts #AU-A1GUZBCX5  (m.light({colorTemp:{range:undefined}}))
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1GUZBCX5[] = { "TWGU10Bulb50AU" };
}  // namespace

extern const PreparedDefinition kDef_AU_A1GUZBCX5{
    .zigbee_models=kModels_AU_A1GUZBCX5,
    .zigbee_models_count=sizeof(kModels_AU_A1GUZBCX5)/sizeof(kModels_AU_A1GUZBCX5[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1GUZBCX5", .vendor="AuroraLighting",
    .meta=nullptr,
    .exposes=kExposesAuroraCTLight, .exposes_count=kExposesAuroraCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAuroraCTLight, .from_zigbee_count=kFzAuroraCTLightCount,
    .to_zigbee=kTzAuroraCTLight,   .to_zigbee_count=kTzAuroraCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAuroraCTLight, .bindings_count=kBindingsAuroraCTLightCount,
};

}  // namespace zhc::devices::aurora_lighting
