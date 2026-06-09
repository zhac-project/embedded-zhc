// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BSP-FD — Smart plug compact (type G, RBSH-SP2-ZB-EU).
// Was mis-classed as a Light (state/brightness/color_temp) and decoded
// nothing the device sends. Re-targeted to the bosch/_shared smart-plug
// bundle: genOnOff fz/tz + seMetering fz, exposing state/energy/power —
// same bundle as BSP-FZ2. z2m adds onOff + electricityMeter.
// Gaps vs z2m: auto_off / led_brightness / energy_saving_mode
// (boschGeneralEnergyDevice 0xFCA0) + reset_energy_meters (seMetering
// cmd 0x80 mfgcode 0x1209) are manuSpec — see BOSCH_PARITY.md.
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
    .meta=nullptr, .exposes=kExposesBoschPlug, .exposes_count=kExposesBoschPlugCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschPlug, .from_zigbee_count=kFzBoschPlugCount,
    .to_zigbee=kTzBoschPlug, .to_zigbee_count=kTzBoschPlugCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschPlug,
    .bindings_count           = kBindingsBoschPlugCount,
};

}  // namespace zhc::devices::bosch
