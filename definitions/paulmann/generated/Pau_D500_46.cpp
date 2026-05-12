// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Paulmann 500.46 — hand-rewritten to use shared CTLight bundle.
// SmartHome Zigbee MaxLED tunable white controller max. 144W / 24V DC
// z2m-source: paulmann.ts #500.46.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
constexpr const char* kModels_D500_46[] = { "500.46", "CCT" };
}  // namespace

extern const PreparedDefinition kDef_D500_46{
    .zigbee_models=kModels_D500_46, .zigbee_models_count=sizeof(kModels_D500_46)/sizeof(kModels_D500_46[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="500.46", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposesPaulmannCTLight, .exposes_count=kExposesPaulmannCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulmannCTLight, .from_zigbee_count=kFzPaulmannCTLightCount,
    .to_zigbee=kTzPaulmannCTLight, .to_zigbee_count=kTzPaulmannCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulmannCTLight, .bindings_count=kBindingsPaulmannCTLightCount,
};

}  // namespace zhc::devices::paulmann
