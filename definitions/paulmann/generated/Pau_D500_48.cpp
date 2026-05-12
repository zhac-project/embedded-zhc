// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Paulmann 500.48 — hand-rewritten to use shared ColorLight bundle.
// SmartHome Zigbee YourLED dim/switch controller max. 60 W
// z2m-source: paulmann.ts #500.48.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
constexpr const char* kModels_D500_48[] = { "500.48" };
}  // namespace

extern const PreparedDefinition kDef_D500_48{
    .zigbee_models=kModels_D500_48, .zigbee_models_count=sizeof(kModels_D500_48)/sizeof(kModels_D500_48[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="500.48", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposesPaulmannColorLight, .exposes_count=kExposesPaulmannColorLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulmannColorLight, .from_zigbee_count=kFzPaulmannColorLightCount,
    .to_zigbee=kTzPaulmannColorLight, .to_zigbee_count=kTzPaulmannColorLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulmannColorLight, .bindings_count=kBindingsPaulmannColorLightCount,
};

}  // namespace zhc::devices::paulmann
