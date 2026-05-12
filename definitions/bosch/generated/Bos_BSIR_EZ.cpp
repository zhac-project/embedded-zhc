// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BSIR-EZ — auto-generated.
// Outdoor siren
// z2m-source: bosch.ts #BSIR-EZ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BSIR_EZ[] = { "RBSH-OS-ZB-EU" };

}  // namespace

extern const PreparedDefinition kDef_BSIR_EZ{
    .zigbee_models=kModels_BSIR_EZ, .zigbee_models_count=sizeof(kModels_BSIR_EZ)/sizeof(kModels_BSIR_EZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BSIR-EZ", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschLight, .exposes_count=kExposesBoschLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschLight, .from_zigbee_count=kFzBoschLightCount,
    .to_zigbee=kTzBoschLight, .to_zigbee_count=kTzBoschLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschLight,
    .bindings_count           = kBindingsBoschLightCount,
};

}  // namespace zhc::devices::bosch
