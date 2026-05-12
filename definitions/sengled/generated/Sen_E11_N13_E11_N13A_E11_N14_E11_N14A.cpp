// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E11-N13/E11-N13A/E11-N14/E11-N14A — auto-generated.
// Element extra bright (A19)
// z2m-source: sengled.ts #E11-N13/E11-N13A/E11-N14/E11-N14A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {


constexpr const char* kModels_E11_N13_E11_N13A_E11_N14_E11_N14A[] = { "E11-N13", "E11-N13A", "E11-N14", "E11-N14A" };

}  // namespace

extern const PreparedDefinition kDef_E11_N13_E11_N13A_E11_N14_E11_N14A{
    .zigbee_models=kModels_E11_N13_E11_N13A_E11_N14_E11_N14A, .zigbee_models_count=sizeof(kModels_E11_N13_E11_N13A_E11_N14_E11_N14A)/sizeof(kModels_E11_N13_E11_N13A_E11_N14_E11_N14A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E11-N13/E11-N13A/E11-N14/E11-N14A", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposesSengledLight, .exposes_count=kExposesSengledLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSengledLight, .from_zigbee_count=kFzSengledLightCount,
    .to_zigbee=kTzSengledLight, .to_zigbee_count=kTzSengledLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSengledLight,
    .bindings_count           = kBindingsSengledLightCount,
};

}  // namespace zhc::devices::sengled
