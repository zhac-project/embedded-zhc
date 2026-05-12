// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Paulmann 371000002 — hand-rewritten to use shared ColorCTLight bundle.
// Amaris LED panels
// z2m-source: paulmann.ts #371000002.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
constexpr const char* kModels_D371000002[] = { "371000002", "RGBW" };
}  // namespace

extern const PreparedDefinition kDef_D371000002{
    .zigbee_models=kModels_D371000002, .zigbee_models_count=sizeof(kModels_D371000002)/sizeof(kModels_D371000002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="371000002", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposesPaulmannColorCTLight, .exposes_count=kExposesPaulmannColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulmannColorCTLight, .from_zigbee_count=kFzPaulmannColorCTLightCount,
    .to_zigbee=kTzPaulmannColorCTLight, .to_zigbee_count=kTzPaulmannColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulmannColorCTLight, .bindings_count=kBindingsPaulmannColorCTLightCount,
};

}  // namespace zhc::devices::paulmann
