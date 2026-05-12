// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BSP-FD — auto-generated.
// Smart plug compact [+M]
// z2m-source: bosch.ts #BSP-FD.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BSP_FD[] = { "RBSH-SP2-ZB-EU" };

}  // namespace

extern const PreparedDefinition kDef_BSP_FD{
    .zigbee_models=kModels_BSP_FD, .zigbee_models_count=sizeof(kModels_BSP_FD)/sizeof(kModels_BSP_FD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BSP-FD", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschLight, .exposes_count=kExposesBoschLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschLight, .from_zigbee_count=kFzBoschLightCount,
    .to_zigbee=kTzBoschLight, .to_zigbee_count=kTzBoschLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschLight,
    .bindings_count           = kBindingsBoschLightCount,
};

}  // namespace zhc::devices::bosch
