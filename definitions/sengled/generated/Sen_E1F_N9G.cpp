// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E1F-N9G — auto-generated.
// Smart LED filament candle (E12)
// z2m-source: sengled.ts #E1F-N9G.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {


constexpr const char* kModels_E1F_N9G[] = { "E1F-N9G" };

}  // namespace

extern const PreparedDefinition kDef_E1F_N9G{
    .zigbee_models=kModels_E1F_N9G, .zigbee_models_count=sizeof(kModels_E1F_N9G)/sizeof(kModels_E1F_N9G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1F-N9G", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposesSengledLight, .exposes_count=kExposesSengledLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSengledLight, .from_zigbee_count=kFzSengledLightCount,
    .to_zigbee=kTzSengledLight, .to_zigbee_count=kTzSengledLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSengledLight,
    .bindings_count           = kBindingsSengledLightCount,
};

}  // namespace zhc::devices::sengled
