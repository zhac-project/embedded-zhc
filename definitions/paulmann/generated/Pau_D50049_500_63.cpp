// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Paulmann 50049/500.63 — hand-rewritten to use shared ColorCTLight bundle.
// Smart Home Zigbee YourLED RGB Controller / LED Reflektor 3,5W GU10 RGBW dimmbar
// z2m-source: paulmann.ts #50049/500.63.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
constexpr const char* kModels_D50049_500_63[] = { "RGBW light", "500.49", "RGBW_light" };
}  // namespace

extern const PreparedDefinition kDef_D50049_500_63{
    .zigbee_models=kModels_D50049_500_63, .zigbee_models_count=sizeof(kModels_D50049_500_63)/sizeof(kModels_D50049_500_63[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="50049/500.63", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposesPaulmannColorCTLight, .exposes_count=kExposesPaulmannColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulmannColorCTLight, .from_zigbee_count=kFzPaulmannColorCTLightCount,
    .to_zigbee=kTzPaulmannColorCTLight, .to_zigbee_count=kTzPaulmannColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulmannColorCTLight, .bindings_count=kBindingsPaulmannColorCTLightCount,
};

}  // namespace zhc::devices::paulmann
