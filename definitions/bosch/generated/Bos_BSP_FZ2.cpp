// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BSP-FZ2 — Smart plug compact (type F plug).
// Wires the bosch/_shared smart-plug bundle (genOnOff fz/tz +
// seMetering fz, with state/energy/power exposes).
//
// Gaps vs z2m: auto_off (boschGeneralEnergyDevice 0xFCA0 attrs 0x0006/
// 0x0007 mfgcode 0x1209) and reset_energy_meters (seMetering cmd 0x80
// mfgcode 0x1209) are not yet wired — see docs/BOSCH_PARITY.md.
//
// White-labels: BSP-EZ2 (RBSH-SP-ZB-FR), BSP-GZ2 (RBSH-SP-ZB-GB).
// z2m-source: bosch.ts #BSP-FZ2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_BSP_FZ2[] = {
    "RBSH-SP-ZB-EU", "RBSH-SP-ZB-FR", "RBSH-SP-ZB-GB",
};
}  // namespace

extern const PreparedDefinition kDef_BSP_FZ2{
    .zigbee_models           = kModels_BSP_FZ2,
    .zigbee_models_count     = sizeof(kModels_BSP_FZ2)/sizeof(kModels_BSP_FZ2[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "BSP-FZ2",
    .vendor                  = "Bosch",
    .meta                    = nullptr,
    .exposes                 = kExposesBoschPlug,
    .exposes_count           = kExposesBoschPlugCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzBoschPlug,
    .from_zigbee_count       = kFzBoschPlugCount,
    .to_zigbee               = kTzBoschPlug,
    .to_zigbee_count         = kTzBoschPlugCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsBoschPlug,
    .bindings_count          = kBindingsBoschPlugCount,
};

}  // namespace zhc::devices::bosch
