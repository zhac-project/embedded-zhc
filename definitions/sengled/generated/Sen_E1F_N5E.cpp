// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E1F-N5E — hand-rewritten 2026-04-28.
// Element color plus E12
// z2m-source: sengled.ts #E1F-N5E.
// Mapping: sengledLight({colorTemp: {range: [154, 500]}, color: {modes: ["xy"]}}) -> ColorCTLight bundle.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {
constexpr const char* kModels_E1F_N5E[] = { "E1F-N5E" };
}  // namespace

extern const PreparedDefinition kDef_E1F_N5E{
    .zigbee_models=kModels_E1F_N5E, .zigbee_models_count=sizeof(kModels_E1F_N5E)/sizeof(kModels_E1F_N5E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1F-N5E", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposesSengledColorCTLight, .exposes_count=kExposesSengledColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSengledColorCTLight, .from_zigbee_count=kFzSengledColorCTLightCount,
    .to_zigbee=kTzSengledColorCTLight, .to_zigbee_count=kTzSengledColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsSengledColorCTLight, .bindings_count=kBindingsSengledColorCTLightCount,
};

}  // namespace zhc::devices::sengled
