// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BTH-RM230Z — auto-generated.
// Room thermostat II 230V
// z2m-source: bosch.ts #BTH-RM230Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BTH_RM230Z[] = { "RBSH-RTH0-ZB-EU" };

}  // namespace

extern const PreparedDefinition kDef_BTH_RM230Z{
    .zigbee_models=kModels_BTH_RM230Z, .zigbee_models_count=sizeof(kModels_BTH_RM230Z)/sizeof(kModels_BTH_RM230Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BTH-RM230Z", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschLight, .exposes_count=kExposesBoschLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschLight, .from_zigbee_count=kFzBoschLightCount,
    .to_zigbee=kTzBoschLight, .to_zigbee_count=kTzBoschLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschLight,
    .bindings_count           = kBindingsBoschLightCount,
};

}  // namespace zhc::devices::bosch
