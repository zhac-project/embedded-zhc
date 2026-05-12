// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BSEN-C2 — Door/window contact II (RBSH-SWD-ZB).
// Wires the bosch/_shared IAS-zone battery sensor bundle (battery +
// IAS contact alarm + tamper). The button-action and break-functionality
// extras live on boschDoorWindowContactCluster (0xFCAD) and are not yet
// wired — see docs/BOSCH_PARITY.md.
// z2m-source: bosch.ts #BSEN-C2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_BSEN_C2[] = { "RBSH-SWD-ZB" };
}  // namespace

extern const PreparedDefinition kDef_BSEN_C2{
    .zigbee_models           = kModels_BSEN_C2,
    .zigbee_models_count     = sizeof(kModels_BSEN_C2)/sizeof(kModels_BSEN_C2[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "BSEN-C2",
    .vendor                  = "Bosch",
    .meta                    = nullptr,
    .exposes                 = kExposesBoschIasBattery,
    .exposes_count           = kExposesBoschIasBatteryCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzBoschIasBattery,
    .from_zigbee_count       = kFzBoschIasBatteryCount,
    .to_zigbee               = nullptr,
    .to_zigbee_count         = 0,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsBoschIasBattery,
    .bindings_count          = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
