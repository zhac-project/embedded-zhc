// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BSEN-C2 — Door/window contact II (RBSH-SWD-ZB).
// Wires the bosch/_shared typed-contact bundle: genPowerCfg battery +
// ssIasZone decoded to the semantic `contact` key (zoneStatus bit 0),
// matching z2m's `reportContactState`. The generic IAS bundle this used
// to share emits a bare `alarm` the SPA can't map; switched to
// kFzBoschContact. Button-action + break-functionality extras live on
// boschDoorWindowContactCluster (0xFCAD) and are manuSpec — not yet
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
    .exposes                 = kExposesBoschContact,
    .exposes_count           = kExposesBoschContactCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzBoschContact,
    .from_zigbee_count       = kFzBoschContactCount,
    .to_zigbee               = nullptr,
    .to_zigbee_count         = 0,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsBoschIasBattery,
    .bindings_count          = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
