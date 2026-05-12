// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BSEN-CV — auto-generated.
// Door/window contact II plus
// z2m-source: bosch.ts #BSEN-CV.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BSEN_CV[] = { "RBSH-SWDV-ZB" };

}  // namespace

extern const PreparedDefinition kDef_BSEN_CV{
    .zigbee_models=kModels_BSEN_CV, .zigbee_models_count=sizeof(kModels_BSEN_CV)/sizeof(kModels_BSEN_CV[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BSEN-CV", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschLight, .exposes_count=kExposesBoschLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschLight, .from_zigbee_count=kFzBoschLightCount,
    .to_zigbee=kTzBoschLight, .to_zigbee_count=kTzBoschLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschLight,
    .bindings_count           = kBindingsBoschLightCount,
};

}  // namespace zhc::devices::bosch
