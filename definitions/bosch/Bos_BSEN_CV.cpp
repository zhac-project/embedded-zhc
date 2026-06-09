// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BSEN-CV — Door/window contact II plus (RBSH-SWDV-ZB).
// Was mis-classed as a Light (state/brightness/color_temp) and decoded
// nothing the device sends. Re-targeted to the bosch/_shared typed-
// contact bundle: genPowerCfg battery + ssIasZone → semantic `contact`
// key (zoneStatus bit 0), matching z2m's `reportContactState`.
// Gaps vs z2m: button-action / break-functionality on
// boschDoorWindowContactCluster (0xFCAD) manuSpec — see BOSCH_PARITY.md.
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
    .meta=nullptr, .exposes=kExposesBoschContact, .exposes_count=kExposesBoschContactCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschContact, .from_zigbee_count=kFzBoschContactCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschIasBattery,
    .bindings_count           = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
