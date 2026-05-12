// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E11-N1EA — hand-rewritten 2026-04-28.
// Element plus color (A19)
// z2m-source: sengled.ts #E11-N1EA.
// Mapping: sengledLight({colorTemp: {range: [154, 500]}, color: {modes: ["xy"]}}) -> ColorCTLight bundle.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {
constexpr const char* kModels_E11_N1EA[] = { "E11-N1EA" };
}  // namespace

extern const PreparedDefinition kDef_E11_N1EA{
    .zigbee_models=kModels_E11_N1EA, .zigbee_models_count=sizeof(kModels_E11_N1EA)/sizeof(kModels_E11_N1EA[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E11-N1EA", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposesSengledColorCTLight, .exposes_count=kExposesSengledColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSengledColorCTLight, .from_zigbee_count=kFzSengledColorCTLightCount,
    .to_zigbee=kTzSengledColorCTLight, .to_zigbee_count=kTzSengledColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsSengledColorCTLight, .bindings_count=kBindingsSengledColorCTLightCount,
};

}  // namespace zhc::devices::sengled
